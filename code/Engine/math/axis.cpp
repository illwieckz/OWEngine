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
//  File name:   axis.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: FLU vectors axis
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

// axis.cpp
#include "axis.h"

const char* axis_c::toString() const
{
	static char buffer[256];
	sprintf( buffer, "%f %f %f %f %f %f %f %f %f",
			 mat[0][0], mat[0][1], mat[0][2],
			 mat[1][0], mat[1][1], mat[1][2],
			 mat[2][0], mat[2][1], mat[2][2] );
	return buffer;
}