/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
// cg_playerstate.c -- this file acts on changes in a new playerState_s
// With normal play, this will be done after local prediction, but when
// following another player or playing back a demo, it will be checked
// when the snapshot transitions like all the other entities

#include "cg_local.h"

/*
================
CG_Respawn

A respawn happened this snapshot
================
*/
void CG_Respawn( void )
{
	// no error decay on player movement
	//  cg.thisFrameTeleport = true;
	
	
}

extern char* eventnames[];

/*
===============
CG_TransitionPlayerState

===============
*/
void CG_TransitionPlayerState( playerState_s* ps, playerState_s* ops )
{
	// check for changing follow mode
	if ( ps->clientNum != ops->clientNum )
	{
		//      cg.thisFrameTeleport = true;
		// make sure we don't get any unwanted transition effects
		*ops = *ps;
	}
	
	//  if ( cg.mapRestart ) {
	///     CG_Respawn();
	///     cg.mapRestart = false;
	//  }
	
	// smooth the ducking viewheight change
	if ( ps->viewheight != ops->viewheight )
	{
		cg.duckChange = ps->viewheight - ops->viewheight;
		cg.duckTime = cg.time;
	}
}

