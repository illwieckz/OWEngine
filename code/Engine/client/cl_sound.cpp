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
//  File name:   cl_sound.cpp
//  Version:     v1.00
//  Created:     10-01-2015
//  Compilers:   Visual Studio
//  Description: sound module access
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "client.h"
#include <api/iFaceMgrAPI.h>
#include <api/moduleManagerAPI.h>
#include <api/sndAPI.h>
#include <api/coreAPI.h>

#include <shared/str.h>

#include "../sys/sys_local.h"
#include "../sys/sys_loadlib.h"

static moduleAPI_i* cl_soundLib = 0;
sndAPI_s* g_snd;

void CL_SoundModule( void )
{
	// sound initialization
	Com_Printf( "----- Initializing Sound DLL ----\n" );
	cl_soundLib = g_moduleMgr->load( "soundLib" );
	if ( !cl_soundLib )
	{
		Com_Error( ERR_FATAL, "Couldn't load sound DLL" );
	}
	g_iFaceMan->registerIFaceUser( &g_snd, SND_API_IDENTSTR );
	g_snd->Init();
	Com_Printf( "-------------------------------\n" );
	
}
void CL_ShutdownSound( void )
{
	if ( !cl_soundLib )
	{
		return;
	}
	g_snd->Shutdown();
	// first sound plugin
	g_moduleMgr->unload( &cl_soundLib );
}
