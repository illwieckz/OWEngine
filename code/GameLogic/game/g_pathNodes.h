////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2013 V.
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
//  File name:   g_pathNodes.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Node base pathfinding
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __G_PATHNODES_H__
#define __G_PATHNODES_H__

// called from G_Init()
void G_InitPathnodesSystem();

// called from G_Shutdown
void G_ShutdownPathnodesSystem();

// returned path must be fried with 'delete'
class simplePathAPI_i *G_FindPath(const vec3_c &from, const vec3_c &to);

// used to display nodes and their links on local client
void G_DebugDrawPathNodes(class rDebugDrawer_i *dd);

// this is called from InfoPathNode::postSpawn()
class pathNode_c *G_AddPathNode(class InfoPathNode *nodeEntity);

// this is called from InfoPathNode destructor
void G_RemovePathNode(class pathNode_c *pathNode);

#endif // __G_PATHNODES_H__
