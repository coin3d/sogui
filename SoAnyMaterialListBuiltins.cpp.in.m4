/**************************************************************************
 *
 *  This file is part of the Coin GUI binding libraries.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  The libraries this file is part of is free software; you can
 *  redistribute them and/or modify them under the terms of the GNU
 *  Lesser General Public License version 2.1 as published by the
 *  Free Software Foundation.  See the file LICENSE.LGPL at the root
 *  directory of the distribution for all the details.
 *
 *  If you want to use the Coin GUI binding libraries for applications
 *  not compatible with the LGPL, contact SIM about acquiring a
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// @configure_input@

#if SO@GUI@_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SO@GUI@_DEBUG

/**************************************************************************
 * IMPORTANT NOTICE
 * Be aware that the file So@Gui@MaterialListBuiltins.cpp is generated from
 * the file So@Gui@MaterialListBuiltins.cpp.m4, so changes has to be done in
 * the source file with the m4 macros.  The generation is done manually,
 * since it is no point in making this project depend on the user having
 * m4 installed.
 **************************************************************************/

#include <assert.h>
#include <string.h>

#include <Inventor/@Gui@/SoAnyMaterialList.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef WITH_STATIC_DEFAULTS
#include <materials/materials.h>
#endif // WITH_STATIC_DEFAULTS

include(libm4.m4)
divert(0)dnl
// *************************************************************************
define([SOANY_BUILTIN_MATERIAL_GROUPS], [13])dnl
define([SOANY_BUILTIN_MATERIAL_GROUP_SIZE], [35])dnl

#ifdef WITH_STATIC_DEFAULTS
static
const char *
changequote(,)dnl
builtin_material_names[] = {
changequote([,])dnl
define([matnum], 0)dnl
m4_foreach([material], [artdeco, autumn, glass, metal, neon, rococo, santafe,
                        sheen, silky, spring, summer, tropical, winter], [dnl
  "material",
m4_for([num], 0, m4_eval(SOANY_BUILTIN_MATERIAL_GROUP_SIZE-1),,
[dnl
    "material.num",
])dnl
define([matnum], m4_eval(matnum+1))dnl
])dnl
  NULL
}; // builtin_material_names
#endif // WITH_STATIC_DEFAULTS

// *************************************************************************

/*!
  \internal

  This method fills in the So@Gui@MaterialDirectory structure with the builtin
  material data.
*/

void
SoAnyMaterialList::setupBuiltinMaterials( // private
  So@Gui@MaterialDirectory * const index ) const
{
  assert( index != NULL );
  index->numGroups = 0;
  index->groups = NULL;
#ifdef WITH_STATIC_DEFAULTS
changequote(,)dnl
  index->flags |= SO@GUI@_BUILTIN_MATERIALS;
  index->numGroups = SOANY_BUILTIN_MATERIAL_GROUPS;
  index->groups = new So@Gui@MaterialGroup * [ SOANY_BUILTIN_MATERIAL_GROUPS ];
  for ( int i = 0; i < index->numGroups; i++ ) {
    index->groups[i] = new So@Gui@MaterialGroup;
    index->groups[i]->numMaterials = SOANY_BUILTIN_MATERIAL_GROUP_SIZE;
    index->groups[i]->materials = new So@Gui@Material * [ SOANY_BUILTIN_MATERIAL_GROUP_SIZE ];
    for ( int j = 0; j < SOANY_BUILTIN_MATERIAL_GROUP_SIZE; j++ ) {
      index->groups[i]->materials[j] = new So@Gui@Material;
    }
  }

changequote([,])dnl
define([matnum], 0)dnl
m4_foreach([material], [artdeco, autumn, glass, metal, neon, rococo, santafe,
                        sheen, silky, spring, summer, tropical, winter], [dnl
changequote(<,>)dnl
  index->groups[matnum]->name = builtin_material_names[m4_eval(matnum*(SOANY_BUILTIN_MATERIAL_GROUP_SIZE+1))];
changequote([,])dnl
m4_for([num], 0, m4_eval(SOANY_BUILTIN_MATERIAL_GROUP_SIZE-1),,
[changequote(<,>)dnl
  index->groups[matnum]->materials[num]->name = builtin_material_names[m4_eval(matnum*(SOANY_BUILTIN_MATERIAL_GROUP_SIZE+1)+num+1)];
  index->groups[matnum]->materials[num]->data = material<>num<>_iv;
changequote([,])dnl
])dnl
define([matnum], m4_eval(matnum+1))dnl
])dnl
#endif // ! WITH_STATIC_DEFAULTS
} // setupBuiltinMaterials()

// *************************************************************************

#if SO@GUI@_DEBUG
static const char * getSoAnyMaterialListBuiltinsRCSId(void) { return rcsid; }
#endif // SO@GUI@_DEBUG

