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
//  File name:   rf_world.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: fuctions used for all world map types
//               (.BSP,.MAP, .PROC ....)
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __RF_WORLD_H__
#define __RF_WORLD_H__

#include <shared/typedefs.h>
#include <shared/array.h>

bool RF_LoadWorldMap( const char* name );
void RF_ClearWorldMap();
bool RF_IsAnyMapLoaded();
const char* RF_GetWorldMapName();
void RF_AddWorldDrawCalls();
bool RF_RayTraceWorld( class trace_c& tr );
void RF_SetWorldAreaBits( const byte* bytes, u32 numBytes );
int RF_AddWorldMapDecal( const class vec3_c& pos, const vec3_c& normal, float radius, class mtrAPI_i* material );
void RF_CacheLightWorldInteractions( class rLightImpl_c* l );
void RF_DrawSingleBSPSurface( u32 sfNum );
const class rIndexBuffer_c* RF_GetSingleBSPSurfaceABSIndices( u32 sfNum );
u32 RF_GetSingleBSPSurfaceTrianglesCount( u32 sfNum );
const class aabb& RF_GetSingleBSPSurfaceBounds( u32 sfNum );
class mtrAPI_i* RF_GetSingleBSPSurfaceMaterial( u32 sfNum );
const class rVertexBuffer_c* RF_GetBSPVertices();
const class r_model_c* RF_GetWorldModel();
void RF_AddBSPSurfaceToShadowVolume( u32 sfNum, const vec3_c& light, class rIndexedShadowVolume_c* staticShadowVolume, float lightRadius );
bool RF_IsWorldTypeProc();
bool RF_IsWorldAreaVisible( int areaNum );
int RF_GetNumAreas();
u32 RF_BoxAreas( const class aabb& absBB, arraySTD_c<u32>& out );
bool RF_CullBoundsByPortals( const aabb& absBB );
// it will work faster if you have touching areas precached
bool RF_CullBoundsByPortals( const aabb& absBB, const arraySTD_c<u32>& areaNums );
void RF_WorldDebugDrawing();
const class lightGridAPI_i* RF_GetWorldLightGridAPI();
bool RF_SampleWorldLightGrid( const vec3_c& point, struct pointLightSample_s& out );
void RF_GetLookatSurfaceInfo( struct rendererSurfaceRef_s& out );
void RF_SetWorldSurfaceMaterial( int areaNum, int surfaceNum, const char* matName );
void RF_GetWorldBounds( class aabb& out );
void RF_GetSunWorldBounds( class aabb& out );

#endif // __RF_BSP_H__
