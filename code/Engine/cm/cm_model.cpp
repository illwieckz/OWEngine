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
//  File name:   cm_model.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "cm_local.h"
#include "cm_model.h"
#include "cm_helper.h"
#include <api/coreAPI.h>
#include <api/cvarAPI.h>
#include <shared/hashTableTemplate.h>

static hashTableTemplateExt_c<cmObjectBase_c> cm_models;

void CM_AddCObjectBaseToHashTable( cmObjectBase_c* newCMObject )
{
	cm_models.addObject( newCMObject );
}

cMod_i* CM_FindModelInternal( const char* name )
{
	cmObjectBase_c* b = cm_models.getEntry( name );
	cMod_i* ret = dynamic_cast<cMod_i*>( b );
	return ret;
}
void CM_FormatCapsuleModelName( str& out, float h, float r )
{
	out = va( "_c%f_%f", h, r );
}
void CM_FormatBBExtsModelName( str& out, const float* halfSizes )
{
	out = va( "_bhe%f_%f_%f", halfSizes[0], halfSizes[1], halfSizes[2] );
}
void CM_FormatBBMinsMaxsModelName( str& out, const aabb& bb )
{
	out = va( "_bmx%f_%f_%f", bb.mins.x, bb.mins.y, bb.mins.x, bb.maxs.x, bb.maxs.y, bb.maxs.z );
}
cmCapsule_i* CM_RegisterCapsule( float height, float radius )
{
	str modName;
	CM_FormatCapsuleModelName( modName, height, radius );
	cMod_i* existing = CM_FindModelInternal( modName );
	if ( existing )
	{
		if ( existing->getType() != CMOD_CAPSULE )
		{
			g_core->DropError( "CM_RegisterCapsule: found non-capsule model using capsules name syntax" );
			return 0;
		}
		return ( cmCapsule_c* )existing;
	}
	cmCapsule_c* n = new cmCapsule_c( modName, height, radius );
	cm_models.addObject( n );
	return n;
}
class cmBBExts_i* CM_RegisterBoxExts( float halfSizeX, float halfSizeY, float halfSizeZ )
{
		str modName;
		CM_FormatBBExtsModelName( modName, vec3_c( halfSizeX, halfSizeY, halfSizeZ ) );
		cMod_i* existing = CM_FindModelInternal( modName );
		if ( existing )
		{
			if ( existing->getType() != CMOD_BBEXTS )
			{
				g_core->DropError( "CM_RegisterBoxExts: found non-bbexts model using bbexts name syntax" );
				return 0;
			}
			return ( cmBBExts_i* )existing;
		}
		cmBBExts_c* n = new cmBBExts_c( modName, vec3_c( halfSizeX, halfSizeY, halfSizeZ ) );
		cm_models.addObject( n );
		return n;
}
class cmHull_i* CM_RegisterHull( const char* modName, const vec3_c* points, u32 numPoints )
{
		cMod_i* ex = CM_FindModelInternal( modName );
		if ( ex )
			return dynamic_cast<cmHull_i*>( ex );
		cmHull_c* newModel = new cmHull_c;
		newModel->setName( modName );
		newModel->createFromPoints( points, numPoints );
		cm_models.addObject( newModel );
		return newModel;
}
class cmBBMinsMaxs_i* CM_RegisterAABB( const class aabb& bb )
{
		str modName;
		CM_FormatBBMinsMaxsModelName( modName, bb );
		cMod_i* existing = CM_FindModelInternal( modName );
		if ( existing )
		{
			if ( existing->getType() != CMOD_BBMINSMAXS )
			{
				g_core->DropError( "CM_RegisterAABB: found non-bbminsmaxs model using CMOD_BBMINSMAXS name syntax" );
				return 0;
			}
			return ( cmBBMinsMaxs_i* )existing;
		}
		cmBBMinsMaxs_c* n = new cmBBMinsMaxs_c( modName, bb );
		cm_models.addObject( n );
		return n;
}
#include <shared/parser.h>
#include <shared/ePairsList.h>
#include <shared/cmBrush.h>
#include <shared/cmBezierPatch.h>
#include <shared/mapBezierPatch.h>
struct cmMapFileEntity_s
{
	ePairList_c ePairs;
	arraySTD_c<cmBrush_c*> brushes;
	arraySTD_c<cmSurface_c*> bezierPatches;
	
	cmHelper_c* allocHelper()
	{
		cmHelper_c* ret = new cmHelper_c;
		ret->setKeyPairs( ePairs );
		return ret;
	}
	
	~cmMapFileEntity_s()
	{
		for ( u32 i = 0; i < brushes.size(); i++ )
		{
			delete brushes[i];
		}
		for ( u32 i = 0; i < bezierPatches.size(); i++ )
		{
			if ( bezierPatches[i] )
			{
				delete bezierPatches[i];
			}
		}
	}
};
class cMod_i* CM_LoadModelFromMapFile( const char* fname )
{
		parser_c p;
		if ( p.openFile( fname ) )
		{
			g_core->RedWarning( "CM_LoadModelFromMapFile: cannot open %s\n", fname );
			return 0;
		}
		bool parseError = false;
		u32 numEntitiesWithBrushes = 0;
		u32 numTotalBrushes = 0;
		u32 numTotalPatches = 0;
		cmBrush_c* firstBrush = 0;
		arraySTD_c<cmMapFileEntity_s*> entities;
		
		if ( p.atWord( "version" ) )
		{
			p.getToken(); // skip doom3/quake4 version ident
		}
		while ( p.atEOF() == false && parseError == false )
		{
			if ( p.atWord( "{" ) )
			{
				// enter new entity
				cmMapFileEntity_s* ne = new cmMapFileEntity_s;
				entities.push_back( ne );
				while ( p.atWord( "}" ) == false && parseError == false )
				{
					if ( p.atEOF() )
					{
						g_core->RedWarning( "CM_LoadModelFromMapFile: unexpected end of file hit while parsing %s\n", fname );
						break;
					}
					if ( p.atWord( "{" ) )
					{
						// enter new primitive
						cmBrush_c* nb = 0;
						p.skipToNextToken();
						const char* primTypePos = p.getCurDataPtr();
						if ( p.atWord( "brushDef3" ) )
						{
							nb = new cmBrush_c;
							if ( nb->parseBrushD3( p ) )
							{
								g_core->RedWarning( "CM_LoadModelFromMapFile: error while parsing brushDef3 at line %i of %s\n", p.getCurrentLineNumber(), fname );
								parseError = true;
								delete nb;
								break;
							}
						}
						else if ( p.atWord( "patchDef2" ) || p.atWord( "patchDef3" ) )
						{
#if 0
							if ( p.skipCurlyBracedBlock() )
							{
								g_core->RedWarning( "CM_LoadModelFromMapFile: error while parsing patchDef2 at line %i of %s\n", p.getCurrentLineNumber(), fname );
								parseError = true;
								break;
							}
#else
							p.setCurDataPtr( primTypePos );
							numTotalPatches++;
							mapBezierPatch_c bp;
							if ( bp.parse( p ) )
							{
								g_core->RedWarning( "CM_LoadModelFromMapFile: error while parsing patchDef* at line %i of %s\n", p.getCurrentLineNumber(), fname );
								parseError = true;
								break;
							}
							cmSurface_c* triData = new cmSurface_c;
#if 0
							// do the fastest, low-detail bezier patch tesselation
							bp.getLowestDetailCMSurface( triData );
#else
							// it seems that low-detail bezier patch tesselation is not enough.
							cmBezierPatch_c cmBP;
							cmBP.fromMapBezierPatch( &bp );
							cmBP.tesselate( 3, triData );
#endif
							ne->bezierPatches.push_back( triData );
#endif
							if ( p.atWord( "}" ) == false )
							{
								g_core->RedWarning( "MOD_LoadConvertMapFileToStaticTriMesh: error while parsing patchDef2 at line %i of %s\n", p.getCurrentLineNumber(), fname );
								parseError = true;
								break;
							}
						}
						else
						{
							nb = new cmBrush_c;
							if ( nb->parseBrushQ3( p ) )
							{
								g_core->RedWarning( "CM_LoadModelFromMapFile: error while parsing old brush format at line %i of %s\n", p.getCurrentLineNumber(), fname );
								parseError = true;
								delete nb;
								break;
							}
						}
						if ( nb )
						{
							if ( nb->hasSideWithMaterial( "textures/editor/visportal" ) )
							{
								// temporary hack needed to remove blocking visportal brushes
								delete nb;
							}
							else
							{
								ne->brushes.push_back( nb );
								nb->calcBounds();
								if ( firstBrush == 0 )
								{
									firstBrush = nb;
								}
							}
						}
					}
					else
					{
						// parse key pair
						str key, val;
						p.getToken( key );
						p.getToken( val );
						ne->ePairs.set( key, val );
					}
				}
				// current entity parsing done
				if ( ne->brushes.size() )
				{
					numEntitiesWithBrushes++;
					numTotalBrushes += ne->brushes.size();
				}
				else
				{
				}
			}
			else
			{
				int line = p.getCurrentLineNumber();
				str token = p.getToken();
				g_core->RedWarning( "CM_LoadModelFromMapFile: unknown token %s at line %i of %s\n", token.c_str(), line, fname );
				parseError = true;
			}
		}
		cMod_i* ret = 0;
		// see if we can simplify the cModel
		if ( numTotalBrushes == 1 && numTotalPatches == 0 )
		{
			cmHull_c* hull = new cmHull_c( fname, *firstBrush );
			cm_models.addObject( hull );
			
			ret = hull;
			// convert the rest of map entities to cmHelpers
			for ( u32 i = 0; i < entities.size(); i++ )
			{
				cmMapFileEntity_s* e = entities[i];
				if ( e->brushes.size() )
					continue;
				cmHelper_c* helper = e->allocHelper();
				hull->addHelper( helper );
			}
		}
		else
		{
			cmCompound_c* compound = new cmCompound_c( fname );
			cm_models.addObject( compound );
			
			ret = compound;
			// add main brushes
			cmMapFileEntity_s* e = entities[0];
			for ( u32 j = 0; j < e->brushes.size(); j++ )
			{
				cmHull_c* hull = new cmHull_c( va( "%s::subBrush%i", fname, j ), *e->brushes[j] );
				compound->addShape( hull );
			}
			for ( u32 j = 0; j < e->bezierPatches.size(); j++ )
			{
				cmTriMesh_c* triMesh = new cmTriMesh_c( va( "%s::subBezierPatch%i", fname, j ), e->bezierPatches[j] );
				e->bezierPatches[j] = 0;
				compound->addShape( triMesh );
			}
			// add helpers and their brushes
			for ( u32 i = 1; i < entities.size(); i++ )
			{
				// create a cmHelper_c for subentity
				cmMapFileEntity_s* e = entities[i];
				cmHelper_c* helper = e->allocHelper();
				if ( e->brushes.size() )
				{
					// if subentity has geometry, create a compound group for helper
					cmCompound_c* subEntityCompoundGroup = helper->registerCompound();
					subEntityCompoundGroup->setName( va( "%s::%i", fname, i ) );
					for ( u32 j = 0; j < e->brushes.size(); j++ )
					{
						cmHull_c* hull = new cmHull_c( va( "%s::subBrush%i", fname, j ), *e->brushes[j] );
						subEntityCompoundGroup->addShape( hull );
					}
				}
				compound->addHelper( helper );
			}
		}
		
		for ( u32 i = 0; i < entities.size(); i++ )
		{
			delete entities[i];
		}
		return ret;
}
#include <api/modelLoaderDLLAPI.h>]
class cmSkelModel_i* CM_RegisterSkelModel( const char* skelModelName )
{
		cMod_i* existing = CM_FindModelInternal( skelModelName );
		if ( existing )
			return existing->getSkelModel();
		skelModelAPI_i* skel = g_modelLoader->loadSkelModelFile( skelModelName );
		if ( skel )
		{
			cmSkelModel_c* skelModel = new cmSkelModel_c( skelModelName, skel );
			cm_models.addObject( skelModel );
			return skelModel;
		}
		return 0;
}
#if 1
#define SWAP_TRIMESH_INDEXES
#endif
class cMod_i* CM_RegisterModel( const char* modName )
{
		cMod_i* existing = CM_FindModelInternal( modName );
		if ( existing )
		{
			return existing;
		}
		// check for submodels override (this is dirty...)
		if ( modName[0] == '*' )
		{
			unsigned int subModelIndex = atoi( modName + 1 ) - 1;
			existing = CM_GetWorldSubModel( subModelIndex );
			if ( existing )
				return existing;
		}
		// check for primitive models (procedurally generated)
		if ( modName[0] == '_' )
		{
			const char* t = modName + 1;
			if ( !Q_stricmpn( t, "c", 1 ) )
			{
				// that's a capsule
				float radius, height;
				sscanf( modName, "_c%f_%f", &height, &radius );
				return CM_RegisterCapsule( height, radius );
			}
			else if ( !Q_stricmpn( t, "bhe", 3 ) )
			{
				// that's a bb defined by halfsizes
				vec3_c halfSizes;
				sscanf( modName, "_bhe%f_%f_%f", &halfSizes.x, &halfSizes.y, &halfSizes.z );
				return CM_RegisterBoxExts( halfSizes.x, halfSizes.y, halfSizes.z );
			}
			else
			{
				// it might be a procedural static model
				if ( g_modelLoader->isStaticModelFile( modName ) )
				{
					cmSurface_c* sf = new cmSurface_c;
					if ( CM_LoadRenderModelToSingleSurface( modName, *sf ) == false )
					{
						// swap triangle indexes for Bullet
#ifdef SWAP_TRIMESH_INDEXES
						sf->swapIndexes();
#endif // SWAP_TRIMESH_INDEXES
						cmTriMesh_c* triMesh = new cmTriMesh_c( modName, sf );
						cm_models.addObject( triMesh );
						return triMesh;
					}
					delete sf;
				}
				else
				{
					return 0;
				}
			}
		}
		// check for .BSP inline models
		// (FIXME: they all should be precached once on map startup??)
		if ( modName[0] == '*' )
		{
			u32 modelNumber = atoi( modName + 1 );
			char mapName[256];
			g_cvars->Cvar_VariableStringBuffer( "mapname", mapName, sizeof( mapName ) );
			return CM_LoadBSPFileSubModel( mapName, modelNumber );
		}
		// check if modName is a fileName
		const char* ext = G_strgetExt( modName );
		if ( ext )
		{
			if ( !stricmp( ext, "map" ) )
			{
				return CM_LoadModelFromMapFile( modName );
			}
			else if ( g_modelLoader->isStaticModelFile( modName ) )
			{
				cmSurface_c* sf = new cmSurface_c;
				if ( CM_LoadRenderModelToSingleSurface( modName, *sf ) == false )
				{
					// swap triangle indexes for Bullet
#ifdef SWAP_TRIMESH_INDEXES
					sf->swapIndexes();
#endif // SWAP_TRIMESH_INDEXES
					cmTriMesh_c* triMesh = new cmTriMesh_c( modName, sf );
					cm_models.addObject( triMesh );
					return triMesh;
				}
				delete sf;
			}
			else if ( g_modelLoader->isSkelModelFile( modName ) )
			{
				return CM_RegisterSkelModel( modName );
			}
		}
		return 0;
}
void CM_FreeAllModels()
{
	for ( u32 i = 0; i < cm_models.size(); i++ )
	{
		delete cm_models[i];
	}
	cm_models.clear();
}
