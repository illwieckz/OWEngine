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
//  File name:   rf_bezier.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Bezier patch class
//               Used for both .BSP file patches and .MAP file patches
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __RF_BEZIER_H__
#define __RF_BEZIER_H__

#include "rf_surface.h"

enum
{
    BP_CONTROL_POINTS_IN_ROW = 3,
    BP_CONTROL_POINTS = BP_CONTROL_POINTS_IN_ROW * BP_CONTROL_POINTS_IN_ROW
};

struct bezierPatchControlGroup3x3_s
{
    //union {
    rVert_c verts[BP_CONTROL_POINTS];
    //	rVert_c verts3x3[BP_CONTROL_POINTS_IN_ROW][BP_CONTROL_POINTS_IN_ROW];
    //};
    
    void tesselate( u32 level, class r_surface_c* out );
};

class bezierPatch3x3_c
{
//friend class r_bezierPatch_c;
    arraySTD_c<bezierPatchControlGroup3x3_s> ctrls3x3;
public:
    void init( const class r_bezierPatch_c* rp );
    void tesselate( u32 level, class r_surface_c* out );
};

class r_bezierPatch_c
{
    friend class bezierPatch3x3_c;
    arraySTD_c<rVert_c> verts;
    u32 width, height;
    class mtrAPI_i* mat;
    class textureAPI_i* lightmap;
    bezierPatch3x3_c* as3x3;
    r_surface_c* sf;
    
    void calcNormals();
public:
    r_bezierPatch_c();
    ~r_bezierPatch_c();
    
    u32 getNumTris() const
    {
        if( sf == 0 )
            return 0;
        return sf->getNumTris();
    }
    void setMaterial( const char* matName );
    
    inline void setMaterial( class mtrAPI_i* newMat )
    {
        mat = newMat;
    }
    inline void setLightmap( class textureAPI_i* newLM )
    {
        lightmap = newLM;
    }
    inline void setWidth( int newW )
    {
        this->width = newW;
    }
    inline void setHeight( int newH )
    {
        this->height = newH;
    }
    void addVertex( const rVert_c& nv )
    {
        verts.push_back( nv );
    }
    const r_surface_c* getInstancePtr() const
    {
        return sf;
    }
    mtrAPI_i* getMat() const
    {
        return mat;
    }
    void tesselate( u32 newLevel );
    void addDrawCall();
    bool traceRay( class trace_c& tr );
    const aabb& getBB() const;
    // load .map file bezier patch definition
    bool fromMapBezierPatch( const class mapBezierPatch_c* p );
    bool fromString( const char* pDefStart, const char* pDefEnd );
};
#endif // __RF_BEZIER_H__
