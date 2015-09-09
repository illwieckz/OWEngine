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
//  File name:   frustumExt.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Extended frustum class
//               With the variable number of planes
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __FRUSTUMEXT_H__
#define __FRUSTUMEXT_H__

#include <shared/array.h>
#include "plane.h"

class frustumExt_c
{
	public:
		arraySTD_c<plane_c> planes;
		
		
		frustumExt_c()
		{
		
		}
		frustumExt_c( const class frustum_c& fr );
		
		enum cullResult_e cull( const class aabb& bb ) const;
		void adjustFrustum( const frustumExt_c& other, const vec3_c& eye, const class cmWinding_c& points, const plane_c& plane );
		void adjustFrustum( const frustumExt_c& other, const vec3_c& eye, const class vec3_c* points, u32 numPoints, const plane_c& plane );
		void fromPointAndWinding( const vec3_c& p, const class cmWinding_c& points, const plane_c& plane );
		
		inline const plane_c&   operator []( const int index ) const
		{
			return planes[index];
		}
		inline plane_c& operator []( const int index )
		{
			return planes[index];
		}
		inline void addPlane( const plane_c& newPlane )
		{
			planes.push_back( newPlane );
		}
		inline void clear()
		{
			planes.clear();
		}
		inline u32 size() const
		{
			return planes.size();
		}
};

#endif // __FRUSTUMEXT_H__
