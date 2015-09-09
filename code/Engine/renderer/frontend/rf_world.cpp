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
//  File name:   rf_world.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Fuctions used for all world map types
//               (.BSP, .MAP, .PROC ...)
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "rf_bsp.h"
#include "rf_surface.h"
#include "rf_proc.h"
#include "rf_local.h"
#include "rf_lightGrid.h"
#include <api/coreAPI.h>
#include <api/modelLoaderDLLAPI.h>
#include <api/materialSystemAPI.h>
#include <api/rEntityAPI.h>
#include <shared/autoCmd.h>
#include <shared/autoCvar.h>
#include <shared/trace.h>
#include <shared/stringList.h>
#include <shared/rendererSurfaceRef.h>

static str r_worldMapName;
static class rBspTree_c* r_bspTree = 0; // for .bsp files
static class r_model_c* r_worldModel = 0; // for .map files (converted to trimeshes) and other model types
static class procTree_c* r_procTree = 0; // for .proc files

static aCvar_c rf_printWorldRayCasts( "rf_printWorldRayCasts", "0" );
static aCvar_c rf_printWorldUpdates( "rf_printWorldUpdates", "0" );
static aCvar_c rf_world_dontAddDecals( "rf_world_dontAddDecals", "0" );
static aCvar_c rf_skipWorld( "rf_skipWorld", "0" );

void RF_ClearWorldMap()
{
	if ( r_bspTree )
	{
		delete r_bspTree;
		r_bspTree = 0;
	}
	if ( r_worldModel )
	{
		delete r_worldModel;
		r_worldModel = 0;
	}
	if ( r_procTree )
	{
		delete r_procTree;
		r_procTree = 0;
	}
	RFL_FreeAllLights();
}
bool RF_IsAnyMapLoaded()
{
	if ( r_bspTree )
		return true;
	if ( r_worldModel )
		return true;
	if ( r_procTree )
		return true;
	return false;
}
void RF_CreateEmptyMap()
{
	// create flat ground plane
	r_worldModel = new r_model_c;
	r_worldModel->setNumSurfs( 1 );
	const float maxSize = 1048576.f;
	const float texWorldSize = 64.f;
	r_worldModel->getSurf( 0 )->addQuad(
		rVert_c( vec2_c( 0, 1 ), vec3_c( -maxSize, maxSize, 0 ) ),
		rVert_c( vec2_c( 0, 0 ), vec3_c( -maxSize, -maxSize, 0 ) ),
		rVert_c( vec2_c( 1, 0 ), vec3_c( maxSize, -maxSize, 0 ) ),
		rVert_c( vec2_c( 1, 1 ), vec3_c( maxSize, maxSize, 0 ) )
	);
	r_worldModel->multTexCoordsXY( maxSize / texWorldSize );
	r_worldModel->recalcModelTBNs();
	r_worldModel->recalcBoundingBoxes();
	
	r_worldMapName = "_empty";
}
const char* RF_GetWorldMapName()
{
	return r_worldMapName.c_str();
}
bool RF_LoadWorldMap( const char* name )
{
	RF_ClearWorldMap();
	if ( !stricmp( name, "_empty" ) )
	{
		RF_CreateEmptyMap();
		return false;
	}
	const char* ext = G_strgetExt( name );
	if ( ext == 0 )
	{
		g_core->RedWarning( "RF_LoadWorldMap: %s has no extension\n", name );
		// create default empty map
		RF_CreateEmptyMap();
		return true;
	}
	r_worldMapName = "_nomap";
	if ( !stricmp( ext, "bsp" ) )
	{
		// Q3/RTCW/ET/MoH/CoD .bsp file
		r_bspTree = RF_LoadBSP( name );
		if ( r_bspTree )
		{
			r_worldMapName = name;
			return false; // ok
		}
		return true; // error
	}
	else if ( !stricmp( ext, "proc" ) )
	{
		// Doom3 / Quake4 .proc
		r_procTree = RF_LoadPROC( name );
		if ( r_procTree )
		{
			r_worldMapName = name;
			return false; // ok
		}
		return true; // error
	}
	else if ( !stricmp( ext, "procb" ) )
	{
		// ETQW binary .proc
		r_procTree = RF_LoadPROCB( name );
		if ( r_procTree )
		{
			r_worldMapName = name;
			return false; // ok
		}
		return true; // error
	}
	else if ( !stricmp( ext, "map" ) )
	{
		// load .map file directly
		r_worldModel = RF_LoadMAPFile( name );
		if ( r_worldModel )
		{
			r_worldMapName = name;
			return false; // ok
		}
		return true; // error
	}
	else if ( g_modelLoader->isStaticModelFile( name ) )
	{
		// any other static model format
		r_model_c* m = new r_model_c;
		if ( g_modelLoader->loadStaticModelFile( name, m ) )
		{
			delete m;
			return true; // error
		}
		r_worldMapName = name;
		m->createVBOsAndIBOs();
		r_worldModel = m;
		return false; // ok
	}
	g_core->RedWarning( "Cannot load worldmap %s\n", name );
	// create default empty map
	RF_CreateEmptyMap();
	return true; // error
}
void RF_AddWorldDrawCalls()
{
	if ( rf_skipWorld.getInt() )
		return;
	if ( r_bspTree )
	{
		if ( rf_printWorldUpdates.getInt() )
		{
			g_core->Print( "RF_AddWorldDrawCalls: adding r_bspTree drawCalls\n" );
		}
		r_bspTree->addDrawCalls();
	}
	if ( r_worldModel )
	{
		if ( rf_printWorldUpdates.getInt() )
		{
			g_core->Print( "RF_AddWorldDrawCalls: adding r_worldModel drawCalls\n" );
		}
		r_worldModel->addDrawCalls();
	}
	if ( r_procTree )
	{
		if ( rf_printWorldUpdates.getInt() )
		{
			g_core->Print( "RF_AddWorldDrawCalls: adding r_procTree drawCalls\n" );
		}
		r_procTree->addDrawCalls();
	}
}
bool RF_RayTraceWorld( class trace_c& tr )
{
	if ( rf_printWorldRayCasts.getInt() )
	{
		g_core->Print( "RF_RayTraceWorld: from %f %f %f to %f %f %f\n", tr.getStartPos().x, tr.getStartPos().y, tr.getStartPos().z,
					   tr.getTo().x, tr.getTo().y, tr.getTo().z );
	}
	if ( r_bspTree )
	{
		return r_bspTree->traceRay( tr );
	}
	if ( r_worldModel )
	{
		return r_worldModel->traceRay( tr );
	}
	if ( r_procTree )
	{
		return r_procTree->traceRay( tr );
	}
	return false;
}
void RF_SetWorldAreaBits( const byte* bytes, u32 numBytes )
{
	if ( r_bspTree )
	{
		r_bspTree->setWorldAreaBits( bytes, numBytes );
	}
}
int RF_AddWorldMapDecal( const vec3_c& pos, const vec3_c& normal, float radius, class mtrAPI_i* material )
{
	if ( rf_world_dontAddDecals.getInt() )
		return -1;
	if ( r_bspTree )
	{
		return r_bspTree->addWorldMapDecal( pos, normal, radius, material );
	}
	if ( r_procTree )
	{
		return r_procTree->addWorldMapDecal( pos, normal, radius, material );
	}
	if ( r_worldModel )
	{
		return r_worldModel->createDecal( RF_GetWorldDecalBatcher(), pos, normal, radius, material );
	}
	return -1;
}
void RF_CacheLightWorldInteractions( class rLightImpl_c* l )
{
	if ( r_bspTree )
	{
		r_bspTree->cacheLightWorldInteractions( l );
	}
	if ( r_procTree )
	{
		r_procTree->cacheLightWorldInteractions( l );
	}
	if ( r_worldModel )
	{
		// assumes that world model is a static model
		r_worldModel->cacheLightStaticModelInteractions( l );
	}
}
void RF_DrawSingleBSPSurface( u32 sfNum )
{
	if ( r_bspTree == 0 )
		return;
	r_bspTree->addBSPSurfaceDrawCall( sfNum );
}
const rIndexBuffer_c* RF_GetSingleBSPSurfaceABSIndices( u32 sfNum )
{
	if ( r_bspTree == 0 )
		return 0;
	return r_bspTree->getSingleBSPSurfaceABSIndices( sfNum );
}
u32 RF_GetSingleBSPSurfaceTrianglesCount( u32 sfNum )
{
	if ( r_bspTree == 0 )
		return 0;
	return r_bspTree->getSingleBSPSurfaceTrianglesCount( sfNum );
}
const class aabb& RF_GetSingleBSPSurfaceBounds( u32 sfNum )
{
		if ( r_bspTree == 0 )
			return aabb();
		return r_bspTree->getSingleBSPSurfaceBounds( sfNum );
}
class mtrAPI_i* RF_GetSingleBSPSurfaceMaterial( u32 sfNum )
{
		if ( r_bspTree == 0 )
			return 0;
		return r_bspTree->getSurfaceMaterial( sfNum );
}
const rVertexBuffer_c* RF_GetBSPVertices()
{
	if ( r_bspTree == 0 )
		return 0;
	return r_bspTree->getVertices();
}
const class r_model_c* RF_GetWorldModel()
{
		return r_worldModel;
}
void RF_AddBSPSurfaceToShadowVolume( u32 sfNum, const vec3_c& light, class rIndexedShadowVolume_c* staticShadowVolume, float lightRadius )
{
	r_bspTree->addBSPSurfaceToShadowVolume( sfNum, light, staticShadowVolume, lightRadius );
}

void RF_PrintWorldMapMaterials_f()
{
	stringList_c matNames;
	matNames.setIgnoreDuplicates( true );
	if ( r_bspTree )
	{
		r_bspTree->getReferencedMatNames( &matNames );
	}
	else if ( r_procTree )
	{
		r_procTree->getReferencedMatNames( &matNames );
	}
	for ( u32 i = 0; i < matNames.size(); i++ )
	{
		g_core->Print( "%i/%i : %s\n", i, matNames.size(), matNames.getString( i ) );
	}
	g_core->Print( "Total %i materials referenced by world surfaces\n", matNames.size() );
}
void RF_PrintWorldMapInfo_f()
{
	if ( r_bspTree )
	{
		g_core->Print( "World map type: BSP\n" );
	}
	else if ( r_procTree )
	{
		g_core->Print( "World map type: PROC\n" );
	}
	else if ( r_worldModel )
	{
		g_core->Print( "World map type: MODEL\n" );
	}
	else
	{
		g_core->RedWarning( "No world map...\n" );
	}
}
void RF_BSP_RebuildBatches_f()
{
	if ( r_bspTree )
	{
		r_bspTree->rebuildBatches();
	}
	else
	{
		g_core->RedWarning( "Map type is not BSP\n" );
	}
}
void RF_SetWorldSurfaceMaterial( int areaNum, int surfaceNum, const char* matName )
{
	if ( r_bspTree )
	{
		r_bspTree->setSurfaceMaterial( surfaceNum, matName );
	}
	else if ( r_worldModel )
	{
		r_worldModel->setSurfaceMaterial( surfaceNum, matName );
	}
	else if ( r_procTree )
	{
		r_procTree->setSurfaceMaterial( areaNum, surfaceNum, matName );
	}
}
void RF_GetWorldBounds( class aabb& out )
{
	if ( r_bspTree )
	{
		out = r_bspTree->getWorldBounds();
	}
	else if ( r_worldModel )
	{
		out = r_worldModel->getBounds();
	}
	else if ( r_procTree )
	{
		//r_procTree->getBB();
	}
}
void RF_GetSunWorldBounds( class aabb& out )
{
	if ( r_bspTree )
	{
		//out = r_bspTree->getWorldBounds();
		out = r_bspTree->getWorldBoundsWithoutSkyBox();
	}
	else if ( r_worldModel )
	{
		r_worldModel->getSunBounds( out );
	}
	else if ( r_procTree )
	{
		//r_procTree->getBB();
	}
	// fix for flickering shadows on the lowest floor of
	// "test_hipshot_sky_violentdays-4096x4096world" map.
	out.extend( 4.f );
}


void RF_SetWorldSurfaceMaterial_f()
{
	if ( g_core->Argc() < 4 )
	{
		g_core->Print( "Usage: RF_SetWorldSurfaceMaterial_f <areaNum> <surfaceNum> <material_name>\n" );
		return;
	}
	// areaNum is needed only for .proc world maps
	int areaNum = atoi( g_core->Argv( 1 ) );
	int surfaceNum = atoi( g_core->Argv( 2 ) );
	const char* matName = g_core->Argv( 3 );
	mtrAPI_i* mat = g_ms->registerMaterial( matName );
	if ( mat == 0 )
	{
		g_core->Print( "NULL material\n" );
		return;
	}
	RF_SetWorldSurfaceMaterial( areaNum, surfaceNum, matName );
}
void RF_GetLookatSurfaceInfo( rendererSurfaceRef_s& out )
{
	trace_c tr;
	RF_DoCameraTrace( tr, true );
	if ( tr.hasHit() == false )
	{
		out.clear();
		return;
	}
	out.areaNum = tr.getHitAreaNum();
	out.surfaceNum = tr.getHitSurfaceNum();
	class rEntityAPI_i* e = tr.getHitREntity();
	if ( e )
	{
		out.entityNum = e->getNetworkingEntityNumber();
	}
	else
	{
		out.entityNum = -1;
	}
}
void RF_SetCrosshairSurfaceMaterial_f()
{
	if ( g_core->Argc() < 2 )
	{
		g_core->Print( "Usage: rf_setCrosshairSurfaceMaterial <material_name>\n" );
		return;
	}
	const char* matName = g_core->Argv( 1 );
	mtrAPI_i* mat = g_ms->registerMaterial( matName );
	if ( mat == 0 )
	{
		g_core->Print( "NULL material\n" );
		return;
	}
	rendererSurfaceRef_s tmp;
	RF_GetLookatSurfaceInfo( tmp );
	RF_SetWorldSurfaceMaterial( tmp.areaNum, tmp.surfaceNum, matName );
}
bool RF_IsWorldTypeProc()
{
	if ( r_procTree )
		return true;
	return false;
}
bool RF_IsWorldAreaVisible( int areaNum )
{
	if ( r_procTree )
	{
		return r_procTree->isAreaVisibleByPlayer( areaNum );
	}
	return true;
}
int RF_GetNumAreas()
{
	if ( r_procTree )
	{
		return r_procTree->getNumAreas();
	}
	return 0;
}
u32 RF_BoxAreas( const aabb& absBB, arraySTD_c<u32>& out )
{
	if ( r_procTree )
	{
		return r_procTree->boxAreas( absBB, out );
	}
	return 0;
}
bool RF_CullBoundsByPortals( const aabb& absBB )
{
	if ( r_procTree )
	{
		return r_procTree->cullBoundsByPortals( absBB );
	}
	if ( r_bspTree )
	{
		// our own Qio BSP format has areaportals data
		// NOTE: it seems that areaPortals data is stored
		// in Call Of Duty BSPs as well, but I havent fully
		// reverse-enginered it yet...
		return r_bspTree->cullBoundsByPortals( absBB );
	}
	return false;
}
bool RF_CullBoundsByPortals( const aabb& absBB, const arraySTD_c<u32>& areaNums )
{
	if ( r_procTree )
	{
		return r_procTree->cullBoundsByPortals( absBB, areaNums );
	}
	if ( r_bspTree )
	{
		// TODO: use "areaNums" here
		return r_bspTree->cullBoundsByPortals( absBB );
	}
	return false;
}

void RF_WorldDebugDrawing()
{
	if ( r_bspTree )
	{
		r_bspTree->doDebugDrawing();
	}
	if ( r_procTree )
	{
		r_procTree->doDebugDrawing();
	}
}
const class lightGridAPI_i* RF_GetWorldLightGridAPI()
{
		if ( r_bspTree )
		{
			return r_bspTree->getLightGridAPI();
		}
		return 0;
}
bool RF_SampleWorldLightGrid( const vec3_c& point, struct pointLightSample_s& out )
{
	const lightGridAPI_i* grid = RF_GetWorldLightGridAPI();
	if ( grid == 0 )
		return true;
	grid->setupPointLighting( point, out );
	return false; // ok
}
static aCmd_c rf_printWorldMapMaterials( "printWorldMapMaterials", RF_PrintWorldMapMaterials_f );
static aCmd_c rf_printWorldMapInfo( "printWorldMapInfo", RF_PrintWorldMapInfo_f );
static aCmd_c rf_bsp_rebuildBatches( "rebuildBSPWorldBatches", RF_BSP_RebuildBatches_f );
static aCmd_c rf_setCrosshairSurfaceMaterial( "rf_setCrosshairSurfaceMaterial", RF_SetCrosshairSurfaceMaterial_f );
static aCmd_c rf_setWorldSurfaceMaterial( "rf_setWorldSurfaceMaterial", RF_SetWorldSurfaceMaterial_f );


