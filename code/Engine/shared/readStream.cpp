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
//  File name:   readStream.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Binary file reader
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "readStream.h"
#include <qcommon/q_shared.h> // REMOVEME
#include <api/vfsAPI.h>
#include <shared/str.h>

readStream_c::readStream_c()
{
	fileData = 0;
	streamLen = 0;
	data = 0;
	ofs = 0;
}
readStream_c::~readStream_c()
{
	freeMemory();
}
void readStream_c::freeMemory()
{
	if ( fileData )
	{
		g_vfs->FS_FreeFile( fileData );
		fileData = 0;
		streamLen = 0;
		ofs = 0;
		data = 0;
	}
}
bool readStream_c::loadFromFile( const char* fname )
{
	// free previous file data
	freeMemory();
	// load new file data
	streamLen = g_vfs->FS_ReadFile( fname, &fileData );
	if ( fileData == 0 )
	{
		return true; // failed to open file
	}
	data = ( byte* )fileData;
	ofs = 0;
	return false; // OK
}
bool readStream_c::isAtEOF() const
{
	return streamLen == ofs;
}
u32 readStream_c::readData( void* out, u32 numBytes )
{
	u32 left = streamLen - ofs;
	u32 read;
	if ( left < numBytes )
	{
		read = left;
	}
	else
	{
		read = numBytes;
	}
	if ( read )
	{
		memcpy( out, data + ofs, read );
		ofs += read;
	}
	return read;
}
bool readStream_c::isAtData( const void* out, u32 numBytes )
{
	u32 left = streamLen - ofs;
	if ( left < numBytes )
		return false;
	if ( memcmp( data + ofs, out, numBytes ) )
	{
		return false;
	}
	data += numBytes; // skip it
	return true;
}
u32 readStream_c::skipBytes( u32 numBytesToSkip )
{
	u32 left = streamLen - ofs;
	u32 skip;
	if ( left < numBytesToSkip )
	{
		skip = left;
	}
	else
	{
		skip = numBytesToSkip;
	}
	ofs += skip;
	return skip;
}
const void* readStream_c::getCurDataPtr() const
{
	if ( streamLen == ofs )
		return 0; // EOF
	return data + ofs;
}
const void* readStream_c::getDataPtr() const
{
	return data;
}
u32 readStream_c::pointerToOfs( const void* p ) const
{
	return ( ( byte* )p ) - data;
}
u32 readStream_c::getPos() const
{
	return ofs;
}
bool readStream_c::setPos( u32 newPosABS )
{
	ofs = newPosABS;
	if ( ofs > streamLen )
		return true;
	return false;
}
u32 readStream_c::getTotalLen() const
{
	return streamLen;
}
void readStream_c::readByteString( str& out )
{
	const byte* start = ( ( const byte* )fileData ) + ofs;
	while ( ( ( const char* )fileData )[ofs] )
	{
		ofs++;
	}
	out.setFromTo( ( const char* )start, ( ( const char* )fileData ) + ofs );
	ofs++;
}
