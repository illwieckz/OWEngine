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
//  File name:   entDefsList.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: List of entity definitions (keyValues + primitives)
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "entDefsList.h"
#include "../qcommon/q_shared.h"
#include <api/vfsAPI.h>
#include <api/coreAPI.h>
#include <fileformats/bspFileFormat.h>
#include <shared/parser.h>
#include <shared/str.h>

entDefsList_c::entDefsList_c()
{

}
entDefsList_c::~entDefsList_c()
{
    clear();
}
void entDefsList_c::clear()
{
    for( u32 i = 0; i < entities.size(); i++ )
    {
        delete entities[i];
        entities[i] = 0;
    }
    entities.clear();
}
bool entDefsList_c::fromString( const char* text )
{
    parser_c p;
    p.setup( text );
    
    // Doom 3 / Quake 4 NOTE:
    // There are no entities data in .proc files.
    // Entities are loaded from .map files.
    if( p.atWord( "mapProcFile" ) )
    {
        g_core->RedWarning( "entDefsList_c::fromString: cannot load entities string from .proc files.\n" );
        return true;
    }
    
    int mapFileVersion = -1;
    
    // only for Doom3/Quake4 .map files - skip version ident
    if( p.atWord( "Version" ) )
    {
        mapFileVersion = p.getInteger();
    }
    u32 curModNum = 0;
    while( p.atEOF() == false )
    {
        if( p.atChar( '{' ) )
        {
            entDef_c* ed = new entDef_c;
            this->entities.push_back( ed );
            bool bHadMapPrimitives = false;
            while( p.atChar( '}' ) == false )
            {
                if( p.atEOF() )
                {
                
                    break;
                }
                if( p.atChar( '{' ) )
                {
                    p.skipCurlyBracedBlock( false );
                    bHadMapPrimitives = true;
                }
                else
                {
                    str key = p.getToken();
                    str val = p.getToken();
                    ed->setKeyValue( key, val );
                }
            }
            if( bHadMapPrimitives )
            {
                if( curModNum )
                {
                    // automatically set model field for direct
                    // .map files loading, but dont override
                    // Doom3 func_static names (they are used with .proc files)
                    if( ed->hasKey( "model" ) == false )
                    {
                        ed->setKeyValue( "model", va( "*%i", curModNum ) );
                    }
                }
                curModNum++;
            }
        }
        else
        {
            p.getToken();
        }
    }
    return false; // OK
}
bool entDefsList_c::loadEntitiesFromBSPFile( const char* mapName )
{
    char buf[512];
    strcpy( buf, mapName );
    fileHandle_t f;
    int len = g_vfs->FS_FOpenFile( buf, &f, FS_READ );
    if( f == 0 )
    {
        strcpy( buf, "maps/" );
        strcat( buf, mapName );
        strcat( buf, ".bsp" );
        len = g_vfs->FS_FOpenFile( buf, &f, FS_READ );
        if( f == 0 )
        {
            return true;
        }
    }
    byte* data = ( byte* )malloc( len );
    g_vfs->FS_Read( data, len, f );
    g_vfs->FS_FCloseFile( f );
    
    const char* text;
    
    q3Header_s* h = ( q3Header_s* )data;
    h->swapCoDLumpLenOfsValues();
    
    if( h->ident == BSP_IDENT_IBSP )
    {
        if( h->version == BSP_VERSION_COD1 )
        {
            // NOTE: call of duty bsps has IBSP ident
            text = ( const char* )h->getLumpData( COD1_ENTITIES );
        }
        else
        {
            text = ( const char* )h->getLumpData( Q3_ENTITIES );
        }
    }
    else if( h->ident == BSP_IDENT_2015 || h->ident == BSP_IDENT_EALA )
    {
        text = ( const char* )h->getLumpData( MOH_ENTITIES );
    }
    else if( h->ident == BSP_VERSION_HL )
    {
        text = ( const char* )h->getLumpData( HL_ENTITIES );
    }
    else if( h->ident == BSP_IDENT_VBSP )
    {
        text = ( const char* )h->getLumpData( SRC_ENTITIES );
    }
    else if( h->ident == BSP_IDENT_QIOBSP )
    {
        text = ( const char* )h->getLumpData( Q3_ENTITIES );
    }
    else
    {
        g_core->RedWarning( "entDefsList_c::load: unknown bsp type\n" );
        text = 0;
        free( data );
        return true; // error
    }
    bool error = this->fromString( text );
    free( data );
    return error;
}
bool entDefsList_c::loadEntitiesFromMapFile( const char* mapName )
{
    char buf[512];
    strcpy( buf, mapName );
    fileHandle_t f;
    int len = g_vfs->FS_FOpenFile( buf, &f, FS_READ );
    if( f == 0 )
    {
        strcpy( buf, "maps/" );
        strcat( buf, mapName );
        strcat( buf, ".map" );
        len = g_vfs->FS_FOpenFile( buf, &f, FS_READ );
        if( f == 0 )
        {
            return true;
        }
    }
    byte* data = ( byte* )malloc( len );
    g_vfs->FS_Read( data, len, f );
    g_vfs->FS_FCloseFile( f );
    const char* text = ( const char* )data;
    bool error = this->fromString( text );
    free( data );
    return error;
}
bool entDefsList_c::loadEntitiesFromEntitiesFile( const char* mapName )
{
    char buf[512];
    strcpy( buf, "maps/" );
    strcat( buf, mapName );
    strcat( buf, ".entities" );
    parser_c p;
    if( p.openFile( buf ) )
    {
        strcpy( buf, "maps/" );
        strcat( buf, mapName );
        strcat( buf, ".ent" );
        if( p.openFile( buf ) )
        {
            return true;
        }
    }
    if( p.atWord( "Version" ) )
    {
        p.getInteger();
    }
    while( p.atEOF() == false )
    {
        if( p.atWord( "{" ) )
        {
            entDef_c* ed = new entDef_c;
            this->entities.push_back( ed );
            while( p.atWord( "}" ) == false )
            {
                if( p.atEOF() )
                {
                    g_core->RedWarning( "entDefsList_c::loadEntitiesFromEntitiesFile: unexpected EOF hit while parsing entity %i\n", entities.size() );
                    break;
                }
                str key, val;
                key = p.getToken();
                val = p.getToken();
                ed->setKeyValue( key, val );
            }
        }
        else
        {
            int line = p.getCurrentLineNumber();
            const char* tok = p.getToken();
            g_core->RedWarning( "entDefsList_c::loadEntitiesFromEntitiesFile: unknown token \"%s\" at line %i of %s\n", tok, line, buf );
        }
    }
    return false;
}
bool entDefsList_c::load( const char* mapName )
{
    if( loadEntitiesFromBSPFile( mapName ) == false )
    {
        return false;
    }
    if( loadEntitiesFromMapFile( mapName ) == false )
    {
        return false;
    }
    if( loadEntitiesFromEntitiesFile( mapName ) == false )
    {
        return false;
    }
    g_core->RedWarning( "Cannot load entity list for map %s\n", mapName );
    return true;
}





