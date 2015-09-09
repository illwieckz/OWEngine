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
//  File name:   rf_proc.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Doom 3 / Quake 4 .proc rendering code
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "rf_proc.h"
#include "rf_surface.h"
#include "rf_local.h"
#include "rf_model.h"
#include <api/coreAPI.h>
#include <shared/parser.h>
#include <shared/cmWinding.h>
#include <shared/autoCvar.h>
#include <math/frustumExt.h>

static aCvar_c rf_proc_printCamArea( "rf_proc_printCamArea", "0" );
static aCvar_c rf_proc_showAreaPortals( "rf_proc_showAreaPortals", "0" );
static aCvar_c rf_proc_ignorePortals( "rf_proc_ignorePortals", "0" );
static aCvar_c rf_proc_ignoreCullBounds( "rf_proc_ignoreCullBounds", "0" );
// use this to see if lights are being properly culled by .proc areaportals
/*static*/
aCvar_c rf_proc_useProcDataToOptimizeLighting( "rf_proc_useProcDataToOptimizeLighting", "1" );
static aCvar_c rf_proc_printChoppedFrustums( "rf_proc_printChoppedFrustums", "0" );

class procPortal_c
{
    friend class procTree_c;
    int areas[2];
    cmWinding_c points;
    plane_c plane;
    aabb bounds;
    frustumExt_c lastFrustum[MAX_PORTAL_VISIT_COUNT];
    int visitCount;
    int visCount;
    
    procPortal_c()
    {
        visCount = -1;
        visitCount = 0;
    }
    void calcPlane()
    {
        points.getPlane( plane );
    }
    void calcBounds()
    {
        points.getBounds( bounds );
    }
};

void procTree_c::setAreaModel( u32 areaNum, r_model_c* newAreaModel )
{
    procArea_c* area = getArea( areaNum );
    if( area->areaModel )
    {
        g_core->RedWarning( "procTree_c::setAreaModel: WARNING: area %i already has its model set (modName: %s)\n", areaNum, area->areaModel->getName() );
        return;
    }
    area->areaModel = newAreaModel;
}
void procTree_c::addPortalToAreas( procPortal_c* portal )
{
    addPortalToArea( portal->areas[0], portal );
    addPortalToArea( portal->areas[1], portal );
}
void procTree_c::clear()
{
    nodes.clear();
    for( u32 i = 0; i < models.size(); i++ )
    {
        if( models[i] )
        {
            delete models[i];
        }
    }
    models.clear();
    for( u32 i = 0; i < areas.size(); i++ )
    {
        if( areas[i] )
        {
            delete areas[i];
        }
    }
    areas.clear();
    for( u32 i = 0; i < portals.size(); i++ )
    {
        if( portals[i] )
        {
            delete portals[i];
        }
    }
    portals.clear();
}
bool procTree_c::parseNodes( class parser_c& p, const char* fname )
{
    if( p.atWord( "{" ) == false )
    {
        g_core->RedWarning( "procTree_c::parseNodes: expected '{' to follow \"nodes\" in file %s at line %i, found %s\n",
                            fname, p.getCurrentLineNumber(), p.getToken() );
        return true; // error
    }
    u32 numNodes = p.getInteger();
    this->nodes.resize( numNodes );
    procNode_c* n = this->nodes.getArray();
    for( u32 i = 0; i < numNodes; i++, n++ )
    {
        if( p.atWord( "(" ) == false )
        {
            g_core->RedWarning( "procTree_c::parseNodes: expected '(' to follow node %i in file %s at line %i, found %s\n",
                                i, fname, p.getCurrentLineNumber(), p.getToken() );
            return true; // error
        }
        p.getFloatMat( n->plane.norm, 3 );
        n->plane.dist = p.getFloat();
        n->plane.setSignBitsAndType();
        if( p.atWord( ")" ) == false )
        {
            g_core->RedWarning( "procTree_c::parseNodes: expected '(' after node's %i plane equation in file %s at line %i, found %s\n",
                                i, fname, p.getCurrentLineNumber(), p.getToken() );
            return true; // error
        }
        n->children[0] = p.getInteger();
        n->children[1] = p.getInteger();
    }
    if( p.atWord( "}" ) == false )
    {
        g_core->RedWarning( "procTree_c::parseNodes: expected closing '}' for \"nodes\" block in file %s at line %i, found %s\n",
                            fname, p.getCurrentLineNumber(), p.getToken() );
        return true; // error
    }
    return false; // OK
}
bool procTree_c::parseAreaPortals( class parser_c& p, const char* fname )
{
    if( p.atWord( "{" ) == false )
    {
        g_core->RedWarning( "procTree_c::parseAreaPortals: expected '{' to follow \"interAreaPortals\" in file %s at line %i, found %s\n",
                            fname, p.getCurrentLineNumber(), p.getToken() );
        return true; // error
    }
    u32 numAreas = p.getInteger();
    u32 numAreaPortals = p.getInteger();
    this->portals.resize( numAreaPortals );
    for( u32 i = 0; i < numAreaPortals; i++ )
    {
        procPortal_c* portal = this->portals[i] = new procPortal_c;
        u32 numPoints = p.getInteger();
        if( numPoints <= 2 )
        {
            g_core->RedWarning( "procTree_c::parseAreaPortals: ERROR: portal %i has less than three points! (%s at line %i)\n", i, fname, p.getCurrentLineNumber() );
            return true;
        }
        portal->areas[0] = p.getInteger();
        portal->areas[1] = p.getInteger();
        portal->points.resize( numPoints );
        vec3_c* v = portal->points.getArray();
        for( u32 j = 0; j < numPoints; j++, v++ )
        {
            p.getFloatMat_braced( &v->x, 3 );
        }
        portal->calcPlane();
        portal->calcBounds();
        // let the areas know that they are connected by a portal
        this->addPortalToAreas( portal );
    }
    if( p.atWord( "}" ) == false )
    {
        g_core->RedWarning( "procTree_c::parseAreaPortals: expected closing '}' for \"interAreaPortals\" block in file %s at line %i, found %s\n",
                            fname, p.getCurrentLineNumber(), p.getToken() );
        return true; // error
    }
    return false; // OK
}
bool procTree_c::loadProcFile( const char* fname )
{
    parser_c p;
    if( p.openFile( fname ) )
    {
        g_core->RedWarning( "procTree_c::loadProcFile: cannot open %s\n", fname );
        return true; // error
    }
    
    // check for Doom3 ident first
    if( p.atWord( "mapProcFile003" ) == false )
    {
        if( p.atWord( "PROC" ) )
        {
            // Quake4 ident
            str version = p.getToken();
        }
        else
        {
            g_core->RedWarning( "procTree_c::loadProcFile: %s has bad ident %s, should be %s or %s\n", fname, p.getToken(), "mapProcFile003", "PROC" );
            return true; // error
        }
    }
    
    while( p.atEOF() == false )
    {
        if( p.atWord( "model" ) )
        {
            r_model_c* newModel = new r_model_c;
            this->models.push_back( newModel );
            if( newModel->parseProcModel( p ) )
            {
                return true; // error occured during model parsing
            }
            newModel->recalcModelTBNs();
            model_c* m = RF_AllocModel( newModel->getName() );
            m->initProcModel( this, newModel );
            
            if( newModel->isAreaModel() )
            {
                u32 areaNum = newModel->getAreaNumber();
                this->setAreaModel( areaNum, newModel );
            }
        }
        else if( p.atWord( "interAreaPortals" ) )
        {
            if( parseAreaPortals( p, fname ) )
            {
                return true;
            }
        }
        else if( p.atWord( "nodes" ) )
        {
            if( parseNodes( p, fname ) )
            {
                return true;
            }
        }
        else if( p.atWord( "shadowModel" ) )
        {
            p.skipCurlyBracedBlock();
        }
        else
        {
            g_core->RedWarning( "procTree_c::loadProcFile: skipping unknown token %s in file %s at line %i\n", p.getToken(), fname, p.getCurrentLineNumber() );
        }
    }
    
    this->procFileName = fname;
    
    // create model VBOs and IBOs
    for( u32 i = 0; i < models.size(); i++ )
    {
        r_model_c* m = models[i];
        m->createVBOsAndIBOs();
    }
    
    g_core->RedWarning( "procTree_c::loadProcFile: %s has %i models, %i areas, %i portals, and %i nodes\n",
                        fname, models.size(), areas.size(), portals.size(), nodes.size() );
                        
    return false; // OK
}
#include <shared/readStream.h>

#pragma pack(push)
#pragma pack(1)
struct procbSurface_s
{
    int materialIndex;
    byte hasVertexColor;
    vec3_c mins;
    vec3_c maxs;
    int numVerts;
    int numIndices;
    int __privateCount;
};
#pragma pack(pop)
bool procTree_c::loadProcFileBinary( const char* fname )
{
    readStream_c s;
    if( s.loadFromFile( fname ) )
    {
    
        return true;
    }
    int version = s.readInt();
    char identStr[15];
    s.readCharArray( identStr, 14 );
    u32 numMaterials;
    arraySTD_c<str> matNames;
    int numAreas, numAreaPortals;
    while( s.isAtEOF() == false )
    {
        int subSectionNameStringLen = s.readInt();
        if( s.isAtCharArray( "materials", subSectionNameStringLen ) )
        {
            numMaterials = s.readInt();
            matNames.resize( numMaterials );
            for( u32 i = 0; i < numMaterials; i++ )
            {
                u32 len = s.readInt();
                char buf[256];
                s.readCharArray( buf, len );
                buf[len] = 0;
                matNames[i] = buf;
                //g_core->Print("procTree_c::loadProcFileBinary: mat %i is a string %s with len %i\n",i,buf,len);
            }
        }
        else if( s.isAtCharArray( "interAreaPortals", subSectionNameStringLen ) )
        {
            numAreas = s.readInt();
            numAreaPortals = s.readInt();
#if 1
            while( s.isAtStr( "model" ) == false )
            {
                s.skipBytes( 1 );
            }
            goto parseModel;
#endif
        }
        else if( s.isAtCharArray( "model", subSectionNameStringLen ) )
        {
parseModel:
            int modelDataLen = s.readInt();
            int modelDataLenEnd = s.getPos() + modelDataLen;
            int nameLen = s.readInt();
            char name[256];
            s.readCharArray( name, nameLen );
            name[nameLen] = 0;
            g_core->Print( "Model name: %s\n", name );
            int numModelSurfs = s.readInt();
            int numModelAreas = s.readInt();
            for( u32 i = 0; i < numModelAreas; i++ )
            {
                int ar = s.readInt();
            }
            r_model_c* mod = new r_model_c;
            mod->setName( name );
            mod->resizeSurfaces( numModelSurfs );
            this->models.push_back( mod );
            for( u32 i = 0; i < numModelSurfs; i++ )
            {
                const procbSurface_s* sf = ( procbSurface_s* )s.getCurDataPtr();
                if( sf->materialIndex < 0 )
                {
                    g_core->RedWarning( "bad material index\n" );
                }
                s.skipBytes( sizeof( procbSurface_s ) );
                r_surface_c* out = mod->getSurf( i );
                rVertexBuffer_c& verts = out->getVerts();
                verts.resize( sf->numVerts );
                rVert_c* ov = verts.getArray();
                for( u32 j = 0; j < sf->numVerts; j++, ov++ )
                {
                    int numVertexElements = s.readInt();
                    float vertexData[32];
                    s.readData( vertexData, numVertexElements * sizeof( float ) );
                    if( sf->hasVertexColor )
                    {
                        int numColorElements = s.readInt();
                        s.skipBytes( numColorElements * sizeof( byte ) );
                    }
                    ov->xyz = &vertexData[0];
                    ov->tc = &vertexData[3];
                    ov->normal = &vertexData[5];
                }
                rIndexBuffer_c& indices = out->getIndices();
                if( sf->numVerts < U16_MAX )
                {
                    // use 16 bit indices
                    u16* u16Indices = indices.initU16( sf->numIndices );
                    for( u32 j = 0; j < sf->numIndices; j++ )
                    {
                        u16Indices[j] = s.readInt();
                    }
                }
                else
                {
                    // use 32 bit indices
                    indices.initU32( sf->numIndices );
                    s.readData( indices.getArray(), sf->numIndices * sizeof( int ) );
                }
                for( u32 j = 0; j < sf->__privateCount; j++ )
                {
                    float prv[6];
                    int pri[4];
                    s.readData( prv, sizeof( prv ) );
                    s.readData( pri, sizeof( pri ) );
                }
                // set material
                const char* matName = matNames[sf->materialIndex];
                out->setMaterial( matName );
            }
            mod->createVBOsAndIBOs();
            int modelDataRealEnd = s.getPos();
            if( modelDataLenEnd != modelDataRealEnd )
            {
                s.setPos( modelDataLenEnd );
            }
        }
        else
        {
            s.skipBytes( subSectionNameStringLen );
            int subSectionSize = s.readInt();
            s.skipBytes( subSectionSize );
        }
    }
    return false; // OK
}
int procTree_c::pointArea( const vec3_c& xyz )
{
    if( nodes.size() == 0 )
        return 0;
        
    int nodeNum = 0;
    do
    {
        if( nodeNum >= this->nodes.size() )
        {
            g_core->RedWarning( "procTree_c::pointArea: node index %i out of range <0,%i)\n", nodeNum, this->nodes.size() );
            return -1;
        }
        const procNode_c& node = this->nodes[nodeNum];
        float d = node.plane.distance( xyz );
        if( d > 0 )
        {
            // front
            nodeNum = node.children[0];
        }
        else
        {
            // back
            nodeNum = node.children[1];
        }
    }
    while( nodeNum > 0 );
    return -nodeNum - 1;
}
void procTree_c::boxAreas_r( const aabb& bb, arraySTD_c<u32>& out, int nodeNum )
{
    while( 1 )
    {
        if( nodeNum < 0 )
        {
            int areaNum = ( -nodeNum - 1 );
            procArea_c* ar = areas[areaNum];
            if( ar->checkCount != this->checkCount )
            {
                ar->checkCount = this->checkCount;
                out.push_back( areaNum );
            }
            return; // done.
        }
        const procNode_c& n = nodes[nodeNum];
        planeSide_e ps = n.plane.onSide( bb );
        
        if( ps == SIDE_FRONT )
        {
            if( n.children[0] == 0 )
            {
                return;
            }
            nodeNum = n.children[0];
        }
        else if( ps == SIDE_BACK )
        {
            if( n.children[1] == 0 )
            {
                return;
            }
            nodeNum = n.children[1];
        }
        else
        {
            if( n.children[0] )
            {
                nodeNum = n.children[0];
                if( n.children[1] )
                {
                    boxAreas_r( bb, out, n.children[1] );
                }
            }
            else
            {
                nodeNum = n.children[1];
            }
        }
    }
}
u32 procTree_c::boxAreas( const aabb& bb, arraySTD_c<u32>& out )
{
    checkCount++;
    // preallocate memory (areas.size() should be always enough)
    out.reserve( areas.size() );
    if( nodes.size() == 0 )
    {
        out.push_back( 0 );
        return 1;
    }
    boxAreas_r( bb, out, 0 );
    return out.size();
}
u32 procTree_c::boxAreaSurfaces( const aabb& bb, arraySTD_c<const r_surface_c*>& out )
{
    arraySTD_c<u32> areaNums;
    // preallocate memory
    out.reserve( areas.size() * 4 );
    boxAreas( bb, areaNums );
    for( u32 i = 0; i < areaNums.size(); i++ )
    {
        procArea_c* area = areas[areaNums[i]];
        area->areaModel->boxSurfaces( bb, out );
    }
    return out.size();
}
void procTree_c::addAreaDrawCalls_r( int areaNum, const frustumExt_c& fr, procPortal_c* prevPortal )
{
    if( areaNum >= areas.size() || areaNum < 0 )
        return;
    procArea_c* ar = areas[areaNum];
    if( ar == 0 )
        return;
    if( ar->visCount != this->visCount )
    {
        //drawAreaDrawCalls(ar);
        ar->areaModel->addDrawCalls();
        ar->visCount = this->visCount;
    }
    for( u32 i = 0; i < ar->portals.size(); i++ )
    {
        procPortal_c* p = ar->portals[i];
        if( p == prevPortal )
        {
            continue;
        }
        // first check if the portal is in the frustum
        if( fr.cull( p->bounds ) == CULL_OUT )
            continue;
        // then check if the portal side facing camera
        // belong to current area. If not, portal is occluded
        // by the wall and is not really visible
        float d = p->plane.distance( rf_camera.getOrigin() );
        if( p->areas[0] == areaNum )
        {
            if( d > 0 )
                continue;
        }
        else
        {
            if( d < 0 )
                continue;
        }
        // adjust the frustum, so addAreaDrawCalls_r will never loop and cause a stack overflow...
        frustumExt_c adjusted;
        adjusted.adjustFrustum( fr, rf_camera.getOrigin(), p->points, p->plane );
        if( adjusted.size() == 0 )
        {
            // it happend very often and doesn't seem to cause any visible errors
            if( rf_proc_printChoppedFrustums.getInt() )
            {
                g_core->RedWarning( "procTree_c::addAreaDrawCalls_r: frustum chopped away (dist %f)\n", d );
            }
            continue;
        }
        
        if( p->visCount != this->visCount )
        {
            p->visCount = this->visCount;
            p->visitCount = 0;
        }
        else
        {
            if( p->visitCount >= MAX_PORTAL_VISIT_COUNT )
            {
                g_core->RedWarning( "MAX_PORTAL_VISIT_COUNT!!!\n" );
                continue;
            }
        }
        p->lastFrustum[p->visitCount] = adjusted;
        p->visitCount++;
        
        if( p->areas[0] == areaNum )
        {
            addAreaDrawCalls_r( p->areas[1], adjusted, p );
        }
        else
        {
            addAreaDrawCalls_r( p->areas[0], adjusted, p );
        }
    }
}
void procTree_c::addDrawCallsForAllAreas()
{
    for( u32 i = 0; i < areas.size(); i++ )
    {
        procArea_c* ar = areas[i];
        if( rf_camera.getFrustum().cull( ar->areaModel->getBounds() ) != CULL_OUT )
        {
            ar->areaModel->addDrawCalls();
            ar->visCount = this->visCount;
        }
    }
}
void procTree_c::addDrawCalls()
{
    visCount++;
    if( nodes.size() == 0 )
    {
        for( u32 i = 0; i < models.size(); i++ )
        {
            models[i]->addDrawCalls();
        }
        return;
    }
    camArea = pointArea( rf_camera.getPVSOrigin() );
    if( rf_proc_printCamArea.getInt() )
    {
        g_core->Print( "camera is in area %i of %i\n", camArea, areas.size() );
    }
    // if camera is outside world or if we're debug-drawing all areas
    if( camArea == -1 || rf_proc_ignorePortals.getInt() )
    {
        addDrawCallsForAllAreas();
        return;
    }
    frustumExt_c baseFrustum( rf_camera.getFrustum() );
    addAreaDrawCalls_r( camArea, baseFrustum, 0 );
}
#include <shared/trace.h>
void procTree_c::traceNodeRay_r( int nodeNum, class trace_c& out )
{
    if( nodeNum < 0 )
    {
        int areaNum = ( -nodeNum - 1 );
        procArea_c* ar = areas[areaNum];
        if( ar->checkCount != this->checkCount )
        {
            if( ar->areaModel->traceRay( out ) )
            {
                out.setHitAreaNum( areaNum );
            }
            ar->checkCount = this->checkCount;
        }
        return; // done.
    }
    const procNode_c& n = nodes[nodeNum];
    // classify ray against split plane
    float d0 = n.plane.distance( out.getStartPos() );
    // hitPos is the actual endpos of the trace
    float d1 = n.plane.distance( out.getHitPos() );
    
    if( d0 >= 0 && d1 >= 0 )
    {
        // trace is on the front side of the plane
        if( n.children[0] )
        {
            traceNodeRay_r( n.children[0], out );
        }
    }
    else if( d0 < 0 && d1 < 0 )
    {
        // trace is on the back side of the plane
        if( n.children[1] )
        {
            traceNodeRay_r( n.children[1], out );
        }
    }
    else
    {
        // trace crosses the plane - both childs must be checked.
        // TODO: clip the trace start/end points?
        if( n.children[0] )
        {
            traceNodeRay_r( n.children[0], out );
        }
        if( n.children[1] )
        {
            traceNodeRay_r( n.children[1], out );
        }
    }
}
bool procTree_c::traceRay( class trace_c& out )
{
    checkCount++;
    float prevFrac = out.getFraction();
    if( nodes.size() )
    {
        traceNodeRay_r( 0, out );
    }
    else
    {
        for( u32 i = 0; i < models.size(); i++ )
        {
            models[i]->traceRay( out );
        }
    }
    if( out.getFraction() < prevFrac )
        return true;
    return false;
}
#include "rf_decalProjector.h"
u32 procTree_c::createAreaDecals( u32 areaNum, class decalProjector_c& proj ) const
{
    const procArea_c* ar = areas[areaNum];
    ar->areaModel->createDecalInternal( proj );
    return 0;
}
int procTree_c::addWorldMapDecal( const vec3_c& pos, const vec3_c& normal, float radius, class mtrAPI_i* material )
{
    decalProjector_c proj;
    proj.init( pos, normal, radius );
    proj.setMaterial( material );
    arraySTD_c<u32> arNums;
    boxAreas( proj.getBounds(), arNums );
    for( u32 i = 0; i < arNums.size(); i++ )
    {
        createAreaDecals( arNums[i], proj );
    }
    proj.addResultsToDecalBatcher( RF_GetWorldDecalBatcher() );
    return 0; // TODO: return valid decal handle?
}
bool procTree_c::cullBoundsByPortals( const aabb& absBB, const arraySTD_c<u32>& areaNums )
{
    for( u32 i = 0; i < areaNums.size(); i++ )
    {
        u32 areaNum = areaNums[i];
        if( areaNum == camArea )
        {
            return false; // didnt cull
        }
    }
    for( u32 i = 0; i < areaNums.size(); i++ )
    {
        u32 areaNum = areaNums[i];
        if( areaNum == camArea )
        {
            continue;
        }
        procArea_c* ar = areas[areaNum];
        if( ar->visCount == this->visCount )
        {
            //printf("Checking area %i with %i portals\n",areaNum,ar->portals.size());
            for( u32 j = 0; j < ar->portals.size(); j++ )
            {
                procPortal_c* p = ar->portals[j];
                if( p->visCount == this->visCount )
                {
                    //printf("Portal visicount %i\n",p->visitCount);
                    for( u32 k = 0; k < p->visitCount; k++ )
                    {
                        if( p->lastFrustum[k].cull( absBB ) != CULL_OUT )
                        {
                            //printf("Area %i ent visible\n",areaNum);
                            return false; // didnt cull
                        }
                    }
                }
            }
        }
    }
    // entity is not visible by player (culled by portals)
    return true;
}
bool procTree_c::cullBoundsByPortals( const aabb& absBB )
{
    if( rf_proc_ignoreCullBounds.getInt() )
        return false; // didnt cull
    arraySTD_c<u32> areaNums;
    boxAreas( absBB, areaNums );
    return cullBoundsByPortals( absBB, areaNums );
}
#include "rf_lights.h"
void procTree_c::addSingleAreaSurfacesInteractions( int areaNum, class rLightImpl_c* l )
{
    procArea_c* ar = areas[areaNum];
    if( ar->litCount == this->litCount )
        return;
    ar->litCount = this->litCount;
    r_model_c* areaModel = ar->areaModel;
    for( u32 i = 0; i < areaModel->getNumSurfs(); i++ )
    {
        r_surface_c* sf = areaModel->getSurf( i );
        if( sf->getBB().intersect( l->getABSBounds() ) == false )
        {
            continue;
        }
        l->addProcAreaSurfaceInteraction( areaNum, sf, SIFT_ONLY_LIGHTING );
    }
}
void procTree_c::cacheLightWorldInteractions_r( class rLightImpl_c* l, int areaNum, const frustumExt_c& fr, procPortal_c* prevPortal, u32 recursionDepth )
{
    if( areaNum >= areas.size() || areaNum < 0 )
        return;
    procArea_c* ar = areas[areaNum];
    if( ar == 0 )
        return;
    // current algorithm still causes stack overflow in some very rare cases
    if( recursionDepth > 64 )
    {
        return;
    }
    addSingleAreaSurfacesInteractions( areaNum, l );
    for( u32 i = 0; i < ar->portals.size(); i++ )
    {
        procPortal_c* p = ar->portals[i];
        if( p == prevPortal )
        {
            continue;
        }
        if( l->getABSBounds().intersect( p->bounds ) == false )
            continue;
        // first check if the portal is in the frustum
        if( fr.cull( p->bounds ) == CULL_OUT )
            continue;
        // then check if the portal side facing light
        // belong to current area. If not, portal is occluded
        // by the wall and is not really visible
        float d = p->plane.distance( l->getOrigin() );
        if( p->areas[0] == areaNum )
        {
            if( d > 0 )
                continue;
        }
        else
        {
            if( d < 0 )
                continue;
        }
        // adjust the frustum, so cacheLightWorldInteractions_r will never loop and cause a stack overflow...
        frustumExt_c adjusted;
        adjusted.adjustFrustum( fr, l->getOrigin(), p->points, p->plane );
        if( p->areas[0] == areaNum )
        {
            cacheLightWorldInteractions_r( l, p->areas[1], adjusted, p, recursionDepth + 1 );
        }
        else
        {
            cacheLightWorldInteractions_r( l, p->areas[0], adjusted, p, recursionDepth + 1 );
        }
    }
}
void procTree_c::cacheLightWorldInteractions( class rLightImpl_c* l )
{
    if( rf_proc_useProcDataToOptimizeLighting.getInt() == 0 )
    {
        // get all .proc surfaces withing light bounds and cache'em
        arraySTD_c<const r_surface_c*> sfs;
        boxAreaSurfaces( l->getABSBounds(), sfs );
        for( u32 i = 0; i < sfs.size(); i++ )
        {
            l->addStaticModelSurfaceInteraction( ( r_surface_c* )sfs[i] );
        }
    }
    else
    {
        // get shadow-only interactions
        // It seems we need to render all the surfaces within light bounds,
        // even if they are not visible for light trough portals
        arraySTD_c<u32> list;
        boxAreas( l->getABSBounds(), list );
        for( u32 i = 0; i < list.size(); i++ )
        {
            int areaNum = list[i];
            procArea_c* a = areas[areaNum];
            r_model_c* m = a->areaModel;
            for( u32 j = 0; j < m->getNumSurfs(); j++ )
            {
                l->addProcAreaSurfaceInteraction( areaNum, m->getSurf( j ), SIFT_ONLY_SHADOW );
            }
        }
        // flood through portals (for lighting interactions)
        // draw only surfaces that are inside light "view" (affected by portals)
        litCount++;
        int lightArea = this->pointArea( l->getOrigin() );
        if( lightArea < 0 )
            return;
        const procArea_c* ar = areas[lightArea];
        addSingleAreaSurfacesInteractions( lightArea, l );
        for( u32 i = 0; i < ar->portals.size(); i++ )
        {
            procPortal_c* portal = ar->portals[i];
            frustumExt_c fr;
            // create light->portal frustum
            fr.fromPointAndWinding( l->getOrigin(), portal->points, portal->plane );
            // add far plane
            //vec3_c center = portal->points.getCenter();
            //vec3_c normal = center - l->getOrigin();
            //normal.normalize();
            //plane_c farPlane;
            //farPlane.fromPointAndNormal(center,normal);
            //fr.addPlane(farPlane);
            
            /*	if(fr.cull(portal->points.getCenter() != CULL_IN) {
            		g_core->RedWarning("bad frustum for portal %i\n",i);
            	}*/
            //g_core->Print("procTree_c::cacheLightWorldInteractions: area %i to portal %i frustum has %i planes\n",lightArea,i,fr.size());
            int otherArea;
            if( lightArea == portal->areas[0] )
            {
                otherArea = portal->areas[1];
            }
            else
            {
                otherArea = portal->areas[0];
            }
            cacheLightWorldInteractions_r( l, otherArea, fr, portal );
        }
    }
}
void procTree_c::getReferencedMatNames( class perStringCallbackListener_i* callback ) const
{
    for( u32 i = 0; i < areas.size(); i++ )
    {
        const procArea_c* ar = areas[i];
        if( ar == 0 )
            continue;
        ar->areaModel->getReferencedMatNames( callback );
    }
}
void procTree_c::setSurfaceMaterial( u32 areaNum, u32 surfaceNum, const char* matName )
{
    if( areaNum >= areas.size() )
    {
        g_core->RedWarning( "procTree_c::setSurfaceMaterial: bad areaNum %i\n", areaNum );
        return;
    }
    areas[areaNum]->areaModel->setSurfaceMaterial( surfaceNum, matName );
}

void procTree_c::doDebugDrawing()
{
    if( rf_proc_showAreaPortals.getInt() )
    {
        for( u32 i = 0; i < portals.size(); i++ )
        {
            procPortal_c* p = portals[i];
            const cmWinding_c& w = p->points;
            float col[4];
            col[0] = ( i % 39 ) / 39.f;
            col[1] = ( ( i + 10 ) % 47 ) / 47.f;
            col[2] = ( i % 33 ) / 33.f;
            col[3] = 1.f;
            rb->setColor4( col );
            rb->drawWinding( w.getArray(), w.size() );
        }
    }
}
procTree_c* RF_LoadPROC( const char* fname )
{
    procTree_c* ret = new procTree_c;
    if( ret->loadProcFile( fname ) )
    {
        delete ret;
        return 0;
    }
    return ret;
}
procTree_c* RF_LoadPROCB( const char* fname )
{
    procTree_c* ret = new procTree_c;
    if( ret->loadProcFileBinary( fname ) )
    {
        delete ret;
        return 0;
    }
    return ret;
}
