////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 1999-2005 Id Software, Inc.
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
//  File name:   g_active.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "g_local.h"
#include "classes/Player.h"
#include <api/serverAPI.h>

/*
==============
ClientThink

This will be called once for each client frame, which will
usually be a couple times for each server frame on fast clients.

If "g_synchronousClients 1" is set, this will be called exactly
once for each server frame, which makes for smooth demo recording.
==============
*/
void ClientThink_real( edict_s* ent )
{
	Player* pl = dynamic_cast<Player*>( ent->ent );
	
	// don't think if the client is not yet connected (and thus not yet spawned in)
	if ( pl->pers.connected != CON_CONNECTED )
	{
		return;
	}
	pl->runPlayer();
}

/*
==================
ClientThink

A new command has arrived from the client
==================
*/
void ClientThink( int clientNum )
{
	edict_s* ent;
	
	ent = g_entities + clientNum;
	Player* pl = dynamic_cast<Player*>( ent->ent );
	g_server->GetUsercmd( clientNum, &pl->pers.cmd );
}


void G_RunClient( edict_s* ent )
{
	Player* pl = dynamic_cast<Player*>( ent->ent );
	pl->pers.cmd.serverTime = level.time;
	ClientThink_real( ent );
}

/*
==============
ClientEndFrame

Called at the end of each server frame for each connected client
A fast client will have multiple ClientThink for each ClientEdFrame,
while a slow client may have multiple ClientEndFrame between ClientThink.
==============
*/
void ClientEndFrame( edict_s* ent )
{
	//  Player *pl = dynamic_cast<Player*>(ent->ent);
	// now playerState_s inherts from entityState
	//BG_PlayerStateToEntityState( &pl->ps, &ent->s, true );
}


