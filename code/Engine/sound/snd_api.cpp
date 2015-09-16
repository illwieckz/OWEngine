////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
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
//  File name:   snd_api.cpp
//  Version:     v1.00
//  Created:     09-16-2016
//  Compilers:   Visual Studio
//  Description: Sound API implementation
// -------------------------------------------------------------------------
//  History:
//  09-16-2015: Added basic support for sound module
//
////////////////////////////////////////////////////////////////////////////

// snd_api.cpp - sound DLL entry point

#include <qcommon/q_shared.h>
#include <api/iFaceMgrAPI.h>
#include <api/vfsAPI.h>
#include <api/cvarAPI.h>
#include <api/coreAPI.h>
#include <shared/autoCvar.h>
#include <shared/autoCmd.h>
#include <api/sndAPI.h>

// interface manager (import)
class iFaceMgrAPI_i* g_iFaceMan = 0;

// imports
vfsAPI_s* g_vfs = 0;
cvarsAPI_s* g_cvars = 0;
coreAPI_s* g_core = 0;

// exports
static sndAPI_s g_staticSndAPI;

void ShareAPIs( iFaceMgrAPI_i* iFMA )
{
	g_iFaceMan = iFMA;
	
	// exports
	g_iFaceMan->registerInterface( &g_staticSndAPI, SND_API_IDENTSTR );
	
	// imports
	g_iFaceMan->registerIFaceUser( &g_vfs, VFS_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_cvars, CVARS_API_IDENTSTR );
	g_iFaceMan->registerIFaceUser( &g_core, CORE_API_IDENTSTR );
}

qioModule_e IFM_GetCurModule()
{
	return QMD_SOUND;
}

