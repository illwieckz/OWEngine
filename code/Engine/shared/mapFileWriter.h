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
//  File name:   mapFileWriter.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Helper class for writting .MAP files to disk
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __MAPFILEWRITER_H__
#define __MAPFILEWRITER_H__

#include "typedefs.h"
#include "keyValuesListener.h"

class mapFileWriter_c : public keyValuesListener_i
{
    u32 c_curEntity;
    bool writingEntity;
    class fileStreamHelper_c* myFile;
    class writeStreamAPI_i* writeStream;
public:
    mapFileWriter_c();
    ~mapFileWriter_c();
    
    // returns true on error
    bool beginWritingMapFile( const char* fname );
    
    void beginEntity( const char* className );
    void endEntity();
    
    // keyValuesListener_i IMPL
    // adds a key value to a current entity (this->writingEntity must be true!)
    virtual void addKeyValue( const char* key, const char* value );
    virtual void addKeyValue( const char* key, float floatVal );
    virtual void addKeyValue( const char* key, const class vec3_c& v3 );
    virtual void addKeyValue( const char* key, const class quat_c& qXYZW );
};

#endif // __FILESTREAMHELPER_H__
