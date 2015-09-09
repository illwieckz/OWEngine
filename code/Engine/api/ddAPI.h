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
//  File name:   ddAPI.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __DD_API_H__
#define __DD_API_H__

#include "iFaceBase.h"

#define RENDERER_API_IDENTSTR "RendererAPI0001"

class rDebugDrawer_i : public iFaceBase_i
{
public:
    virtual void setColor( float r, float g, float b, float a ) = 0;
    virtual void drawCapsuleZ( const float* xyz, float h, float w ) = 0;
    virtual void drawBBExts( const float* xyz, const float* angles, const float* halfSizes ) = 0;
    virtual void drawLineFromTo( const float* from, const float* to, const float* colorRGB, float lineWidth = 1.f ) = 0;
    virtual void drawBBLines( const class aabb& bb ) = 0;
};

extern rDebugDrawer_i* r_dd;

#endif // __DD_API_H__
