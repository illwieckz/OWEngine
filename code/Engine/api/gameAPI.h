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
//  File name:   gameAPI.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: server game DLL interface
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __GAMEAPI_H__
#define __GAMEAPI_H__

#include "iFaceBase.h"

#define GAME_API_IDENTSTR "ServerGameAPI0001"

// these are only temporary function pointers, TODO: rework them?
struct gameAPI_s : public iFaceBase_i
{
	void ( *InitGame )( int levelTime, int randomSeed, int restart );
	void ( *RunFrame )( int levelTime );
	void ( *ShutdownGame )( int restart );
	void ( *DebugDrawFrame )( class rAPI_i* pRFAPI );
};

#define GAMECLIENTS_API_IDENTSTR "ServerGameClientsAPI0001"

struct gameClientAPI_s : public iFaceBase_i
{
	const char* ( *ClientConnect )( int clientNum, bool firstTime, bool isBot );
	void ( *ClientUserinfoChanged )( int clientNum );
	void ( *ClientDisconnect )( int clientNum );
	void ( *ClientBegin )( int clientNum );
	void ( *ClientCommand )( int clientNum );
	void ( *ClientThink )( int clientNum );
	struct playerState_s* ( *ClientGetPlayerState )( u32 clientNum );
};

extern gameAPI_s* g_game;
extern gameClientAPI_s* g_gameClients;

#endif // __GAMEAPI_H__
