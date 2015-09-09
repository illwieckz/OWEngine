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
//  File name:   parser.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Simple parser class
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "parser.h"
#include <api/vfsAPI.h>

parser_c::parser_c()
{
    base = 0;
    p = 0;
    fileData = 0;
}
parser_c::~parser_c()
{
    clear();
}

void parser_c::clear()
{
    if( fileData )
    {
        g_vfs->FS_FreeFile( fileData );
    }
}
bool parser_c::openFile( const char* fname )
{
    this->clear();
    int len = g_vfs->FS_ReadFile( fname, ( void** )&fileData );
    if( fileData == 0 )
    {
        return true; // cannot open
    }
    base = ( const char* )fileData;
    p = base;
    debugFileName = fname;
    return false;
}
void parser_c::setup( const char* newText, const char* newP )
{
    this->clear();
    base = newText;
    if( newP == 0 )
    {
        p = base;
    }
    else
    {
        p = newP;
    }
}
void parser_c::setDebugFileName( const char* newDebugFileName )
{
    this->debugFileName = newDebugFileName;
}
bool parser_c::skipToNextToken()
{
    while( *p )
    {
        if( p[0] == '/' )
        {
            if( p[1] == '*' )
            {
                p += 2; // skip "/*"
                while( 1 )
                {
                    if( *p == 0 )
                    {
                        printf( "Error: unexpected end of file hit in multi-line comment\n" );
                        return true; // EOF
                    }
                    else if( p[0] == '*' && p[1] == '/' )
                    {
                        break;
                    }
                    p++;
                }
                p += 2;  // skip "*/"
            }
            else if( p[1] == '/' )
            {
                p += 2; // skip "//"
                while( *p != '\n' )
                {
                    if( *p == 0 )
                    {
                        printf( "Error: unexpected end of file hit in single-line comment\n" );
                        return true; // EOF
                    }
                    p++;
                }
                p++; // skip '\n'
            }
            else
            {
                break;
            }
        }
        else if( G_isWS( *p ) == false )
        {
            break;
        }
        else
        {
            p++;
        }
    }
    if( *p == 0 )
        return true; // EOF
    return false;
}

const char* parser_c::getToken( str& out, const char* stopSet )
{
    if( skipToNextToken() )
    {
        printf( "parser_c::getToken: EOF reached\n" );
        out.clear();
        return 0;
    }
    const char* start, *end;
    if( *p == '"' )
    {
        p++;
        start = p;
        while( *p != '"' )
        {
            if( *p == 0 )
            {
                printf( "parser_c::getToken: unexpected end of file hit in quoted string\n" );
                break;
            }
            p++;
        }
        end = p;
        if( *p != 0 )
        {
            p++;
        }
    }
    else
    {
        start = p;
        while( ( G_isWS( *p ) == false ) && ( *p != 0 ) && isCharInCharset( stopSet, *p ) == false )
        {
            p++;
        }
        end = p;
    }
    out.setFromTo( start, end );
    return out;
}
const char* parser_c::getLine( str& out, const char* stopSet )
{
    if( skipToNextToken() )
    {
        printf( "parser_c::getLine: EOF reached\n" );
        out.clear();
        return 0;
    }
    const char* start, *end;
    start = p;
    while( 1 )
    {
        if( *p == '\n' )
            break;
        if( stopSet && isCharInCharset( stopSet, *p ) )
        {
            break;
        }
        if( *p == 0 )
        {
            break;
        }
        if( p[0] == '/' && p[1] == '/' )
        {
            // the rest of line is a comment, so ignore it
            out.setFromTo( start, p );
            while( *p != '\n' )
            {
                if( *p == 0 )
                    break;
                p++;
            }
            return out;
        }
        p++;
    }
    end = p;
    out.setFromTo( start, end );
    return out;
}
const char* parser_c::getD3Token()
{
    if( skipToNextToken() )
    {
        printf( "parser_c::getD3Token: EOF reached\n" );
        return 0;
    }
    if( *p == ',' )
    {
        p++;
        if( skipToNextToken() )
        {
            printf( "parser_c::getD3Token: EOF reached\n" );
            return 0;
        }
    }
    if( *p == '"' )
    {
        p++;
        const char* start = p;
        while( *p != '"' )
        {
            if( *p == 0 )
            {
                break;
            }
            p++;
        }
        lastToken.setFromTo( start, p );
        p++;
        if( *p == ',' )
            p++;
        return lastToken;
    }
    const char* end;
    end = p + 1;
    while( G_isWS( *end ) == false )
    {
        if( *end == ',' )
        {
            // hit special character
            lastToken.setFromTo( p, end );
            p = end;
            p++;
            return lastToken;
        }
        end++;
    }
    lastToken.setFromTo( p, end );
    p = end;
    return lastToken;
}
float parser_c::getFloat()
{
    const char* t = getToken();
    if( t == 0 )
        return 0;
    return atof( t );
}
int parser_c::getInteger()
{
    const char* t = getToken();
    if( t == 0 )
        return 0;
    return atoi( t );
}
bool parser_c::atChar( char ch )
{
    if( skipToNextToken() )
    {
        printf( "parser_c::atChar: EOF reached\n" );
        return false;
    }
    if( *p == ch )
    {
        p++;
        return true;
    }
    return false;
}
bool parser_c::atWord( const char* word )
{
    if( skipToNextToken() )
    {
        printf( "parser_c::atChar: EOF reached\n" );
        return false;
    }
    u32 checkLen = strlen( word );
    if( *p == '"' )
    {
        if( !Q_stricmpn( word, p + 1, checkLen ) && ( p[checkLen + 1] == '"' ) )
        {
            p += checkLen + 1 + 1;
            return true;
        }
    }
    else
    {
        if( !Q_stricmpn( word, p, checkLen ) && ( G_isWS( p[checkLen] ) || p[checkLen] == 0 ) )
        {
            p += checkLen;
            return true;
        }
    }
    return false;
}
bool parser_c::atWord_dontNeedWS( const char* word )
{
    if( skipToNextToken() )
    {
        printf( "parser_c::atChar: EOF reached\n" );
        return false;
    }
    u32 checkLen = strlen( word );
    if( !Q_stricmpn( word, p, checkLen )/* && G_isWS(p[checkLen])*/ )
    {
        p += checkLen;
        return true;
    }
    return false;
}
void parser_c::skipLine()
{
    while( *p )
    {
        if( *p == '\n' )
        {
            break;
        }
        p++;
    }
}
u32 parser_c::getCurrentLineNumber() const
{
    const char* t = base;
    u32 lineNum = 0;
    while( t != p )
    {
        if( *t == '\n' )
        {
            lineNum++;
        }
        t++;
    }
    return lineNum;
    
}