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
//  File name:   cvarAPI.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: cVars (console variables) system
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __CVARRAPI_H__
#define __CVARRAPI_H__

#include "iFaceBase.h"

#define CVARS_API_IDENTSTR "CvarsAPI0001"

// these are only temporary function pointers, TODO: rework them?
struct cvarsAPI_s : public iFaceBase_i
{
    void ( *Cvar_Register )( vmCvar_s* cvar, const char* var_name, const char* value, int flags );
    void ( *Cvar_Update )( vmCvar_s* cvar );
    void ( *Cvar_Set )( const char* var_name, const char* value );
    int	( *Cvar_VariableIntegerValue )( const char* var_name );
    float( *Cvar_VariableValue )( const char* var_name );
    void ( *Cvar_VariableStringBuffer )( const char* var_name, char* buffer, int bufsize );
    struct cvar_s* ( *Cvar_Get )( const char* var_name, const char* var_value, int flags );
    void ( *Cvar_AddModificationCallback )( struct cvar_s* cv, class cvarModifyCallback_i* callback );
    void ( *Cvar_RemoveModificationCallback )( struct cvar_s* cv, class cvarModifyCallback_i* callback );
};

extern cvarsAPI_s* g_cvars;

#endif // __CVARRAPI_H__
