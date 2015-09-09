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
//  File name:   r2dVert.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Simple vertex struct for 2D grahpics (UI)
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __R2DVERT_H__
#define __R2DVERT_H__

#include <math/vec3.h>
#include <math/vec2.h>

struct r2dVert_s
{
    //vec2_c pos;
    vec3_c pos; // changed to vec3 for DX9
    vec2_c texCoords;
    
    void set( float nX, float nY, float nS, float nT )
    {
        pos.x = nX;
        pos.y = nY;
        texCoords.set( nS, nT );
    }
};

#endif // __R2DVERT_H__
