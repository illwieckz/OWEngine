////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 1999-2005 Id Software, Inc.
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
//  File name:   bg_public.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Definitions shared by both the serverg game and
//               the client game modules
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __BG_PUBLIC_H__
#define __BG_PUBLIC_H__

#include <protocol/netLimits.h>

// because games can change separately from the main system version, we need a
// second version that must match between game and cgame

#define GAME_VERSION        BASEGAME "-1"

//#define   DEFAULT_GRAVITY     800

//#define   MINS_Z              -24
//#define   DEFAULT_VIEWHEIGHT  82

//
// config strings are a general means of communicating variable length strings
// from the server to all connected clients.
//

// CS_SERVERINFO and CS_SYSTEMINFO are defined in q_shared.h


#define CS_GAME_VERSION         20
#define CS_LEVEL_START_TIME     21      // so the timer only shows the current level
#define CS_WORLD_SKYMATERIAL    22
#define CS_WORLD_WATERLEVEL     23
// zFar value (far clip plane)
#define CS_WORLD_FARPLANE       24

// renderer models
#define CS_MODELS               32
// collision models (cm module)
#define CS_COLLMODELS           (CS_MODELS+MAX_MODELS)
// sounds
#define CS_SOUNDS               (CS_COLLMODELS+MAX_MODELS)
#define CS_ANIMATIONS           (CS_SOUNDS+MAX_SOUNDS)
#define CS_RAGDOLLDEFSS         (CS_ANIMATIONS+MAX_ANIMATIONS)
#define CS_SKINS                (CS_RAGDOLLDEFSS+MAX_RAGDOLLDEFS)
#define CS_MATERIALS                (CS_SKINS+MAX_SKINS)
//#define CS_LOCATIONS          (CS_PLAYERS+MAX_CLIENTS)
//#define CS_PARTICLES          (CS_LOCATIONS+MAX_LOCATIONS)
//
//#define CS_MAX                    (CS_PARTICLES+MAX_LOCATIONS)
#define CS_MAX                  (CS_MATERIALS+MAX_MATERIALS)

#if (CS_MAX) > MAX_CONFIGSTRINGS
#error overflow: (CS_MAX) > MAX_CONFIGSTRINGS
#endif

// if a full pmove isn't done on the client, you can just update the angles
void PM_UpdateViewAngles( struct playerState_s* ps, const struct userCmd_s* cmd );

//===================================================================================

void    BG_PlayerStateToEntityState( struct playerState_s* ps, struct entityState_s* s, bool snap );

#endif // __BG_PUBLIC_H__
