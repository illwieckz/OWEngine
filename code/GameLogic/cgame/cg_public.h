/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2011-2015 Dusan Jocic <dusanjocic@msn.com>

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

#include <protocol/playerState.h>

#define CMD_BACKUP          64
#define CMD_MASK            (CMD_BACKUP - 1)
// allow a lot of command backups for very fast systems
// multiple commands may be combined into a single packet, so this
// needs to be larger than PACKET_BACKUP


#define MAX_ENTITIES_IN_SNAPSHOT    MAX_GENTITIES

// snapshots are a view of the server at a given time

// Snapshots are generated at regular time intervals by the server,
// but they may not be sent if a client's rate level is exceeded, or
// they may be dropped by the network.
typedef struct
{
	int             snapFlags;          // SNAPFLAG_RATE_DELAYED, etc
	int             ping;
	
	int             serverTime;     // server time the message is valid for (in msec)
	
	byte            areamask[MAX_MAP_AREA_BYTES];       // portalarea visibility bits
	
	playerState_s   ps;                     // complete information about the current player at this time
	
	int             numEntities;            // all of the entities that need to be presented
	entityState_s   entities[MAX_ENTITIES_IN_SNAPSHOT]; // at the time of this snapshot
	
	int             numServerCommands;      // text based server commands to execute when this
	int             serverCommandSequence;  // snapshot becomes current
} snapshot_t;

/*
==================================================================

functions imported from the main executable

==================================================================
*/

#define CGAME_IMPORT_API_VERSION    4

typedef enum
{
	CG_PRINT,
	CG_ERROR,
	CG_MILLISECONDS,
	CG_CVAR_REGISTER,
	CG_CVAR_UPDATE,
	CG_CVAR_SET,
	CG_CVAR_VARIABLESTRINGBUFFER,
	CG_ARGC,
	CG_ARGV,
	CG_ARGS,
	CG_SENDCONSOLECOMMAND,
	CG_ADDCOMMAND,
	CG_SENDCLIENTCOMMAND,
	CG_UPDATESCREEN,
	
	CG_R_LOADWORLDMAP,
	CG_R_REGISTERSHADER,
	CG_R_CLEARSCENE,
	CG_R_RENDERSCENE,
	CG_R_SETCOLOR,
	CG_R_DRAWSTRETCHPIC,
	CG_GETGAMESTATE,
	CG_GETCURRENTSNAPSHOTNUMBER,
	CG_GETSNAPSHOT,
	CG_GETSERVERCOMMAND,
	CG_GETCURRENTCMDNUMBER,
	CG_GETUSERCMD,
	CG_R_REGISTERSHADERNOMIP,
	CG_REAL_TIME,
	CG_REMOVECOMMAND,
	
	
} cgameImport_t;


/*
==================================================================

functions exported to the main executable

==================================================================
*/

typedef enum
{
	CG_INIT,
	//  void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum )
	// called when the level loads or when the renderer is restarted
	// all media should be registered at this time
	// cgame will display loading status by calling SCR_Update, which
	// will call CG_DrawInformation during the loading process
	// reliableCommandSequence will be 0 on fresh loads, but higher for
	// demos, tourney restarts, or vid_restarts
	
	CG_SHUTDOWN,
	//  void (*CG_Shutdown)( void );
	// oportunity to flush and close any open files
	
	CG_DRAW_ACTIVE_FRAME,
	//  void (*CG_DrawActiveFrame)( int serverTime, stereoFrame_t stereoView, bool demoPlayback );
	// Generates and draws a game scene and status information at the given time.
	// If demoPlayback is set, local movement prediction will not be enabled
	
	
	
} cgameExport_t;

//----------------------------------------------
