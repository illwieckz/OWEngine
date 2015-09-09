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
//  File name:   cmBezierPatch.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Simplified bezier patch class for collision detection
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __RF_BEZIER_H__
#define __RF_BEZIER_H__

#include "array.h"
#include <math/vec3.h>

enum
{
	BP_CONTROL_POINTS_IN_ROW = 3,
	BP_CONTROL_POINTS = BP_CONTROL_POINTS_IN_ROW * BP_CONTROL_POINTS_IN_ROW
};

struct cmBezierPatchControlGroup3x3_s
{
	vec3_c verts[BP_CONTROL_POINTS];
	
	void tesselate( u32 level, class colMeshBuilderAPI_i* out );
};

class cmBezierPatch3x3_c
{
		arraySTD_c<cmBezierPatchControlGroup3x3_s> ctrls3x3;
	public:
		void init( const class cmBezierPatch_c* rp );
		void tesselate( u32 level, class colMeshBuilderAPI_i* out );
};

class cmBezierPatch_c
{
		friend class cmBezierPatch3x3_c;
		arraySTD_c<vec3_c> verts;
		u32 width, height;
		cmBezierPatch3x3_c* as3x3;
	public:
		cmBezierPatch_c();
		~cmBezierPatch_c();
		
		inline void setWidth( int newW )
		{
			this->width = newW;
		}
		inline void setHeight( int newH )
		{
			this->height = newH;
		}
		void addVertex( const vec3_c& nv )
		{
			verts.push_back( nv );
		}
		void tesselate( u32 level, colMeshBuilderAPI_i* out );
		void fromMapBezierPatch( const class mapBezierPatch_c* in );
};
#endif // __RF_BEZIER_H__
