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
//  File name:   Parse.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Text file parsing routines
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "qe3.h"

char	token[MAXTOKEN];
bool	unget;
char*	script_p;
int		scriptline;

void	StartTokenParsing( char* data )
{
    scriptline = 1;
    script_p = data;
    unget = false;
}

bool WINAPI GetToken( bool crossline )
{
    char*    token_p;
    
    if( unget )                        // is a token allready waiting?
    {
        unget = false;
        return true;
    }
    
//
// skip space
//
skipspace:
    while( *script_p <= 32 )
    {
        if( !*script_p )
        {
            if( !crossline )
                Sys_Printf( "Warning: Line %i is incomplete [01]\n", scriptline );
            return false;
        }
        if( *script_p++ == '\n' )
        {
            if( !crossline )
                Sys_Printf( "Warning: Line %i is incomplete [02]\n", scriptline );
            scriptline++;
        }
    }
    
    if( script_p[0] == '/' && script_p[1] == '/' )	// comment field
    {
        if( !crossline )
            Sys_Printf( "Warning: Line %i is incomplete [03]\n", scriptline );
        while( *script_p++ != '\n' )
            if( !*script_p )
            {
                if( !crossline )
                    Sys_Printf( "Warning: Line %i is incomplete [04]\n", scriptline );
                return false;
            }
        goto skipspace;
    }
    
//
// copy token
//
    token_p = token;
    
    if( *script_p == '"' )
    {
        script_p++;
        //if (*script_p == '"')   // handle double quotes i suspect they are put in by other editors cccasionally
        //  script_p++;
        while( *script_p != '"' )
        {
            if( !*script_p )
                Error( "EOF inside quoted token" );
            *token_p++ = *script_p++;
            if( token_p == &token[MAXTOKEN] )
                Error( "Token too large on line %i", scriptline );
        }
        script_p++;
        //if (*script_p == '"')   // handle double quotes i suspect they are put in by other editors cccasionally
        //  script_p++;
    }
    else while( *script_p > 32 )
        {
            *token_p++ = *script_p++;
            if( token_p == &token[MAXTOKEN] )
                Error( "Token too large on line %i", scriptline );
        }
        
    *token_p = 0;
    
    return true;
}

void WINAPI UngetToken( void )
{
    unget = true;
}


/*
==============
TokenAvailable

Returns true if there is another token on the line
==============
*/
bool TokenAvailable( void )
{
    char*    search_p;
    
    search_p = script_p;
    
    while( *search_p <= 32 )
    {
        if( *search_p == '\n' )
            return false;
        if( *search_p == 0 )
            return false;
        search_p++;
    }
    
    if( *search_p == ';' )
        return false;
        
    return true;
}

