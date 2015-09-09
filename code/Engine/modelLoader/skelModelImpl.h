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
//  File name:   skelModelImpl.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: implementation of skeletal model API
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SKELMODELIMPL_H__
#define __SKELMODELIMPL_H__

#include "sk_local.h"
#include <api/skelModelAPI.h>
#include <api/modelPostProcessFuncs.h>
#include <shared/array.h>
#include <shared/str.h>

class skelSurfIMPL_c : public skelSurfaceAPI_i
{
		friend class skelModelIMPL_c;
		str matName;
		str surfName;
		arraySTD_c<skelWeight_s> weights;
		arraySTD_c<skelVert_s> verts;
		arraySTD_c<u16> indices;
		struct extraSurfEdgesData_s* edgesData;
		
		virtual const char* getMatName() const
		{
			return matName;
		}
		virtual const char* getSurfName() const
		{
			return surfName;
		}
		virtual u32 getNumVerts() const
		{
			return verts.size();
		}
		virtual u32 getNumWeights() const
		{
			return weights.size();
		}
		virtual u32 getNumIndices() const
		{
			return indices.size();
		}
		virtual u32 getNumTriangles() const
		{
			return indices.size() / 3;
		}
		// raw data access
		virtual const skelVert_s* getVerts() const
		{
			return verts.getArray();
		}
		virtual const skelWeight_s* getWeights() const
		{
			return weights.getArray();
		}
		virtual const u16* getIndices() const
		{
			return indices.getArray();
		}
		virtual const struct extraSurfEdgesData_s* getEdgesData() const
		{
			return edgesData;
		}
		
		void scaleXYZ( float scale )
		{
			skelWeight_s* w = weights.getArray();
			for ( u32 i = 0; i < weights.size(); i++, w++ )
			{
				w->ofs.scale( scale );
			}
		}
		void swapIndexes()
		{
			u16* indices16 = indices.getArray();
			for ( u32 i = 0; i < indices.size(); i += 3 )
			{
				u16 tmp16 = indices16[i];
				indices16[i] = indices16[i + 3];
				indices16[i + 3] = tmp16;
			}
		}
		void setMaterial( const char* newMatName )
		{
			matName = newMatName;
		}
		bool compareWeights( u32 wi0, u32 wi1 ) const;
		bool compareVertexWeights( u32 v0, u32 v1 ) const;
		void calcEqualPointsMapping( arraySTD_c<u16>& mapping );
		void calcEdges();
	public:
		skelSurfIMPL_c();
		~skelSurfIMPL_c();
};
class skelModelIMPL_c : public skelModelAPI_i, public modelPostProcessFuncs_i
{
		str name;
		arraySTD_c<skelSurfIMPL_c> surfs;
		boneDefArray_c bones;
		boneOrArray_c baseFrameABS;
		vec3_c curScale;
		
		// skelModelAPI_i impl
		virtual u32 getNumSurfs() const
		{
			return surfs.size();
		}
		virtual u32 getNumBones() const
		{
			return bones.size();
		}
		virtual u32 getTotalTriangleCount() const
		{
			u32 ret = 0;
			for ( u32 i = 0; i < surfs.size(); i++ )
			{
				ret += surfs[i].getNumTriangles();
			}
			return ret;
		}
		virtual const skelSurfaceAPI_i* getSurface( u32 surfNum ) const
		{
			return &surfs[surfNum];
		}
		virtual const boneOrArray_c& getBaseFrameABS() const
		{
			return baseFrameABS;
		}
		virtual bool hasCustomScaling() const
		{
			if ( curScale.compare( vec3_c( 1.f, 1.f, 1.f ) ) )
				return false;
			return true;
		}
		virtual const vec3_c& getScaleXYZ() const
		{
			return curScale;
		}
		virtual int getLocalBoneIndexForBoneName( const char* nameStr ) const
		{
			u16 nameIndex = SK_RegisterString( nameStr );
			return bones.getLocalBoneIndexForBoneName( nameIndex );
		}
		virtual const char* getBoneName( u32 boneIndex ) const
		{
			return SK_GetString( bones[boneIndex].nameIndex );
		}
		virtual void printBoneNames() const;
		// modelPostProcessFuncs_i impl
		virtual void scaleXYZ( float scale );
		virtual void swapYZ();
		virtual void swapIndexes();
		virtual void translateY( float ofs );
		virtual void multTexCoordsY( float f );
		virtual void multTexCoordsXY( float f );
		virtual void translateXYZ( const class vec3_c& ofs );
		virtual void transform( const class matrix_c& mat );
		virtual void getCurrentBounds( class aabb& out );
		virtual void setAllSurfsMaterial( const char* newMatName );
		virtual void setSurfsMaterial( const u32* surfIndexes, u32 numSurfIndexes, const char* newMatName );
		virtual void recalcBoundingBoxes()
		{
			// TODO?
		}
	public:
		skelModelIMPL_c();
		~skelModelIMPL_c();
		
		bool loadMD5Mesh( const char* fname );
		void recalcEdges();
};

#endif // __SKELMODELIMPL_H__
