/**************************************************************************\
 *
 *  Copyright (C) 1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

static const char rcsid[] =
  "$Id$";

#include <stdio.h>
#include <math.h>

#include "ThumbWheel.h"

// ************************************************************************

ThumbWheel::ThumbWheel(
  void )
: authentic( false )
, modulate( true )
, invalid( true )
, sines( NULL )
, cosines( NULL )
, radians( NULL )
, metrics( NULL )
{
  red = 180;
  green = 180;
  blue = 220;
  
  light = 1.6f;
  front = 1.2f;
  normal = 1.0f;
  shade = 0.8f;
  diameter = 0;
  width = 0;
} // ThumbWheel()

// ************************************************************************

ThumbWheel::~ThumbWheel(
  void )
{
  if ( this->sines != NULL ) delete [] this->sines;
  if ( this->cosines != NULL ) delete [] this->cosines;
  if ( this->radians != NULL ) delete [] this->radians;
  if ( this->metrics != NULL ) delete [] this->metrics;
} // ~ThumbWheel()

// ************************************************************************

void
GUIThumbwheel::SetWheelSize(
  int diameter,
  int width )
{
  if ( this->diameter != diameter || this->width != width ) {
    this->diameter = diameter;
    this->width = width;
    this->invalid = true;
  }
} // SetWheelSize()

void
GUIThumbwheel::GetWheelSize(
  int & diameter,
  int & width ) const
{
  diameter = this->diameter;
  width = this->width;
} // GetWheelSize()

int
GUIThumbwheel::GetDiameter(
  void ) const
{
  return this->diameter;
} // GetDiameter()

int
GUIThumbwheel::GetWidth(
  void ) const
{
  return this->width;
} // GetWidth()

void
GUIThumbwheel::SetColor(
  int red,
  int green,
  int blue )
{
  this->red = red;
  this->green = green;
  this->blue = blue;
} // SetColor()

void
GUIThumbwheel::GetColor(
  int & red,
  int & green,
  int & blue ) const
{
  red = this->red;
  green = this->green;
  blue = this->blue;
} // GetColor()

void
GUIThumbwheel::SetFactors(
  float light,
  float front,
  float normal,
  float shade )
{
  this->light = light;
  this->front = front;
  this->normal = normal;
  this->shade = shade;
} // SetFactors()

void
GUIThumbwheel::GetFactors(
  float & light,
  float & front,
  float & normal,
  float & shade )
{
  light = this->light;
  front = this->front;
  normal = this->normal;
  shade = this->shade;
} // GetFactors()

// ************************************************************************

inline int int8c( float f ) {
  if ( f >= 256.0 )
    return 255;
  return (int) floor( f );
}

/*!
  This method draws the thumbwheel, using a platform-independent GUIPencil
  interface object.  Shading/lighting aliasing can still be done better...
  Can also optimize the drawing to use less computing power...
*/

void
GUIThumbwheel::Draw(
  GUIPencil * const pencil )
{
  Draw( pencil, this->val );
} // Draw()

void
GUIThumbwheel::Draw(
  GUIPencil * const pencil,
  float value )
{
  pencil->Save();
  precalc();

  enum LineMode {
    LINE_SQUARE,
    LINE_SHADE_ALIAS,
    LINE_SHADE,
    LINE_FRONT,
    LINE_LIGHT_ALIAS,
    LINE_LIGHT
  };

  float totlen = squarelength + squarespacing + shadelength;

  LineMode mode = LINE_SHADE;
  for ( int i = 0; i < this->diameter; i++ ) {
    // bright edges
    float factor = front * sines[i];
    pencil->SetColor( int8c(factor*red), int8c(factor*green), int8c(factor*blue) );
    pencil->StrokeLine( 0, i, 1, i );
    pencil->StrokeLine( width - 1, i, width, i );

    // sunken-square pattern
    float pixpos = metrics[i] + fmod( -value, totlen );
    float squarepos = fmod( pixpos, totlen );
    float shadefact = float(i)/float(diameter);
    float lightfact = 1.0f - shadefact;
    if ( squarepos <= squarelength ) {
        if ( mode == LINE_FRONT && lightfact > 0.25f )
          factor = light * sines[i];
        else
          factor = normal * sines[i];
        mode = LINE_SQUARE;
    } else if ( squarepos <= (squarelength + (shadelength * shadefact) ) ) {
        // antialiased shading
        factor = shade * sines[i] * shadefact + normal * sines[i] * lightfact;
        mode = LINE_SHADE_ALIAS;
    } else if ( squarepos <= (squarelength + shadelength) ) {
        factor = shade * sines[i];
        mode = LINE_SHADE;
    } else if ( squarepos <= (squarelength + (shadelength * shadefact) + squarespacing) ) {
        if ( mode == LINE_SQUARE && shadefact > 0.25f )
          factor = shade * sines[i];
        else
          factor = front * sines[i];
        mode = LINE_FRONT;
    } else if ( squarepos <= (squarelength + (shadelength * lightfact) + squarespacing) ) {
        // antialiased light
        factor = front * sines[i] * shadefact + light * sines[i] * lightfact;
        mode = LINE_LIGHT_ALIAS;
    } else {
        factor = light * sines[i];
        mode = LINE_LIGHT;
    }

    pencil->SetColor( int8c(factor*red), int8c(factor*green), int8c(factor*blue) );
    pencil->StrokeLine( 2, i, this->width - 2, i );
  }
  pencil->Restore();
} // Draw()

// ************************************************************************

void
GUIThumbwheel::SetRange(
  float min,
  float max )
{
  if ( this->min != min || this->max != max ) {
    this->min = min;
    this->max = max;
    this->invalid = true;
  }
} // SetRange()

void
GUIThumbwheel::GetRange(
  float & min,
  float & max ) const
{
  min = this->min;
  max = this->max;
} // GetRange()

float
GUIThumbwheel::GetRangeMin(
  void ) const
{
  return this->min;
} // GetRangeMin()

float
GUIThumbwheel::GetRangeMax(
  void ) const
{
  return this->max;
} // GetRangeMax()

void
GUIThumbwheel::SetValue(
  float val )
{
  this->val = val;
} // SetValue()

float
GUIThumbwheel::GetValue(
  void ) const
{
  return this->val;
} // GetValue()

void
GUIThumbwheel::SetModulate(
  bool modulate )
{
  this->modulate = modulate;
} // SetModulate()

bool
GUIThumbwheel::GetModulate(
  void ) const
{
  return this->modulate;
} // GetModulate()

void
GUIThumbwheel::SetAuthentic(
  bool authentic )
{
  this->authentic = authentic;
} // SetAuthentic()

bool
GUIThumbwheel::GetAuthentic(
  void ) const
{
  return this->authentic;
} // GetAuthentic()

/*!
  This method adjusts the thumbwheel value based on the users mouse
  interaction.  origpos and origval is the mouseposition at the moment of
  pressing the button, and origval is the thumbwheels original value at
  that point.  deltapos is the number of pixels the mouse has moved since
  pressing the button.
  
  The point in doing it this way is that the thumbwheel can easily be made
  to behave like an authentic wheel when all of these parameters are known.

  After doing an Adjust, you should redraw the wheel if the value has
  changed (since last time, not compared to origval).
*/

float
GUIThumbwheel::Adjust(
  int origpos,
  float origval,
  int deltapos )
{
  float diff;
  if ( authentic == false ) {
    diff = unistep * deltapos;
  } else {
    int newpos = origpos + deltapos;
    if ( newpos < 0 ) newpos = 0;
    if ( newpos >= diameter ) newpos = diameter - 1;
    diff = metrics[newpos] - metrics[origpos];
  }
  if ( modulate == true ) {
    while ( (origval + diff) < min ) diff += (max - min);
    while ( (origval + diff) > max ) diff -= (max - min);
  } else {
    if ( (origval + diff) < min ) diff = min - origval;
    else if ( (origval + diff) > max ) diff = max - origval;
  }
  val = origval + diff;
  return origval + diff;
} // Adjust()

// ************************************************************************

void
GUIThumbwheel::precalc(
  void )
{
  if ( this->invalid == true ) {
    if ( this->sines != NULL ) delete [] this->sines;
    this->sines = new float [this->diameter];
    if ( this->cosines != NULL ) delete [] this->cosines;
    this->cosines = new float [this->diameter];
    if ( this->radians != NULL ) delete [] this->radians;
    this->radians = new float [this->diameter];
    if ( this->metrics != NULL ) delete [] this->metrics;
    this->metrics = new float [this->diameter];

    float radius = (float(this->diameter) - 1.0f) / 2.0f;

    for ( int i = 0; i < this->diameter; i++ ) {
      if ( float(i) <= radius ) {
        this->cosines[i] = (radius - float(i)) / radius;
        this->radians[i] = acos(this->cosines[i]);
      } else {
        this->cosines[i] = (float(i) - radius) / radius;
        this->radians[i] = (2.0f * acos(0.0f)) - acos(this->cosines[i]);
      }
      this->sines[i] = sqrt( 1.0f - this->cosines[i] * this->cosines[i] );
      this->metrics[i] = ((this->max - this->min) / (2.0f*3.1415926535f)) * this->radians[i];
    }

    if ( (this->diameter % 2) == 0)
      unistep = metrics[diameter/2] - metrics[(diameter/2)-1];
    else
      unistep = (metrics[(diameter/2)+1] - metrics[(diameter/2)-1]) / 2.0f;

    squarespacing = 2.0 * unistep;
    shadelength = unistep;

    int squares = (int) ((max-min) /
                         (((float(width)-4.0f) * unistep) + squarespacing));
    squarelength = ((max-min) / float(squares)) - squarespacing;

    this->invalid = false;
  }
} // precalc()

// ************************************************************************

void
GUIThumbwheel::dump(
  void )
{
  precalc();
  printf("dump()\n");
  printf( "N\tcosine\t\tsine\t\tradian\t\tmetrics\n" );
  printf( "=\t======\t\t====\t\t======\t\t=======\n" );
  for ( int i = 0; i < this->diameter; i++ )
    printf( "%03i\t%f\t%f\t%f\t%f\n", i, this->cosines[i], this->sines[i],
            this->radians[i], this->metrics[i] );
  printf( "unistep:  %f\nsquarelength:  %f  spacing: %f\n",
    unistep, squarelength, squarespacing );
} // dump()

// ************************************************************************
