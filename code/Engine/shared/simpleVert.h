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
//  File name:   simpleVert.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SIMPLEVERT_H__
#define __SIMPLEVERT_H__

#include <math/vec3.h>
#include <math/vec2.h>

struct simpleVert_s
{
	vec3_c xyz;
	vec2_c tc;
	
	void setXYZ( const double* d )
	{
		xyz[0] = d[0];
		xyz[1] = d[1];
		xyz[2] = d[2];
	}
	void setUV( const double* d )
	{
		tc[0] = d[0];
		tc[1] = d[1];
	}
	void setXYZ( const float* f )
	{
		xyz[0] = f[0];
		xyz[1] = f[1];
		xyz[2] = f[2];
	}
	void setXYZ( float nX, float nY, float nZ )
	{
		xyz[0] = nX;
		xyz[1] = nY;
		xyz[2] = nZ;
	}
	void setUV( const float* f )
	{
		tc[0] = f[0];
		tc[1] = f[1];
	}
	void setUV( float u, float v )
	{
		tc[0] = u;
		tc[1] = v;
	}
};

#endif // __SIMPLEVERT_H__
