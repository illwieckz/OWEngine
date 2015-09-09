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
//  File name:   cgameAPI.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: engine core interface
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __COREAPI_H__
#define __COREAPI_H__

#include "iFaceBase.h"
#include "../qcommon/q_shared.h" // only for __attribute__

#define CORE_API_IDENTSTR "CoreEngineAPI0001"

// these are only temporary function pointers, TODO: rework them?
struct coreAPI_s : public iFaceBase_i
{
	void ( *Print )( const char* text, ... );
	void ( *RedWarning )( const char* text, ... );
	void ( *Error )( int level, const char* text, ... ) __attribute__( ( noreturn ) );
	void ( *DropError )( const char* text, ... ) __attribute__( ( noreturn ) );
	// milliseconds should only be used for performance tuning, never
	// for anything game related.
	int ( *Milliseconds )( void );
	// engine command system api
	int ( *Argc )( void );
	void ( *ArgvBuffer )( int n, char* buffer, int bufferLength );
	void ( *Args )( char* buffer, int bufferLength );
	const char* ( *Argv )( int n );
	void ( *Cmd_AddCommand )( const char* name, void ( *func )() );
	void ( *Cmd_RemoveCommand )( const char* name );
	void ( *Cbuf_ExecuteText )( int exec_when, const char* text );
};

extern coreAPI_s* g_core;

#endif // __COREAPI_H__
