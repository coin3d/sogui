/**************************************************************************\
 *
 *  Copyright (C) 1999-2000 by Systems in Motion.  All rights reserved.
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

//  $Id$

#ifndef THUMBWHEEL_H
#define THUMBWHEEL_H

// ************************************************************************

class ThumbWheel {
public:
  ThumbWheel(void);
  ~ThumbWheel(void);

  enum RenderDirection {
    VERTICAL, HORIZONTAL
  };

  enum WheelState {
    DISABLED, ENABLED
  };

  void SetWheelSize( int diameter, int width );
  void GetWheelSize( int & diameter, int & width ) const;

  void SetColor( int red, int green, int blue );
  void GetColor( int & red, int & green, int & blue ) const;
  void SetColorFactors( float light, float front, float normal, float shade );
  void GetColorFactors( float & light, float & front, float & normal, float & shade );

  int BitmapsRequired(void) const;
  void DrawBitmap( int number, void * bitmap, RenderDirection direction );
  float CalculateValue( float origValue, int origPosition, int deltaPosition );
  int GetBitmapForValue( float value, WheelState state );

  enum GraphicsByteOrder {
    ABGR,
    RGBA
  };

  void SetGraphicsByteOrder( const GraphicsByteOrder byteorder );
  GraphicsByteOrder GetGraphicsByteOrder(void) const;

  enum WheelMotionMethod {
    UNIFORM,
    AUTHENTIC
  };

  void SetWheelMotionMethod( const WheelMotionMethod method );
  WheelMotionMethod GetWheelMotionMethod(void) const;

  enum WheelRangeBoundaryHandling {
    MODULATE,
    ACCUMULATE,
    CLAMP
  };

  void SetWheelRangeBoundaryHandling( const WheelRangeBoundaryHandling handling );
  WheelRangeBoundaryHandling GetWheelRangeBoundaryHandling(void) const;

private:
  int diameter, width;
  int disabledred, disabledgreen, disabledblue;
  int red, green, blue;
  float light, front, normal, shade;

  GraphicsByteOrder           byteorder;
  WheelRangeBoundaryHandling  handling;
  WheelMotionMethod           method;

  void Validate(void);

  enum Tables {
    SIN,
    COS,
    RAD,
    NUMTABLES
  };

  float * tables[NUMTABLES];
  int dirtyTables;
  int dirtyVariables;
  float squarelength, squarespacing, shadelength, unistep, numsquares;

  void DrawDisabledWheel(int number, void * bitmap, RenderDirection direction );
  void DrawEnabledWheel( int number, void * bitmap, RenderDirection direction );
}; // class ThumbWheel

// ************************************************************************

#endif // ! THUMBWHEEL_H
