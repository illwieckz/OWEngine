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
//  File name:   InfoPathNode.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Waypoint for AI navigation
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "InfoPathNode.h"
#include <math/aabb.h>
#include <shared/trace.h>
#include <api/coreAPI.h>
#include <api/ddAPI.h>
#include <api/rAPI.h>
#include <api/cmAPI.h>
#include <api/simplePathAPI.h>
#include "../g_local.h"
#include "../g_pathNodes.h"
#include <shared/entityType.h>

DEFINE_CLASS( InfoPathNode, "BaseEntity" );
DEFINE_CLASS_ALIAS( InfoPathNode, info_pathnode );

InfoPathNode::InfoPathNode()
{
	pathNode = 0;
	this->setEntityType( ET_PATHNODE );
}
InfoPathNode::~InfoPathNode()
{
	if ( pathNode )
	{
		G_RemovePathNode( pathNode );
	}
}

void InfoPathNode::postSpawn()
{
	pathNode = G_AddPathNode( this );
}

