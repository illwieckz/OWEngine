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
//  File name:   cm_world.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "cm_local.h"
#include <api/cmAPI.h>
#include <api/vfsAPI.h>
#include <shared/str.h>

cMod_i* cm_worldModel = 0;

bool CM_LoadWorldMap( const char* mapName )
{
    cm_worldModel = 0;
    
    str path = "maps/";
    path.append( mapName );
    path.setExtension( "bsp" );
    if( g_vfs->FS_FileExists( path ) )
    {
        // not needed now
        return false;
    }
    path.setExtension( "proc" );
    if( g_vfs->FS_FileExists( path ) )
    {
        // not needed now
        return false;
    }
    path.setExtension( "map" );
    if( g_vfs->FS_FileExists( path ) )
    {
        cm_worldModel = CM_RegisterModel( path );
        return false;
    }
    return true; // error
}
bool CM_TraceWorldRay( class trace_c& tr )
{
    if( cm_worldModel )
        return cm_worldModel->traceRay( tr );
    return false; // no hit
}
bool CM_TraceWorldSphere( class trace_c& tr )
{
    if( cm_worldModel )
        return cm_worldModel->traceSphere( tr );
    return false; // no hit
}
bool CM_TraceWorldAABB( class trace_c& tr )
{
    if( cm_worldModel )
        return cm_worldModel->traceAABB( tr );
    return false; // no hit
}
cMod_i* CM_GetWorldModel()
{
    return cm_worldModel;
}

cMod_i* CM_GetWorldSubModel( unsigned int subModelIndex )
{
    if( cm_worldModel == 0 )
        return 0;
    return cm_worldModel->getSubModel( subModelIndex );
}

