/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef SOGUI_DEFS_H
#define SOGUI_DEFS_H

#ifdef __FILE__
#define SOGUI_STUB_FILE __FILE__
#else
#define SOGUI_STUB_FILE ((const char *)0L)
#endif

#ifdef __LINE__
#define SOGUI_STUB_LINE __LINE__
#else
#define SOGUI_STUB_LINE 0
#endif

#ifdef __PRETTY_FUNCTION__
#define SOGUI_STUB_FUNC __PRETTY_FUNCTION__
#else
#define SOGUI_STUB_FUNC ((const char *)0L)
#endif

// SOGUI_STUB(): this is the method which prints out stub
// information. Used where there is functionality missing.

#include <stdio.h> // fprintf()

#define SOGUI_STUB() \
  do { \
    (void)fprintf(stderr, "STUB: functionality not yet completed"); \
    if (SOGUI_STUB_FILE) { \
      (void)fprintf(stderr, " at %s", SOGUI_STUB_FILE); \
      if (SOGUI_STUB_LINE > 0) (void)fprintf(stderr, ":line %u", SOGUI_STUB_LINE); \
      if (SOGUI_STUB_FUNC) (void)fprintf(stderr, ":[%s]", SOGUI_STUB_FUNC); \
    } \
    (void)fprintf(stderr, "\n"); \
  } while (0)


#endif // !SOGUI_DEFS_H
