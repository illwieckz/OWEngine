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
//  File name:   sv_game.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: interface to the game DLL
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "server.h"
#include <api/iFaceMgrAPI.h>
#include <api/moduleManagerAPI.h>
#include <api/gameAPI.h>
#include <shared/portalizedBSPTree.h>

static moduleAPI_i* sv_gameDLL = 0;
gameAPI_s* g_game = 0;
gameClientAPI_s* g_gameClients = 0;

// these functions must be used instead of pointer arithmetic, because
// the game allocates gentities with private information after the server shared part
int	SV_NumForGentity( edict_s* ent )
{
    int		num;
    
    // NOTE: edict_s size is now fixed. All the game code should be put in revelant entity classes
    num = ( ( byte* )ent - ( byte* )sv.gentities ) / sizeof( edict_s );
    
    return num;
}

edict_s* SV_GentityNum( int num )
{
    edict_s* ent;
    
    // NOTE: edict_s size is now fixed. All the game code should be put in revelant entity classes
    ent = ( edict_s* )( ( byte* )sv.gentities + sizeof( edict_s ) * ( num ) );
    
    return ent;
}

playerState_s* SV_GameClientNum( int num )
{
    playerState_s*	ps;
    
    ps = g_gameClients->ClientGetPlayerState( num );
    
    return ps;
}

svEntity_t*	SV_SvEntityForGentity( edict_s* gEnt )
{
    if( !gEnt || gEnt->s->number < 0 || gEnt->s->number >= MAX_GENTITIES )
    {
        Com_Error( ERR_DROP, "SV_SvEntityForGentity: bad gEnt" );
    }
    return &sv.svEntities[ gEnt->s->number ];
}

edict_s* SV_GEntityForSvEntity( svEntity_t* svEnt )
{
    int		num;
    
    num = svEnt - sv.svEntities;
    return SV_GentityNum( num );
}

/*
===============
SV_GameSendServerCommand

Sends a command string to a client
===============
*/
void SV_GameSendServerCommand( int clientNum, const char* text )
{
    if( clientNum == -1 )
    {
        SV_SendServerCommand( NULL, "%s", text );
    }
    else
    {
        if( clientNum < 0 || clientNum >= sv_maxclients->integer )
        {
            return;
        }
        SV_SendServerCommand( svs.clients + clientNum, "%s", text );
    }
}


/*
===============
SV_GameDropClient

Disconnects the client with a message
===============
*/
void SV_GameDropClient( int clientNum, const char* reason )
{
    if( clientNum < 0 || clientNum >= sv_maxclients->integer )
    {
        return;
    }
    SV_DropClient( svs.clients + clientNum, reason );
}

#include "sv_vis.h"
void SV_LinkEntity( edict_s* ed )
{
    // ensure that we have bspBoxDesc allocated
    if( ed->bspBoxDesc == 0 )
    {
        ed->bspBoxDesc = new bspBoxDesc_s;
    }
    if( sv_bsp )
    {
        sv_bsp->filterBB( ed->absBounds, *ed->bspBoxDesc );
    }
    else if( sv_procTree )
    {
        sv_procTree->boxAreaNums( ed->absBounds, ed->bspBoxDesc->areas );
    }
}
void SV_UnlinkEntity( edict_s* ed )
{
    // free bspBoxDesc
    if( ed->bspBoxDesc )
    {
        delete ed->bspBoxDesc;
        ed->bspBoxDesc = 0;
    }
}
void SV_AdjustAreaPortalState( int area0, int area1, bool open )
{
    if( sv_bsp )
    {
        sv_bsp->adjustAreaPortalState( area0, area1, open );
    }
}
/*
===============
SV_GetServerinfo

===============
*/
void SV_GetServerinfo( char* buffer, int bufferSize )
{
    if( bufferSize < 1 )
    {
        Com_Error( ERR_DROP, "SV_GetServerinfo: bufferSize == %i", bufferSize );
    }
    Q_strncpyz( buffer, Cvar_InfoString( CVAR_SERVERINFO ), bufferSize );
}

/*
===============
SV_LocateGameData

===============
*/
void SV_LocateGameData( edict_s* gEnts, int numGEntities )
{
    sv.gentities = gEnts;
    sv.num_entities = numGEntities;
}


/*
===============
SV_GetUsercmd

===============
*/
void SV_GetUsercmd( int clientNum, userCmd_s* cmd )
{
    if( clientNum < 0 || clientNum >= sv_maxclients->integer )
    {
        Com_Error( ERR_DROP, "SV_GetUsercmd: bad clientNum:%i", clientNum );
    }
    *cmd = svs.clients[clientNum].lastUsercmd;
}

//==============================================

/*
===============
SV_ShutdownGameProgs

Called every time a map changes
===============
*/
void SV_ShutdownGameProgs( void )
{
    if( !sv_gameDLL )
    {
        return;
    }
    g_game->ShutdownGame( false );
    g_moduleMgr->unload( &sv_gameDLL );
}

/*
==================
SV_InitGameVM

Called for both a full init and a restart
==================
*/
static void SV_InitGameVM( bool restart )
{
    int		i;
    
    // clear all gentity pointers that might still be set from
    // a previous level
    // https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=522
    //   now done before GAME_INIT call
    for( i = 0 ; i < sv_maxclients->integer ; i++ )
    {
        svs.clients[i].gentity = NULL;
    }
    
    // use the current msec count for a random seed
    // init for this gamestate
    g_game->InitGame( sv.time, Com_Milliseconds(), restart );
}



/*
===================
SV_RestartGameProgs

Called on a map_restart, but not on a normal map change
===================
*/
void SV_RestartGameProgs( void )
{
    if( !sv_gameDLL )
    {
        return;
    }
    g_game->ShutdownGame( true );
    
    // do a restart instead of a free
    sv_gameDLL = g_moduleMgr->restart( sv_gameDLL, true );
    if( !sv_gameDLL )
    {
        Com_Error( ERR_FATAL, "VM_Restart on game failed" );
    }
    
    SV_InitGameVM( true );
}

/*
===============
SV_InitGameProgs

Called on a normal map change, not on a map_restart
===============
*/
void SV_InitGameProgs( void )
{
    // load the dll or bytecode
    sv_gameDLL = g_moduleMgr->load( "qagame" );
    if( !sv_gameDLL )
    {
        Com_Error( ERR_FATAL, "VM_Create on game failed" );
    }
    g_iFaceMan->registerIFaceUser( &g_game, GAME_API_IDENTSTR );
    if( !g_game )
    {
        Com_Error( ERR_DROP, "Game module has wrong interface version (%s required)", GAME_API_IDENTSTR );
    }
    g_iFaceMan->registerIFaceUser( &g_gameClients, GAMECLIENTS_API_IDENTSTR );
    if( !g_gameClients )
    {
        Com_Error( ERR_DROP, "GameClients module has wrong interface version (%s required)", GAMECLIENTS_API_IDENTSTR );
    }
    SV_InitGameVM( false );
}


/*
====================
SV_GameCommand

See if the current console command is claimed by the game
====================
*/
bool SV_GameCommand( void )
{
    //if ( sv.state != SS_GAME ) {
    return false;
    //}
    // TODO
    //return VM_Call( gvm, GAME_CONSOLE_COMMAND );
}

