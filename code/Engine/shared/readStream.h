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
//  File name:   readStream.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Binary file reader
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SHARED_READSTREAM_H__
#define __SHARED_READSTREAM_H__

#include <api/readStreamAPI.h>

class readStream_c : public readStreamAPI_i
{
    long streamLen;
    void* fileData;
    byte* data;
    u32 ofs;
    
    void freeMemory();
public:
    readStream_c();
    ~readStream_c();
    bool loadFromFile( const char* fname );
    
    virtual bool isAtEOF() const;
    virtual u32 readData( void* out, u32 numBytes );
    virtual bool isAtData( const void* out, u32 numBytes );
    virtual u32 skipBytes( u32 numBytesToSkip );
    virtual const void* getCurDataPtr() const;
    virtual const void* getDataPtr() const;
    virtual u32 pointerToOfs( const void* p ) const;
    virtual u32 getPos() const;
    virtual bool setPos( u32 newPosABS );
    virtual u32 getTotalLen() const;
    virtual void readByteString( class str& out );
};

#endif // __SHARED_READSTREAM_H__

