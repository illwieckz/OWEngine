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
//  File name:   rf_debugDrawIMPL.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Implementation of debug drawer
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include <api/ddAPI.h>
#include <api/rbAPI.h>
#include <math/vec3.h>

class rDebugDrawerIMPL_c : public rDebugDrawer_i
{
public:
    virtual void setColor( float r, float g, float b, float a )
    {
        float rgba[] = { r, g, b, a };
        rb->setColor4( rgba );
    }
    virtual void drawCapsuleZ( const float* xyz, float h, float w )
    {
        rb->unbindMaterial();
        rb->drawCapsuleZ( xyz, h, w );
    }
    virtual void drawBBExts( const float* xyz, const float* angles, const float* halfSizes )
    {
        rb->setupEntitySpace2( angles, xyz );
        rb->drawBoxHalfSizes( halfSizes );
    }
    virtual void drawLineFromTo( const float* from, const float* to, const float* colorRGB, float lineWidth )
    {
        rb->setupWorldSpace();
        rb->drawLineFromTo( from, to, colorRGB, lineWidth );
    }
    virtual void drawBBLines( const class aabb& bb )
    {
        rb->setupWorldSpace();
        rb->drawBBLines( bb );
    }
};

static rDebugDrawerIMPL_c r_debugDrawer;
extern rDebugDrawer_i* r_dd = &r_debugDrawer;