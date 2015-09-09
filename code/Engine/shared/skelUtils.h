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
//  File name:   skelUtils.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Helper functions for skeletal animation
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SKELUTILS_H__
#define __SKELUTILS_H__

#include <math/matrix.h>
#include <shared/array.h>

///
/// shared raw data structs
///

// bone orientation defined by 4x4 matrix and bone name index
struct boneOr_s
{
    matrix_c mat;
    u32 boneName;
    
    void setBlendResult( const boneOr_s& from, const boneOr_s& to, float frac );
    const vec3_c& getOrigin() const
    {
        return mat.getOrigin();
    }
}; // 68 bytes

// array of bone orientations
class boneOrArray_c :  public arraySTD_c<boneOr_s>
{
public:
    // concat bone transforms
    void localBonesToAbsBones( const class boneDefArray_c* boneDefs );
    void setBlendResult( const boneOrArray_c& from, const boneOrArray_c& to, float frac );
    void setBlendResult( const boneOrArray_c& from, const boneOrArray_c& to, float frac, const arraySTD_c<u32>& bonesToBlend );
    void setBones( const boneOrArray_c& from, const arraySTD_c<u32>& bonesToSet );
    u32 findNearestBone( const vec3_c& pos, float* outDist ) const;
    
    void transform( const matrix_c& ofs );
    void scale( float scale );
    void scaleXYZ( const vec3_c& scaleXYZ );
    
    const vec3_c& getBonePos( u32 idx ) const
    {
        return ( *this )[idx].getOrigin();
    }
    const matrix_c& getBoneMat( u32 idx ) const
    {
        return ( *this )[idx].mat;
    }
};

struct boneDef_s
{
    u16 nameIndex;
    short parentIndex;
};
// array of bone definitions (name index + parent index)
class boneDefArray_c :  public arraySTD_c<boneDef_s>
{
public:
    int getLocalBoneIndexForBoneName( u16 boneName ) const
    {
        const boneDef_s* d = getArray();
        for( u32 i = 0; i < size(); i++, d++ )
        {
            if( d->nameIndex == boneName )
                return i;
        }
        return -1;
    }
};

#endif // __SKELUTILS_H__
