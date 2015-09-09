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
//  File name:   rf_shadowVolume.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Shadow volume class for stencil shadows
//               (Doom 3 style)
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __RF_SHADOWVOLUME_H__
#define __RF_SHADOWVOLUME_H__

#include "../rIndexBuffer.h"
#include "../rPointBuffer.h"
#include <math/aabb.h>

// NOTE:
// sizeof(vec3_c) == 12,
// sizeof(hashVec3_c) == 16 !!!
class rIndexedShadowVolume_c
{
		rIndexBuffer_c indices;
		rPointBuffer_c points;
		aabb bounds;
		u32 c_edgeQuadsAdded;
		u32 c_capTriPairsAdded;
		bool hasCapsSeparated;
		u32 numIndicesNoCaps;
		vec3_c lightPos;
		
		u32 registerPoint( const vec3_c& p );
	public:
		rIndexedShadowVolume_c();
		~rIndexedShadowVolume_c();
		
		const rIndexBuffer_c& getIndices() const
		{
			return indices;
		}
		const rPointBuffer_c& getPoints() const
		{
			return points;
		}
		
		void clear()
		{
			indices.setNullCount();
			points.setNullCount();
			bounds.clear();
			c_edgeQuadsAdded = 0;
			c_capTriPairsAdded = 0;
			hasCapsSeparated = false;
			numIndicesNoCaps = 0;
		}
		const aabb& getAABB() const
		{
			return bounds;
		}
		
		// before calling this rf_currentEntity and rf_currentLight must be set!
		void addDrawCall();
		
		// shadow volume creation
		void createShadowVolumeForEntity( class rEntityImpl_c* ent, const vec3_c& light, float lightRadius );
		void addIndexedVertexList( const rIndexBuffer_c& ibo, const rVertexBuffer_c& vbo, const vec3_c& light, const class planeArray_c* extraPlanesArray, float lightRadius, const class aabb* bounds = 0 );
		void addIndexedVertexListWithEdges( const rIndexBuffer_c& ibo, const rVertexBuffer_c& vbo, const vec3_c& light, const class planeArray_c* extraPlanesArray, const struct extraSurfEdgesData_s* edges );
		void addRSurface( const class r_surface_c* sf, const vec3_c& light, const struct extraSurfEdgesData_s* edges, float lightRadius );
		void fromRModel( const class r_model_c* m, const vec3_c& light, float lightRadius );
		void fromPrecalculatedStencilShadowCaster( const class r_stencilShadowCaster_c* ssvCaster, const vec3_c& light );
		void addTriangle( const vec3_c& p0, const vec3_c& p1, const vec3_c& p2, const vec3_c& light );
		
		void addFrontCapAndBackCapForTriangle( const vec3_c& p0, const vec3_c& p1, const vec3_c& p2, const vec3_c& light );
		void addFrontCapAndBackCapForIndexedVertsList( const rIndexBuffer_c& ibo, const rVertexBuffer_c& vbo, const vec3_c& light );
		void addEdge( const vec3_c& p0, const vec3_c& p1, const vec3_c& light );
		
		// for directional lights
		void addDirectionalRSurface( const class r_surface_c* sf, const vec3_c& direction, float lightInfinity );
		void addDirectionalIndexedVertexList( const rIndexBuffer_c& ibo, const rVertexBuffer_c& vbo, const vec3_c& lightDir, const class planeArray_c* extraPlanesArray, float lightInfinity, const class aabb* bounds = 0 );
		void createDirectionalShadowVolumeForEntity( class rEntityImpl_c* ent, const vec3_c& lightDirection, float lightInfinity );
		void fromDirectionalRModel( const class r_model_c* m, const vec3_c& lightDirection, float lightInfinity );
		
		u32 getNumVerts() const
		{
			return points.size();
		}
		u32 getNumTris() const
		{
			return indices.getNumTriangles();
		}
		const vec3_c& getLightPos() const
		{
			return lightPos;
		}
};

class rEntityShadowVolume_c
{
		rIndexedShadowVolume_c data;
		class rEntityAPI_i* ent;
	public:
		void addDrawCall();
};

#endif // __RF_SHADOWVOLUME_H__