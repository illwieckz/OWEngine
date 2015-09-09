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
//  File name:   cm_helper.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __CM_HELPER_H__
#define __CM_HELPER_H__

#include <api/cmApi.h>
#include <shared/ePairsList.h>

class cmHelper_c : public cmHelper_i
{
		ePairList_c keyValues;
		class cmCompound_c* myCompound;
	public:
		cmHelper_c();
		~cmHelper_c();
		void setKeyPairs( const ePairList_c& newList )
		{
			this->keyValues = newList;
		}
		const ePairList_c& getEPairs() const
		{
			return this->keyValues;
		}
		
		// that function will alloc this->compound if its not present
		class cmCompound_c* registerCompound();
		bool hasCompoundModel() const;
		class cmCompound_c* getCompound() const;
		class cmCompound_i* getCompoundAPI() const;
		
		bool hasClassName( const char* classNameToCheck ) const
		{
			return this->keyValues.hasKeyValue( "classname", classNameToCheck );
		}
		
		virtual u32 getNumKeyPairs() const
		{
			return keyValues.size();
		}
		virtual const char* getKeyValue( const char* key ) const
		{
			return keyValues.getKeyValue( key );
		}
		virtual bool getKeyVec3( const char* key, class vec3_c& out ) const
		{
			return keyValues.getKeyVec3( key, out );
		}
};

#endif // __CM_HELPER_H__
