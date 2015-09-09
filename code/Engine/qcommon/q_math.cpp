////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 1999-2005 Id Software, Inc.
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
//  File name:   q_math.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "q_shared.h"

//============================================================================

/*
** float q_rsqrt( float number )
*/
float Q_rsqrt( float number )
{
	floatInt_u t;
	float x2, y;
	const float threehalfs = 1.5F;
	
	x2 = number * 0.5F;
	t.f  = number;
	t.i  = 0x5f3759df - ( t.i >> 1 );               // what the fuck?
	y  = t.f;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	//  y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
	
	return y;
}
//============================================================

/*
===============
LerpAngle

===============
*/
float LerpAngle( float from, float to, float frac )
{
	float   a;
	
	if ( to - from > 180 )
	{
		to -= 360;
	}
	if ( to - from < -180 )
	{
		to += 360;
	}
	a = from + frac * ( to - from );
	
	return a;
}


/*
=================
AngleNormalize360

returns angle normalized to the range [0 <= angle < 360]
=================
*/
float AngleNormalize360( float angle )
{
	return ( 360.0 / 65536 ) * ( ( int )( angle * ( 65536 / 360.0 ) ) & 65535 );
}

