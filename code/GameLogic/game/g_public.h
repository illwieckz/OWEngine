////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 1999-2005 Id Software, Inc.
//  Copyright (C) 2012-2014 V.
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
//  File name:   g_public.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Game module information visible to server
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __G_PUBLIC_H__
#define __G_PUBLIC_H__

#include <math/vec3.h>
#include <math/aabb.h>

// DO NOT MODIFY THIS STRUCT
// (unless you're able to rebuild both server and game)
struct edict_s {
	entityState_s *s;	// communicated by server to clients; this is non-zero only for active entities
	int freetime;	// level.time when the object was freed
	// entity class for game-only usage
	class BaseEntity *ent;

	// for serverside entity culling (BSP PVS)
	aabb absBounds;
	struct bspBoxDesc_s *bspBoxDesc;
};

////===============================================================
//
#endif // __G_PUBLIC_H__
