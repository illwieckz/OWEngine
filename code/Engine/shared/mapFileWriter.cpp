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
//  File name:   mapFileWriter.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "mapFileWriter.h"
#include "fileStreamHelper.h"
#include <math/vec3.h>
#include <math/quat.h>
#include <api/coreAPI.h>

mapFileWriter_c::mapFileWriter_c()
{
    c_curEntity = 0;
    writingEntity = 0;
    myFile = 0;
    writeStream = 0;
}
mapFileWriter_c::~mapFileWriter_c()
{
    if( myFile )
    {
        delete myFile;
        myFile = 0;
    }
}
// returns true on error
bool mapFileWriter_c::beginWritingMapFile( const char* fname )
{
    myFile = new fileStreamHelper_c;
    if( myFile->beginWriting( fname ) )
    {
        delete myFile;
        myFile = 0;
        return true; // error
    }
    writeStream = myFile;
    return false; // OK
}

void mapFileWriter_c::beginEntity( const char* className )
{
    if( writingEntity )
    {
        g_core->RedWarning( "mapFileWriter_c::beginEntity: already writing entity\n" );
        return;
    }
    writeStream->writeText( "// entity %i\n", c_curEntity );
    writeStream->writeText( "{\n" ); // open entity
    writeStream->writeText( "\"classname\" \"%s\"\n", className );
    writingEntity = true;
}
void mapFileWriter_c::endEntity()
{
    if( writingEntity == false )
    {
        g_core->RedWarning( "mapFileWriter_c::endEntity: already outside entity\n" );
        return;
    }
    writeStream->writeText( "}\n" );
    writingEntity = false;
    c_curEntity ++;
}
// keyValuesListener_i IMPL
// adds a key value to a current entity (this->writingEntity must be true!)
void mapFileWriter_c::addKeyValue( const char* key, const char* value )
{
    writeStream->writeText( "\"%s\" \"%s\"\n", key, value );
}
void mapFileWriter_c::addKeyValue( const char* key, float floatVal )
{
    writeStream->writeText( "\"%s\" \"%f\"\n", key, floatVal );
}
void mapFileWriter_c::addKeyValue( const char* key, const class vec3_c& v3 )
{
    writeStream->writeText( "\"%s\" \"%f %f %f\"\n", key, v3.x, v3.y, v3.z );
}
void mapFileWriter_c::addKeyValue( const char* key, const class quat_c& qXYZW )
{
    writeStream->writeText( "\"%s\" \"%f %f %f %f\"\n", key, qXYZW.x, qXYZW.y, qXYZW.z, qXYZW.w );
}

