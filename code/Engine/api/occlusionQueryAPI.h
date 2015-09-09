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
//  File name:   occlusionQueryAPI.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __OCCLUSIONQUERYAPI_H__
#define __OCCLUSIONQUERYAPI_H__

class occlusionQueryAPI_i
{
	public:
		virtual bool isResultAvailable() const = 0;
		virtual u32 getNumSamplesPassed() const = 0;
		virtual void assignSphereQuery( const class vec3_c& center, float radius ) = 0;
		// you can always use previous test result if the current result is not avaible yet
		virtual u32 getPreviousResult() const = 0;
		virtual u32 waitForLatestResult() const = 0;
		
		virtual ~occlusionQueryAPI_i()
		{
		
		}
};

#endif // __OCCLUSIONQUERYAPI_H__
