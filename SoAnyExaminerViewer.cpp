/**************************************************************************\
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
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

/*!
  \class SoAnyExaminerViewer SoAnyExaminerViewer.h
  \brief The SoAnyExaminerViewer class is a collection of common functions for
  all the ExaminerViewer components for Coin.
*/

#include <Inventor/nodes/SoCamera.h>

#include "SoAnyExaminerViewer.h"

// ************************************************************************

/*!
  The "rotX" wheel is the wheel on the left decoration on the examiner
  viewer.  This function translates interaction with the "rotX" wheel into
  camera movement.
*/

float
SoAnyExaminerViewer::rotXWheelMotion(
  float value,
  float oldvalue,
  SoCamera * camera )
{
  assert( camera != NULL );

  SbVec3f dir;
  camera->orientation.getValue().multVec( SbVec3f( 0, 0, -1 ), dir );

  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * dir;

  camera->orientation = SbRotation( SbVec3f( 1, 0, 0 ), value - oldvalue ) *
    camera->orientation.getValue();

  camera->orientation.getValue().multVec( SbVec3f(0, 0, -1), dir );
  camera->position = focalpoint - camera->focalDistance.getValue() * dir;

  return value;
} // rotXWheelMotion()

/*!
  The "rotY" wheel is the wheel on the bottom decoration on the examiner
  viewer.  This function translates interaction with the "rotX" wheel into
  camera movement.
*/

float
SoAnyExaminerViewer::rotYWheelMotion(
  float value,
  float oldvalue,
  SoCamera * camera )
{
  assert( camera != NULL );

  SbVec3f dir;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), dir);

  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * dir;

  camera->orientation = SbRotation( SbVec3f( 0, 1, 0 ), oldvalue - value ) *
    camera->orientation.getValue();

  camera->orientation.getValue().multVec( SbVec3f(0, 0, -1), dir );
  camera->position = focalpoint - camera->focalDistance.getValue() * dir;

  return value;
} // rotYWheelMotion()

// ************************************************************************
