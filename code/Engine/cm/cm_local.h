////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2012 V.
//  Copyright (C) 2015 Dusan Jocic <dusanjocic@msn.com>
// 
//  OWEngine source code is free software; you can redistribute it
//  and/or modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//  
//  OWEngine source code is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA,
//  or simply visit <http://www.gnu.org/licenses/>.
// -------------------------------------------------------------------------
//  File name:   cm_local.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __CM_LOCAL_H__
#define __CM_LOCAL_H__

#include <shared/typedefs.h>

// cm_model.cpp
class cMod_i* CM_FindModelInternal( const char* name );
class cmCapsule_i* CM_RegisterCapsule( float height, float radius );
class cmBBExts_i* CM_RegisterBoxExts( float halfSizeX, float halfSizeY, float halfSizeZ );
class cmHull_i* CM_RegisterHull( const char* modName, const class vec3_c* points, u32 numPoints );
class cmBBMinsMaxs_i* CM_RegisterAABB( const class aabb& bb );
class cMod_i* CM_RegisterModel( const char* modName );
class cmSkelModel_i* CM_RegisterSkelModel( const char* skelModelName );
void CM_AddCObjectBaseToHashTable( class cmObjectBase_c* newCMObject );
void CM_FreeAllModels();

// cm_modelLoaderWrapper.cpp
bool CM_LoadRenderModelToSingleSurface( const char* rModelName, class cmSurface_c& out ); // returns true if model loading fails

// cm_cmds.cpp
void CM_AddConsoleCommands();
void CM_RemoveConsoleCommands();

// cm_inlineBSPModel.cpp
// load any inline model from any bsp file
class cMod_i* CM_LoadBSPFileSubModel( const char* bspFileName, u32 subModelNumber );

// cm_world.cpp
bool CM_LoadWorldMap( const char* mapName );
cMod_i* CM_GetWorldModel();
cMod_i* CM_GetWorldSubModel( unsigned int subModelIndex );
bool CM_TraceWorldRay( class trace_c& tr );
bool CM_TraceWorldSphere( class trace_c& tr );
bool CM_TraceWorldAABB( class trace_c& tr );

#endif // __CM_LOCAL_H__
