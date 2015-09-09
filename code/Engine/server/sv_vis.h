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
//  File name:   sv_vis.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Server side visibility checks (BSP PVS)
//               used to cull entities before sending the to players
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SV_VIS_H__
#define __SV_VIS_H__

#include <fileFormats/bspFileFormat.h>
#include <shared/array.h>
#include <shared/bspBoxDesc.h>

// NOTE: the order of fields in svBSPLeaf_s
// and svBSPNode_s must be the same as the order
// in q3Node_s and q3Leaf_s from bspFileFormat.h
struct svBSPLeaf_s
{
    int cluster;
    int area;
};
struct svBSPNode_s
{
    u32 planeNum;
    int children[2];
};
struct svPortal_s
{
    // TODO
    //int areas[2];
};
struct svBSPArea_s
{
    svBSPArea_s()
    {
        floodNum = 0;
        floodValid = 0;
    }
    int floodNum;
    int floodValid;
};
// BSP data needed by server to cull entities before sending them to players.
// (leaves, nodes, planes, areas and vis)
class svBSP_c
{
    const struct q3Header_s* h; // used only while loading
    // BSP data
    arraySTD_c<svBSPLeaf_s> leaves;
    arraySTD_c<svBSPNode_s> nodes;
    arraySTD_c<q3Plane_s> planes;
    arraySTD_c<svBSPArea_s> areas;
    struct visHeader_s* vis;
    // Qio BSP data (extra areaportals data)
    arraySTD_c<vec3_c> points;
    arraySTD_c<dareaPortal_t> areaPortals;
    
    int floodValid;
    int* areaPortalStates;
    
    bool loadPlanes( u32 lumpPlanes );
    bool loadNodesAndLeaves( u32 lumpNodes, u32 lumpLeaves, u32 sizeOfLeaf );
    bool loadVisibility( u32 visLump );
    bool loadQioPortals( u32 lumpNum );
    bool loadQioPoints( u32 lumpNum );
    
    void filterBB_r( const class aabb& bb, struct bspBoxDesc_s& out, int nodeNum ) const;
    
    void floodArea_r( int areaNum, int floodNum );
    void floodAreaConnections();
    bool areasConnected( int area0, int area1 ) const;
public:
    svBSP_c();
    ~svBSP_c();
    void clear();
    bool load( const char* mapName );
    
    void filterBB( const class aabb& bb, struct bspBoxDesc_s& out ) const;
    void filterPoint( const class vec3_c& p, struct bspPointDesc_s& out ) const;
    bool checkVisibility( struct bspPointDesc_s& p, const struct bspBoxDesc_s& box ) const;
    void adjustAreaPortalState( int area0, int area1, bool open );
    u32 getNumAreaBytes() const;
    void appendCurrentAreaBits( int area, class bitSet_c& bs );
    
    u32 getNumAreas() const
    {
        return areas.size();
    }
};

#endif // __SV_VIS_H__

