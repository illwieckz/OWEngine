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
//  File name:   snd_main.cpp
//  Version:     v1.00
//  Created:     10-01-2015
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include <api/sndAPI.h>
#include <api/coreAPI.h>
#include <math/vec3.h>

#include "snd_local.h"

#include <fmod.h>
#include <fmod_errors.h>

bool Sound_Init()
{
	g_core->Print( "------ Initializing FMOD Sound ------\n" );
	
	FMOD_RESULT result = FMOD_System_Create( &fmodSystem );
	if ( !fmodSystem )
	{
		g_core->Print( "FMOD_System_Create failed (%s)\n", FMOD_ErrorString( result ) );
		g_core->Print( "Sound initialization failed.\n" );
		return false; // error
	}
	
	u32 version;
	FMOD_System_GetVersion( fmodSystem, &version );
	g_core->Print( "FMOD Ex Version: %08x\n", version );
	
	FMOD_System_Init( fmodSystem, MAX_FMOD_CHANNELS, DEFAULT_FMOD_INIT, NULL );
	g_core->Print( "FMOD Ex system initialized\n" );
	
	return true; // OK
}
void Sound_Shutdown( void )
{
	g_core->Print( "------ Shutdown Sound ------\n" );
	
	for ( u32 i = 0; i < MAX_SFX; i++ )
	{
		FMOD_SOUND* sfx = loadedSfx[i].sound;
		if ( sfx )
		{
			FMOD_Sound_Release( sfx );
		}
	}
}
