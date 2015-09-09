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
//  File name:   vec3.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 3D vector class
//               Used for pitch-yaw-roll Euler angles as well
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "vec3.h"

void vec3_c::normalizeFast()
{
    float lengthSQ = x * x + y * y + z * z;
    if( lengthSQ )
    {
        float iLength = G_rsqrt( lengthSQ );
        x *= iLength;
        y *= iLength;
        z *= iLength;
    }
}
void vec3_c::normalize()
{
#if 1
    float lengthSQ = x * x + y * y + z * z;
    if( lengthSQ )
    {
        float iLength = 1.f / sqrt( lengthSQ );
        x *= iLength;
        y *= iLength;
        z *= iLength;
    }
#elif 0
    float lengthSQ = x * x + y * y + z * z;
    if( lengthSQ )
    {
        //float iLength = G_rsqrt(lengthSQ);
        float iLength = G_rsqrt3( lengthSQ );
        x *= iLength;
        y *= iLength;
        z *= iLength;
    }
#else
    float data[4] = { x, y, z, 0 };
    __asm
    {
        movups xmm0, data
        movaps xmm2, xmm0
        mulps xmm0, xmm0
        movaps xmm1, xmm0
        shufps xmm0, xmm1, 0x4e
        addps xmm0, xmm1
        movaps xmm1, xmm0
        shufps xmm1, xmm1, 0x11
        addps xmm0, xmm1
        rsqrtps xmm0, xmm0
        mulps xmm2, xmm0
        movups data, xmm2
    }
    x = data[0];
    y = data[1];
    z = data[2];
#endif
}


