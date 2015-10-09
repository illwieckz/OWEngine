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

#include <fmod.h>
#include <fmod_errors.h>

#define MAX_SFX 4096
#define MAX_FMOD_CHANNELS 2048
#define DEFAULT_FMOD_MODE FMOD_3D | FMOD_IGNORETAGS | FMOD_LOWMEM
#define DEFAULT_FMOD_INIT FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED | FMOD_INIT_VOL0_BECOMES_VIRTUAL

typedef struct fmodSfx_s
{
	char filename[256];
	FMOD_SOUND* sound;
	bool isDefault;
	int lastUsedTime;
} fmodSfx_t;

static FMOD_SYSTEM* fmodSystem;
static fmodSfx_t loadedSfx[4096];

bool Sound_Init();
void Sound_Shutdown( void );
