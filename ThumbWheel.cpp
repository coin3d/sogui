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

#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "ThumbWheel.h"

/*!
  \class ThumbWheel ThumbWheel.h
  \brief The ThumbWheel class is a helper class for managing thumb wheel
  GUI widgets.
  \internal
*/

/*!
  \var bool ThumbWheel::dirtyTables
  If this flag is set, the internal tables needs to be recalculated.
*/

/*!
  \var bool ThumbWheel::dirtyVariables
  If this flag is set, the internal variables that are calculated from the
  tables and wheel settings need to be recalculated.
*/

inline
int
int8clamp( float f ) {
  assert( f >= 0.0f );
  if ( f >= 255.0f )
    return 255;
  return (int) floor( f );
} // int8clamp()

// ************************************************************************

/*!
  Constructor.
*/

ThumbWheel::ThumbWheel(
  void )
: diameter( 0 )
, width( 0 )
, byteorder( ABGR )
, handling( MODULATE )
, method( AUTHENTIC )
, dirtyTables( true )
, dirtyVariables( true )
{
  assert( sizeof(int) == 4 && "FIXME: use int32 datatype instead" );

  disabledred = 255;
  disabledgreen = 255;
  disabledblue = 255;

  red = 180;
  green = 180;
  blue = 220;
  
  light = 1.6f;
  front = 1.2f;
  normal = 1.0f;
  shade = 0.8f;

  for ( int i = 0; i < NUMTABLES; i++ )
    this->tables[i] = NULL;

} // ThumbWheel()

// ************************************************************************

/*!
  Destructor.
*/

ThumbWheel::~ThumbWheel(
  void )
{
  for ( int i = 0; i < NUMTABLES; i++ )
    delete [] this->tables[i];
} // ~ThumbWheel()

// ************************************************************************

/*!
*/

void
ThumbWheel::SetWheelSize(
  int diameter,
  int width )
{
  if ( this->diameter != diameter ) {
    this->dirtyTables = true;
    this->dirtyVariables = true;
  } else if ( this->width != width ) {
    this->dirtyVariables = true; // embossed squares grows...
  } else {
    return;
  }
  this->diameter = diameter;
  this->width = width;
} // SetWheelSize()

// ************************************************************************

/*!
*/

void
ThumbWheel::GetWheelSize(
  int & diameter,
  int & width ) const
{
  diameter = this->diameter;
  width = this->width;
} // GetWheelSize()

// ************************************************************************

/*!
*/

void
ThumbWheel::SetColor(
  int red,
  int green,
  int blue )
{
  this->red = red;
  this->green = green;
  this->blue = blue;
} // SetColor()

// ************************************************************************

/*!
*/

void
ThumbWheel::GetColor(
  int & red,
  int & green,
  int & blue ) const
{
  red = this->red;
  green = this->green;
  blue = this->blue;
} // GetColor()

// ************************************************************************

/*!
*/

void
ThumbWheel::SetColorFactors(
  float light,
  float front,
  float normal,
  float shade )
{
  this->light = light;
  this->front = front;
  this->normal = normal;
  this->shade = shade;
} // SetColorFactors()

// ************************************************************************

/*!
*/

void
ThumbWheel::GetColorFactors(
  float & light,
  float & front,
  float & normal,
  float & shade )
{
  light = this->light;
  front = this->front;
  normal = this->normal;
  shade = this->shade;
} // GetColorFactors()

// ************************************************************************

/*!
  This method returns the number of bitmaps required to represent all the
  possible rotations of the thumb wheel.
*/

int
ThumbWheel::BitmapsRequired(
  void ) const
{
  return this->width - 4 + 2 + 1;
} // BitmapsRequired()

// ************************************************************************

/*!
  This method draws thumb wheel bitmap number num.
*/

void
ThumbWheel::DrawBitmap(
  int number,
  void * bitmap,
  bool vertical )
{
  if ( number == 0 ) {
    DrawDisabledWheel( number, bitmap, vertical );
  } else {
    DrawEnabledWheel( number, bitmap, vertical );
  }
}

// ************************************************************************

/*!
  This method calculates the thumb wheel value based on the users mouse
  interaction.  origpos and origval is the mouseposition at the moment of
  pressing the button, and origval is the thumbwheels original value at
  that point.  deltapos is the number of pixels the mouse has moved since
  pressing the button.
  
  The point in doing it this way is that the thumbwheel can easily be made
  to behave like an authentic wheel when all of these parameters are known.

  After doing a CalculateValue, you should redraw the wheel if the value has
  changed (since last time, not compared to origval).
*/

float
ThumbWheel::CalculateValue(
  float origval,
  int origpos,
  int deltapos )
{
  this->Validate();

//  fprintf( stderr, "calc( %8.4f, %d, %d)\n", origval, origpos, deltapos );

  float diff = 0.0f;
  switch ( this->method ) {
  case UNIFORM:
    diff = this->unistep * deltapos;
    break;
  case AUTHENTIC:
    int newpos = origpos + deltapos;
    if ( newpos < 0 ) newpos = 0;
    if ( newpos >= diameter ) newpos = diameter - 1;
    diff = this->tables[RAD][newpos] - this->tables[RAD][origpos];
    break;
  } // switch ( this->method )

  switch ( this->handling ) {
  case MODULATE:
    while ( (origval + diff) < 0.0f          ) diff += 2.0f * M_PI;
    while ( (origval + diff) > (2.0f * M_PI) ) diff -= 2.0f * M_PI;
    break;
  case CLAMP:
    if    ( (origval + diff) < 0.0f          ) diff = 0.0f - origval;
    if    ( (origval + diff) > (2.0f * M_PI) ) diff = (2.0f * M_PI) - origval;
    break;
  case ACCUMULATE:
    // nothing - just add the difference on top of original value
    break;
  } // switch ( this->handling )

  return origval + diff;
} // CalculateValue()

// ************************************************************************

/*!
  This method returns the bitmap you need to display to represent a thumb
  wheel with the given value and active state.
*/

int
ThumbWheel::GetBitmapForValue(
  float value,
  bool enabled )
{
  this->Validate();

  if ( enabled == false )
    return 0; // only one disabled bitmap in this implementation

//  float numsquares = 
  float squarerange = (2.0f * M_PI) / (float) numsquares;
  float normalizedmodval = fmod( value, squarerange ) / squarerange;
  int bitmap = 1 + (int) floor( normalizedmodval * (float) (this->width - 4 + 2));
//  return 1 + (squaresize + 2) * fmod( value, (2.0f*M_PI) / numsquares );
//  fprintf( stderr, "bitmap = %d\n", bitmap );
  return bitmap;
} // GetBitmapForValue()

// ************************************************************************

/*!
  This method sets whether to set up uints in ARGB or BGRA mode before
  storing them in the 32-bit frame buffer.
*/

void
ThumbWheel::SetGraphicsByteOrder(
  const GraphicsByteOrder byteorder )
{
  this->byteorder = byteorder;
} // SetGraphicsByteOrder()

// ************************************************************************

/*!
*/

ThumbWheel::GraphicsByteOrder
ThumbWheel::GetGraphicsByteOrder(
  void ) const
{
  return this->byteorder;
} // GetGraphicsByteOrder()

// ************************************************************************

/*!
*/

void
ThumbWheel::SetWheelMotionMethod(
  const WheelMotionMethod method )
{
  this->method = method;
} // SetWheelMotionMethod()

// ************************************************************************

/*!
*/

ThumbWheel::WheelMotionMethod
ThumbWheel::GetWheelMotionMethod(
  void ) const
{
  return this->method;
} // GetWheelMotionMethod()

// ************************************************************************

void
ThumbWheel::SetWheelRangeBoundaryHandling(
  const WheelRangeBoundaryHandling handling )
{
  this->handling = handling;
} // SetWheelRangeBoundaryHandling()

// ************************************************************************

/*!
*/

ThumbWheel::WheelRangeBoundaryHandling
ThumbWheel::GetWheelRangeBoundaryHandling(
  void ) const
{
  return this->handling;
} // GetWheelRangeBoundaryHandling()

// ************************************************************************

/*!
  This method validates the ThumbWheel object, recalculating the tables and
  state variables if necessary.  If object is already valid, no calculations
  are performed.
*/

void
ThumbWheel::Validate( // private
  void )
{
  if ( this->dirtyTables != false ) {
    assert( this->dirtyVariables != false );
    for ( int i = 0; i < NUMTABLES; i++ ) {
      if ( this->tables[i] ) delete [] this->tables[i];
      this->tables[i] = new float [ this->diameter ];
    }

    float radius = ((float) this->diameter - 1.0f) / 2.0f;
    float range = 2.0f * M_PI;
    float acos0times2 = 2.0f*acos( 0.0f );

    for ( int i = 0; i < this->diameter; i++ ) {
      if ( (float) i <= radius ) {
        this->tables[COS][i] = (radius - (float) i) / radius;
        this->tables[RAD][i] = acos( this->tables[COS][i] );
      } else {
        this->tables[COS][i] = ((float) i - radius) / radius;
        this->tables[RAD][i] = acos0times2 - acos(this->tables[COS][i]);
      }
      this->tables[SIN][i] = sqrt( 1.0f - this->tables[COS][i] * this->tables[COS][i] );
    }

    this->dirtyTables = false;
  }

  if ( this->dirtyVariables != false ) {
    assert( this->dirtyTables == false );
    if ( (this->diameter % 2) == 0)
      this->unistep = this->tables[RAD][this->diameter/2] -
                      this->tables[RAD][(this->diameter/2)-1];
    else
      this->unistep = (this->tables[RAD][(this->diameter/2)+1] -
                       this->tables[RAD][(this->diameter/2)-1]) / 2.0f;

    this->squarespacing = 2.0f * this->unistep;
    this->shadelength = this->unistep; // remove?

    int squares = (int) floor(((2.0f * M_PI) /
                         ((((float) width - 4.0f) * this->unistep) + this->squarespacing)) + 0.5f);
    this->numsquares = squares;
    this->squarelength = ((2.0f * M_PI) / (float) squares) - this->squarespacing;

    this->dirtyVariables = false;
  }
} // Validate()

// ************************************************************************

/*!
  This method draws a wheel that has been disabled from being rotated.
*/

void
ThumbWheel::DrawDisabledWheel( // private
  int number,
  void * bitmap,
  bool vertical )
{
  assert( number == 0 );

  this->Validate();

  unsigned int * buffer = (unsigned int *) bitmap;

  for ( int j = 0; j < this->diameter; j++ ) {
    unsigned int light, normal, shade;
    light  = (unsigned int) int8clamp( floor(255.0f * this->tables[SIN][j] * 1.15f) );
    normal = (unsigned int) floor(255.0f * this->tables[SIN][j]);
    shade  = (unsigned int) floor(255.0f * this->tables[SIN][j] * 0.85f);
    if ( this->byteorder == ABGR ) {
      // FIXME: use disabledblue / disabledgreen / disabledred;
      light  =  light |  (light << 8) |  (light << 16);
      normal = normal | (normal << 8) | (normal << 16);
      shade  =  shade |  (shade << 8) |  (shade << 16);
    } else {
      light  =  (light << 24) |  (light << 16) |  (light << 8);
      normal = (normal << 24) | (normal << 16) | (normal << 8);
      shade  =  (shade << 24) |  (shade << 16) |  (shade << 8);
    }
    if ( vertical == true ) {
      buffer[j*this->width] = light;
      for ( int i = 1; i < (width - 1); i++ )
        buffer[(j*this->width)+i] = normal;
      buffer[(j*this->width)+(this->width-1)] = shade;
    } else {
      buffer[j] = light;
      for ( int i = 1; i < (this->width-1); i++ )
        buffer[j+(i*this->diameter)] = normal;
      buffer[j+((this->width-1)*this->diameter)] = shade;
    }
  }
} // DrawDisabledWheel()

// ************************************************************************

/*
  This method draws the thumb wheel.
  Shading/lighting aliasing can still be done better...
  Can also optimize the drawing to use less computing power...
*/

void
ThumbWheel::DrawEnabledWheel(
  int number,
  void * bitmap,
  bool vertical )
{
  this->Validate();

  int numEnabledBitmaps = this->width - 4 + 2;
  assert( number > 0 && number <= numEnabledBitmaps );

  float modulo = (2.0f * M_PI) / numsquares;
  float radian = modulo - (((2.0f * M_PI) / (float) numsquares) * (((float) (number - 1)) / (float) numEnabledBitmaps));
//  fprintf( stderr, "radoffset = %8.4f\n", radoffset );
//  DrawDisabledWheel( 0, bitmap, vertical );

  bool newsquare = true;
  unsigned int * buffer = (unsigned int *) bitmap;
  for ( int j = 0; j < this->diameter; j++ ) {
    unsigned int light, normal, shade;
    light  = (unsigned int) int8clamp( floor(255.0f * this->tables[SIN][j] * 1.15f) );
    normal = (unsigned int) floor(255.0f * this->tables[SIN][j]);
    shade  = (unsigned int) floor(255.0f * this->tables[SIN][j] * 0.85f);

    if ( newsquare ) {
      normal = 0x00000000;
      newsquare = false;
    } else {
      normal = 0x00ffffff;
    }

    if ( vertical == true ) {
      for ( int i = 0; i < (this->width); i++ )
        buffer[(j*this->width)+i] = normal;
    } else {
      for ( int i = 0; i < (this->width); i++ )
        buffer[j+(i*this->diameter)] = normal;
    }
 


    if ( j < (this->diameter - 1) ) {
      radian += this->tables[RAD][j+1] - this->tables[RAD][j];
      if ( radian > modulo ) {
        radian = fmod( radian, modulo );
        newsquare = true;
      }
    }

  }
/*
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
    SetColor( int8c(factor*red), int8c(factor*green), int8c(factor*blue) );
    StrokeLine( 0, i, 1, i );
    StrokeLine( width - 1, i, width, i );

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

    SetColor( int8clamp(factor*red), int8clamp(factor*green), int8clamp(factor*blue) );
    StrokeLine( 2, i, this->width - 2, i );
  }
*/
} // DrawEnabledWheel()

// ************************************************************************

