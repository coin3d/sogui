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

//  $Id$

#ifndef __THUMBWHEEL_H__
#define __THUMBWHEEL_H__

// ************************************************************************

class ThumbWheel {
public:
  ThumbWheel(void);
  ~ThumbWheel(void);

// visual
  void SetWheelSize(int diameter, int width);
  void GetWheelSize(int & diameter, int & width) const;
  int GetDiameter(void) const;
  int GetWidth(void) const;
  void SetColor(int red, int green, int blue);
  void GetColor(int & red, int & green, int & blue) const;
  void SetFactors(float light, float front, float normal, float shade);
  void GetFactors(float & light, float & front, float & normal, float & shade);

//  void Draw(GUIPencil * const pencil);

// operational
  void SetRange(float min, float max);
  void GetRange(float & min, float & max) const;
  float GetRangeMin(void) const;
  float GetRangeMax(void) const;
  void SetValue(float val);
  float GetValue(void) const;

  void SetModulate(bool modulate);
  bool GetModulate(void) const;
  void SetAuthentic(bool authentic);
  bool GetAuthentic(void) const;

  float Adjust(int origpos, float origval, int deltapos);

  void dump(void);

private:
  int diameter, width;
  int red, green, blue;
  float light, front, normal, shade;

  float min, max, val;
  bool authentic, modulate;

//  void Draw(GUIPencil * const pencil, float value);
  
  void precalc(void);
  // precalculated tables and values based on pixel offsets into wheel picture
  float * sines, * cosines, * radians, * metrics;
  float squarelength; // length of "sunken squares" in wheel
  float squarespacing; // space between squares (two pixels)
  float shadelength;
  float unistep; // uniform steplength
  bool invalid;
}; // class ThumbWheel

// ************************************************************************

#endif // ! __THUMBWHEEL_H__
