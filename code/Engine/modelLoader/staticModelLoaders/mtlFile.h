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
//  File name:   mtlFile.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Simple WaveFront (.OBJ, .MTL) file reader
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __MTLFILE_H__
#define __MTLFILE_H__

#include <shared/str.h>
#include <shared/array.h>

struct mtlEntry_s
{
	str name;
	str map_Ka;
	str map_Kd;
	str map_bump;
	str bump;
	str map_refl;
	str map_d;
};

class mtlFile_c
{
		str name;
		arraySTD_c<mtlEntry_s> entries;
	public:
		bool loadMTL( const char* fileName );
		
		const mtlEntry_s* findEntry( const char* mtlName ) const;
};

#endif // __MTLFILE_H__

