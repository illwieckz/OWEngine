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
//  File name:   backEndGLAPI.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include <qcommon/q_shared.h>
#include <api/iFaceMgrAPI.h>
#include <api/vfsAPI.h>
#include <api/cvarAPI.h>
#include <api/coreAPI.h>
#include <api/inputSystemAPI.h>
#include <api/sdlSharedAPI.h>
#include <api/rbAPI.h>

// it's needed here only for Doom3 .mtr tables access
#include <api/materialSystemAPI.h>
#include <api/rAPI.h>

// for debug image output
#include <api/imgAPI.h>

// interface manager (import)
class iFaceMgrAPI_i* g_iFaceMan = 0;
// imports
vfsAPI_s* g_vfs = 0;
cvarsAPI_s* g_cvars = 0;
coreAPI_s* g_core = 0;
inputSystemAPI_i* g_inputSystem = 0;
sdlSharedAPI_i* g_sharedSDLAPI = 0;
class rbAPI_i* rb = 0;
materialSystemAPI_i* g_ms = 0;
rAPI_i* rf = 0;
imgAPI_i* g_img = 0;

void SDLOpenGL_RegisterBackEnd();

void ShareAPIs( iFaceMgrAPI_i* iFMA )
{
	g_iFaceMan = iFMA;
	
	// exports
	SDLOpenGL_RegisterBackEnd();
	
	// imports
	g_iFaceMan->registerIFaceUser( &g_vfs, VFS_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_cvars, CVARS_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_core, CORE_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_inputSystem, INPUT_SYSTEM_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_sharedSDLAPI, SHARED_SDL_API_IDENTSTRING );
	g_iFaceMan->registerIFaceUser( &rb, RENDERER_BACKEND_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_ms, MATERIALSYSTEM_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &rf, RENDERER_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_img, IMG_API_IDENTSTR );
}

qioModule_e IFM_GetCurModule()
{
	return QMD_REF_BACKEND_GL;
}

