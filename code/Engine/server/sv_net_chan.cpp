////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 1999-2005 Id Software, Inc.
//  Copyright (C) 2012-2014 V.
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
//  File name:   sv_net_chan.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "server.h"

#ifdef LEGACY_PROTOCOL
/*
==============
SV_Netchan_Encode

	// first four bytes of the data are always:
	long reliableAcknowledge;

==============
*/
static void SV_Netchan_Encode( client_t* client, msg_s* msg, const char* clientCommandString )
{
    long i, index;
    byte key, *string;
    int	srdc, sbit;
    bool soob;
    
    if( msg->cursize < SV_ENCODE_START )
    {
        return;
    }
    
    srdc = msg->readcount;
    sbit = msg->bit;
    soob = msg->oob;
    
    msg->bit = 0;
    msg->readcount = 0;
    msg->oob = false;
    
    /* reliableAcknowledge = */
    MSG_ReadLong( msg );
    
    msg->oob = soob;
    msg->bit = sbit;
    msg->readcount = srdc;
    
    string = ( byte* ) clientCommandString;
    index = 0;
    // xor the client challenge with the netchan sequence number
    key = client->challenge ^ client->netchan.outgoingSequence;
    for( i = SV_ENCODE_START; i < msg->cursize; i++ )
    {
        // modify the key with the last received and with this message acknowledged client command
        if( !string[index] )
            index = 0;
        if( string[index] > 127 || string[index] == '%' )
        {
            key ^= '.' << ( i & 1 );
        }
        else
        {
            key ^= string[index] << ( i & 1 );
        }
        index++;
        // encode the data with this key
        *( msg->data + i ) = *( msg->data + i ) ^ key;
    }
}

/*
==============
SV_Netchan_Decode

	// first 12 bytes of the data are always:
	long serverId;
	long messageAcknowledge;
	long reliableAcknowledge;

==============
*/
static void SV_Netchan_Decode( client_t* client, msg_s* msg )
{
    int serverId, messageAcknowledge, reliableAcknowledge;
    int i, index, srdc, sbit;
    bool soob;
    byte key, *string;
    
    srdc = msg->readcount;
    sbit = msg->bit;
    soob = msg->oob;
    
    msg->oob = false;
    
    serverId = MSG_ReadLong( msg );
    messageAcknowledge = MSG_ReadLong( msg );
    reliableAcknowledge = MSG_ReadLong( msg );
    
    msg->oob = soob;
    msg->bit = sbit;
    msg->readcount = srdc;
    
    string = ( byte* )client->reliableCommands[ reliableAcknowledge & ( MAX_RELIABLE_COMMANDS - 1 ) ];
    index = 0;
    //
    key = client->challenge ^ serverId ^ messageAcknowledge;
    for( i = msg->readcount + SV_DECODE_START; i < msg->cursize; i++ )
    {
        // modify the key with the last sent and acknowledged server command
        if( !string[index] )
            index = 0;
        if( string[index] > 127 || string[index] == '%' )
        {
            key ^= '.' << ( i & 1 );
        }
        else
        {
            key ^= string[index] << ( i & 1 );
        }
        index++;
        // decode the data with this key
        *( msg->data + i ) = *( msg->data + i ) ^ key;
    }
}
#endif



/*
=================
SV_Netchan_FreeQueue
=================
*/
void SV_Netchan_FreeQueue( client_t* client )
{
    netchan_buffer_t* netbuf, *next;
    
    for( netbuf = client->netchan_start_queue; netbuf; netbuf = next )
    {
        next = netbuf->next;
        free( netbuf );
    }
    
    client->netchan_start_queue = NULL;
    client->netchan_end_queue = &client->netchan_start_queue;
}

/*
=================
SV_Netchan_TransmitNextInQueue
=================
*/
void SV_Netchan_TransmitNextInQueue( client_t* client )
{
    netchan_buffer_t* netbuf;
    
    Com_DPrintf( "#462 Netchan_TransmitNextFragment: popping a queued message for transmit\n" );
    netbuf = client->netchan_start_queue;
    
#ifdef LEGACY_PROTOCOL
    if( client->compat )
        SV_Netchan_Encode( client, &netbuf->msg, netbuf->clientCommandString );
#endif
        
    Netchan_Transmit( &client->netchan, netbuf->msg.cursize, netbuf->msg.data );
    
    // pop from queue
    client->netchan_start_queue = netbuf->next;
    if( !client->netchan_start_queue )
    {
        Com_DPrintf( "#462 Netchan_TransmitNextFragment: emptied queue\n" );
        client->netchan_end_queue = &client->netchan_start_queue;
    }
    else
        Com_DPrintf( "#462 Netchan_TransmitNextFragment: remaining queued message\n" );
        
    free( netbuf );
}

/*
=================
SV_Netchan_TransmitNextFragment
Transmit the next fragment and the next queued packet
Return number of ms until next message can be sent based on throughput given by client rate,
-1 if no packet was sent.
=================
*/

int SV_Netchan_TransmitNextFragment( client_t* client )
{
    if( client->netchan.unsentFragments )
    {
        Netchan_TransmitNextFragment( &client->netchan );
        return SV_RateMsec( client );
    }
    else if( client->netchan_start_queue )
    {
        SV_Netchan_TransmitNextInQueue( client );
        return SV_RateMsec( client );
    }
    
    return -1;
}


/*
===============
SV_Netchan_Transmit
TTimo
https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=462
if there are some unsent fragments (which may happen if the snapshots
and the gamestate are fragmenting, and collide on send for instance)
then buffer them and make sure they get sent in correct order
================
*/
#include <zlib.h>
#include <shared/autoCvar.h>

aCvar_c sv_compressPackets( "sv_compressPackets", "1" );
aCvar_c sv_printCompressedPacketSize( "sv_printCompressedPacketSize", "0" );

void SV_Netchan_Transmit( client_t* client, msg_s* msg )
{
    MSG_WriteByte( msg, svc_EOF );
    
    // try to compress message with zlib
    // it's only usefull while sending a very large gamestate message
    if( sv_compressPackets.getInt() )
    {
        byte buff[MAX_MSGLEN];
        uLongf destLen = sizeof( buff );
        int res = compress( buff, &destLen, msg->data + 1, msg->cursize - 1 );
        if( res != Z_OK )
        {
            Com_RedWarning( "SV_Netchan_Transmit: zlib compress failed for %i bytes.\n", msg->cursize - 1 );
        }
        else
        {
            if( sv_printCompressedPacketSize.getInt() )
            {
                Com_Printf( "SV_Netchan_Transmit: zlib compressed %i to %i\n", msg->cursize, destLen );
            }
            // if the compressed message is smaller than original one, send compressed data instead.
            // very small messages like snapshots usually have even bigger size after compression !!
            if( destLen < msg->cursize )
            {
                MSG_Init( msg, msg->data, msg->maxsize );
                msg->oob = true;
                // write compression marker (1 is 'zlib')
                MSG_WriteByte( msg, 1 );
                // write message data compressed with zlib
                MSG_WriteData( msg, buff, destLen );
                // done.
                msg->oob = false;
            }
        }
    }
    
    if( client->netchan.unsentFragments || client->netchan_start_queue )
    {
        netchan_buffer_t* netbuf;
        Com_DPrintf( "#462 SV_Netchan_Transmit: unsent fragments, stacked\n" );
        netbuf = ( netchan_buffer_t* ) malloc( sizeof( netchan_buffer_t ) );
        // store the msg, we can't store it encoded, as the encoding depends on stuff we still have to finish sending
        MSG_Copy( &netbuf->msg, netbuf->msgBuffer, sizeof( netbuf->msgBuffer ), msg );
#ifdef LEGACY_PROTOCOL
        if( client->compat )
        {
            Q_strncpyz( netbuf->clientCommandString, client->lastClientCommandString,
                        sizeof( netbuf->clientCommandString ) );
        }
#endif
        netbuf->next = NULL;
        // insert it in the queue, the message will be encoded and sent later
        *client->netchan_end_queue = netbuf;
        client->netchan_end_queue = &( *client->netchan_end_queue )->next;
    }
    else
    {
#ifdef LEGACY_PROTOCOL
        if( client->compat )
            SV_Netchan_Encode( client, msg, client->lastClientCommandString );
#endif
        Netchan_Transmit( &client->netchan, msg->cursize, msg->data );
    }
}

/*
=================
Netchan_SV_Process
=================
*/
bool SV_Netchan_Process( client_t* client, msg_s* msg )
{
    int ret;
    ret = Netchan_Process( &client->netchan, msg );
    if( !ret )
        return false;
        
#ifdef LEGACY_PROTOCOL
    if( client->compat )
        SV_Netchan_Decode( client, msg );
#endif
        
    return true;
}

