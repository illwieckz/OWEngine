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
//  File name:   serverAPI.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: server core API, used by serverGame module
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SERVERAPI_H__
#define __SERVERAPI_H__

#include "iFaceBase.h"

#define SERVER_API_IDENTSTR "ServerAPI0001"

typedef struct edict_s edict_s;

// these are only temporary function pointers, TODO: rework them?
struct svAPI_s : public iFaceBase_i
{
    void ( *LocateGameData )( edict_s* gEnts, int numGEntities );
    void ( *DropClient )( int clientNum, const char* reason );
    void ( *SendServerCommand )( int clientNum, const char* text );
    void ( *SetConfigstring )( int num, const char* string );
    void ( *GetConfigstring )( int num, char* buffer, int bufferSize );
    void ( *GetUserinfo )( int num, char* buffer, int bufferSize );
    void ( *SetUserinfo )( int num, const char* buffer );
    void ( *GetUsercmd )( int clientNum, struct userCmd_s* cmd );
    
    void ( *linkEntity )( edict_s* ed );
    void ( *unlinkEntity )( edict_s* ed );
    
    // Quake3-style areaPortals access
    void ( *adjustAreaPortalState )( int area0, int area1, bool open );
};

extern svAPI_s* g_server;

#endif // __SERVERAPI_H__
