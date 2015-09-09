////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2013 V.
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
//  File name:   hl2MDLReader.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __HL2_MDL_READER_H__
#define __HL2_MDL_READER_H__

#include <shared/array.h>
#include <shared/str.h>
#include <shared/readStream.h>

class hl2MDLReader_c
{
		str name;
		// .mdl file data
		readStream_c data;
		long fileLen;
		u32 version;
		arraySTD_c<str> matNames;
		// vvd file data
		long vvdFileLen;
		struct vvd4FileHeader_s* vvd;
		
		bool readMatNames();
	public:
		hl2MDLReader_c();
		~hl2MDLReader_c();
		bool beginReading( const char* fname );
		
		bool getStaticModelData( class staticModelCreatorAPI_i* out );
		
		u32 getNumMaterials() const;
		const char* getMatName( u32 matIndex ) const;
};

#endif // __HL2_MDL_READER_H__
