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
//  File name:   modelPostProcessFuncs.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: functions used by model postprocessing code
//               (scaling, rotating, etc...)
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __MODELPOSTPROCESSFUNCS_H__
#define __MODELPOSTPROCESSFUNCS_H__

#include <shared/typedefs.h>

class modelPostProcessFuncs_i
{
	public:
		virtual void scaleXYZ( float scale ) = 0;
		virtual void swapYZ() = 0;
		virtual void swapIndexes() = 0;
		virtual void translateY( float ofs ) = 0;
		virtual void multTexCoordsY( float f ) = 0;
		virtual void multTexCoordsXY( float f ) = 0;
		virtual void translateXYZ( const class vec3_c& ofs ) = 0;
		virtual void transform( const class matrix_c& mat ) = 0;
		virtual void getCurrentBounds( class aabb& out ) = 0;
		virtual void setAllSurfsMaterial( const char* newMatName ) = 0;
		virtual u32 getNumSurfs() const = 0;
		virtual void setSurfsMaterial( const u32* surfIndexes, u32 numSurfIndexes, const char* newMatName ) = 0;
		virtual void recalcBoundingBoxes() = 0;
		
		// md3 style tags (used for attaching objects/emitters to model)
		virtual void addAbsTag( const char* newTagName, const class vec3_c& newPos, const class vec3_c& newAngles ) { };
		
		// optional, per-surface functions
		virtual bool hasPerSurfaceFunctionsImplemented() const
		{
			return false;
		}
		virtual void setNumSurfs( u32 newSurfsCount )
		{
		
		}
		virtual void resizeSurfaceVerts( u32 surfNum, u32 numVerts )
		{
		
		}
		virtual void setSurfaceVert( u32 surfNum, u32 vertIndex, const float* xyz, const float* st )
		{
		
		}
		virtual void setSurfaceIndicesU32( u32 surfNum, u32 numIndices, const u32* indices )
		{
		
		}
		virtual void setSurfaceMaterial( u32 surfNum, const char* material )
		{
		
		}
};

#endif // __MODELPOSTPROCESSFUNCS_H__
