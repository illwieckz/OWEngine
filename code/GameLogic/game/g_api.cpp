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
//  File name:   g_api.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Game DLL entry point
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "g_local.h"
#include <api/iFaceMgrAPI.h>
#include <api/vfsAPI.h>
#include <api/serverAPI.h>
#include <api/cvarAPI.h>
#include <api/coreAPI.h>
#include <api/gameAPI.h>
#include <api/cmAPI.h>
#include <api/loadingScreenMgrAPI.h>
#include <api/rAPI.h>
#include <api/declManagerAPI.h>
#include <api/moduleManagerAPI.h>

// interface manager (import)
class iFaceMgrAPI_i *g_iFaceMan = 0;
// imports
svAPI_s *g_server = 0;
vfsAPI_s *g_vfs = 0;
cvarsAPI_s *g_cvars = 0;
coreAPI_s *g_core = 0;
cmAPI_i *cm = 0;
loadingScreenMgrAPI_i *g_loadingScreen = 0;
rAPI_i *rf = 0;
declManagerAPI_i *g_declMgr = 0;
moduleManagerAPI_i *g_moduleMgr = 0;
class modelLoaderDLLAPI_i *g_modelLoader = 0;
// exports
static gameAPI_s g_staticGameAPI;
static gameClientAPI_s g_staticGameClientsAPI;

void ShareAPIs(iFaceMgrAPI_i *iFMA) {
	g_iFaceMan = iFMA;

	// exports
	g_staticGameAPI.InitGame = G_InitGame;
	g_staticGameAPI.RunFrame = G_RunFrame;
	g_staticGameAPI.ShutdownGame = G_ShutdownGame;
	g_staticGameAPI.DebugDrawFrame = G_DebugDrawFrame;

	g_iFaceMan->registerInterface(&g_staticGameAPI,GAME_API_IDENTSTR);

	g_staticGameClientsAPI.ClientBegin = ClientBegin;
	g_staticGameClientsAPI.ClientCommand = ClientCommand;
	g_staticGameClientsAPI.ClientUserinfoChanged = ClientUserinfoChanged;
	g_staticGameClientsAPI.ClientConnect = ClientConnect;
	g_staticGameClientsAPI.ClientThink = ClientThink;
	g_staticGameClientsAPI.ClientDisconnect = ClientDisconnect;
	g_staticGameClientsAPI.ClientGetPlayerState = ClientGetPlayerState;
	g_iFaceMan->registerInterface(&g_staticGameClientsAPI,GAMECLIENTS_API_IDENTSTR);

	// imports
	g_iFaceMan->registerIFaceUser(&g_server,SERVER_API_IDENTSTR);
	g_iFaceMan->registerIFaceUser(&g_vfs,VFS_API_IDENTSTR);
	g_iFaceMan->registerIFaceUser(&g_cvars,CVARS_API_IDENTSTR);
	g_iFaceMan->registerIFaceUser(&g_core,CORE_API_IDENTSTR);
	g_iFaceMan->registerIFaceUser(&cm,CM_API_IDENTSTR);
	g_iFaceMan->registerIFaceUser(&g_loadingScreen,LOADINGSCREENMGR_API_IDENTSTR);
	g_iFaceMan->registerIFaceUser(&rf,RENDERER_API_IDENTSTR);
	g_iFaceMan->registerIFaceUser(&g_declMgr,DECL_MANAGER_API_IDENTSTR);
	g_iFaceMan->registerIFaceUser(&g_moduleMgr,MODULEMANAGER_API_IDENTSTR);
}

qioModule_e IFM_GetCurModule() {
	return QMD_GAME;
}

