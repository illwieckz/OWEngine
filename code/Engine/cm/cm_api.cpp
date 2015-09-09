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
//  File name:   cm_api.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: CM API implementation
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "cm_local.h"
#include <qcommon/q_shared.h>
#include <api/iFaceMgrAPI.h>
#include <api/vfsAPI.h>
#include <api/cvarAPI.h>
#include <api/coreAPI.h>
#include <api/cmAPI.h>
#include <api/modelLoaderDLLAPI.h>
#include <api/moduleManagerAPI.h>

class cmAPIImpl_c : public cmAPI_i
{
		// capsule for bullet character controller
		virtual class cmCapsule_i* registerCapsule( float height, float radius )
		{
				return CM_RegisterCapsule( height, radius );
		}
		// simple box for basic items / physics testing
		virtual class cmBBExts_i* registerBoxExts( float halfSizeX, float halfSizeY, float halfSizeZ )
		{
				return CM_RegisterBoxExts( halfSizeX, halfSizeY, halfSizeZ );
		}
		virtual class cmBBMinsMaxs_i* registerAABB( const class aabb& bb )
		{
				return CM_RegisterAABB( bb );
		}
		virtual class cmHull_i* registerHull( const char* modName, const vec3_c* points, u32 numPoints )
		{
				return CM_RegisterHull( modName, points, numPoints );
		}
		
		virtual class cMod_i* registerModel( const char* modName )
		{
				return CM_RegisterModel( modName );
		}
		virtual class cmSkelModel_i* registerSkelModel( const char* skelModelName )
		{
				return CM_RegisterSkelModel( skelModelName );
		}
		virtual class cMod_i* findModel( const char* modName )
		{
				return CM_FindModelInternal( modName );
		}
		virtual void freeAllModels()
		{
			CM_FreeAllModels();
		}
		
		virtual void loadMap( const char* mapName )
		{
			CM_LoadWorldMap( mapName );
		}
		virtual bool traceWorldRay( class trace_c& tr )
		{
			return CM_TraceWorldRay( tr );
		}
		virtual bool traceWorldSphere( class trace_c& tr )
		{
			return CM_TraceWorldSphere( tr );
		}
		virtual bool traceWorldAABB( class trace_c& tr )
		{
			return CM_TraceWorldAABB( tr );
		}
	public:
		cmAPIImpl_c()
		{
		
		}
		~cmAPIImpl_c()
		{
			this->freeAllModels();
		}
};

// interface manager (import)
class iFaceMgrAPI_i* g_iFaceMan = 0;
// imports
vfsAPI_s* g_vfs = 0;
cvarsAPI_s* g_cvars = 0;
coreAPI_s* g_core = 0;
modelLoaderDLLAPI_i* g_modelLoader = 0;
moduleManagerAPI_i* g_moduleMgr = 0;

// exports
static cmAPIImpl_c g_staticCMAPI;
cmAPI_i* cm = &g_staticCMAPI;

void ShareAPIs( iFaceMgrAPI_i* iFMA )
{
	g_iFaceMan = iFMA;
	
	// exports
	g_iFaceMan->registerInterface( ( iFaceBase_i* )( void* )cm, CM_API_IDENTSTR );
	
	// imports
	g_iFaceMan->registerIFaceUser( &g_vfs, VFS_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_cvars, CVARS_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_core, CORE_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_moduleMgr, MODULEMANAGER_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_modelLoader, MODELLOADERDLL_API_IDENTSTR );
	
	// TODO: put it in CM_Init()
	CM_AddConsoleCommands();
}

qioModule_e IFM_GetCurModule()
{
	return QMD_CM;
}

