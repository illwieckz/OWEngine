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
//  File name:   cg_collision.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Clientside-only collision detection system.
//               Usefull for 3rd person camera clipping, local entities and particles.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "cg_local.h"
#include <api/rAPI.h>
#include <api/rEntityAPI.h>
#include <shared/trace.h>

bool CG_RayTrace( class trace_c& tr, u32 skipEntNum )
{
	if ( rf->rayTraceWorldMap( tr ) )
	{
		tr.setHitCGEntity( &cg_entities[ENTITYNUM_WORLD] );
	}
	for ( u32 i = 0; i < MAX_GENTITIES; i++ )
	{
		centity_s* cent = &cg_entities[i];
		if ( cent->currentValid == false )
		{
			continue;
		}
		if ( cent->rEnt == 0 )
		{
			continue;
		}
		if ( skipEntNum != ENTITYNUM_NONE )
		{
			if ( i == skipEntNum )
				continue;
			if ( cent->currentState.parentNum == skipEntNum )
				continue;
		}
		const aabb& bb = cent->rEnt->getBoundsABS();
		if ( tr.getTraceBounds().intersect( bb ) == false )
		{
			continue;
		}
		if ( cent->rEnt->rayTraceWorld( tr ) )
		{
			tr.setHitCGEntity( cent );
		}
	}
	return tr.hasHit();
}





