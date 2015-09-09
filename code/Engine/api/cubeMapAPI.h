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
//  File name:   cubemapAPI.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: cubemap class interface
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __CUBEMAPAPI_H__
#define __CUBEMAPAPI_H__

#include <shared/typedefs.h>

class cubeMapAPI_i
{
	public:
		virtual ~cubeMapAPI_i()
		{
		
		}
		
		// returns the cubemap name
		virtual const char* getName() const = 0;
		
		virtual void* getInternalHandleV() const = 0;
		virtual void setInternalHandleV( void* newHandle ) = 0;
		virtual u32 getInternalHandleU32() const = 0;
		virtual void setInternalHandleU32( u32 newHandle ) = 0;
};

#endif // __CUBEMAPAPI_H__

