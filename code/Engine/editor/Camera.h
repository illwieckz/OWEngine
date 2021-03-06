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
//  File name:   Camera.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Window system independent camera view code
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

enum camera_draw_mode
{
	cd_wire,
	cd_solid,
	cd_texture,
	cd_light,
	cd_blend
};

struct camera_t
{
	int     width, height;
	
	bool    timing;
	
	edVec3_c    origin;
	edVec3_c    angles;
	
	camera_draw_mode    draw_mode;
	
	vec3_t  color;          // background
	
	vec3_t  forward, right, up; // move matrix
	
	edVec3_c    vup, vpn, vright;   // view matrix
};

