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
  \class SoAnyPlaneViewer SoAnyPlaneViewer.h
  \brief The SoAnyPlaneViewer class is a collection of common functions for
  all the PlaneViewer components for Coin.
*/

#include <Inventor/nodes/SoCamera.h>

#include "SoAnyPlaneViewer.h"

// ************************************************************************

/*!
*/

float
SoAnyPlaneViewer::transYWheelMotion( // static
  const float value,
  const float oldvalue,
  SoCamera * const camera )
{
  // FIXME: GLX aspect ratio needs to be factored in
  assert( camera != NULL );
  SbVec3f dir;
  camera->orientation.getValue().multVec( SbVec3f( 0, 1, 0 ), dir );
  camera->position = camera->position.getValue() + (dir * (value - oldvalue)) *
    camera->focalDistance.getValue() / 2.5f;
  return value;
} // transYWheelMotion()

/*!
*/

float
SoAnyPlaneViewer::transXWheelMotion( // static
  const float value,
  const float oldvalue,
  SoCamera * const camera )
{
  // FIXME: GLX aspect ratio needs to be factored in
  assert( camera != NULL );
  SbVec3f dir;
  camera->orientation.getValue().multVec( SbVec3f( 1, 0, 0 ), dir );
  camera->position = camera->position.getValue() + (dir * (oldvalue - value)) *
    camera->focalDistance.getValue() / 2.5f;
  return value;
} // transXWheelMotion()

// ************************************************************************

/*!
*/

void
SoAnyPlaneViewer::viewPlaneX( // static
  SoCamera * const camera )
{
  assert( camera != NULL );
  SbVec3f dir;
  camera->orientation.getValue().multVec( SbVec3f( 0, 0, -1 ), dir );
  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * dir;
  camera->position = focalpoint +
    camera->focalDistance.getValue() * SbVec3f( 1, 0, 0 );
  camera->orientation = SbRotation( SbVec3f( 0, 1, 0 ), SB_PI/2.0f );
} // viewPlaneX()

/*!
*/

void
SoAnyPlaneViewer::viewPlaneY( // static
  SoCamera * const camera )
{
  assert( camera != NULL );
  SbVec3f dir;
  camera->orientation.getValue().multVec( SbVec3f( 0, 0, -1 ), dir );
  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * dir;
  camera->position = focalpoint +
    camera->focalDistance.getValue() * SbVec3f( 0, 1, 0 );
  camera->orientation = SbRotation( SbVec3f( 1, 0, 0 ), -SB_PI/2.0f );
} // viewPlaneY()

/*!
*/

void
SoAnyPlaneViewer::viewPlaneZ( // static
  SoCamera * const camera )
{
  assert( camera != NULL );
  SbVec3f dir;
  camera->orientation.getValue().multVec( SbVec3f( 0, 0, -1 ), dir );
  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * dir;
  camera->position = focalpoint +
    camera->focalDistance.getValue() * SbVec3f( 0, 0, 1 );
  camera->orientation = SbRotation( SbVec3f( 0, 1, 0 ), 0 );
} // viewPlaneZ()

// ************************************************************************
