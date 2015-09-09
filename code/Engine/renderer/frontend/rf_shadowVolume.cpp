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
//  File name:   rf_shadowVolume.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Shadow volume class for stencil shadows
//               (Doom 3 style)
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "rf_shadowVolume.h"
#include "rf_drawCall.h"
#include "rf_local.h"
#include "rf_entities.h"
#include "rf_surface.h"
#include "rf_stencilShadowCaster.h"
#include "rf_model.h"
#include <math/plane.h>
#include <shared/autoCvar.h>
#include <shared/planeArray.h>
#include <shared/collisionUtils.h>
#include <api/coreAPI.h>
#include <api/rLightAPI.h>
#include "../rModelAPI.h"

static int c_allocatedShadowVolumes = 0;

static aCvar_c rf_printShadowVolumesStats( "rf_printShadowVolumesStats", "0" );
static aCvar_c rf_dontUsePrecomputedSSVCasters( "rf_dontUsePrecomputedSSVCasters", "0" );
static aCvar_c rf_dontUseExtraEdgeArrays( "rf_dontUseExtraEdgeArrays", "0" );
static aCvar_c rf_skipAnimatedObjectsShadows( "rf_skipAnimatedObjectsShadows", "0" );
static aCvar_c rf_ssv_cullTrianglesOutSideLightSpheres( "rf_ssv_cullTrianglesOutSideLightSpheres", "1" );
// looks like hashing shadow volume vertices actually slows down everything for models that has shadow volume updated every frame
// (the benefit from sending the smaller count of vertices to GPU is lower than slowdown caused by hashing)
static aCvar_c rf_ssv_hashVertices( "rf_ssv_hashVertices", "0" );
static aCvar_c rf_ssv_algorithm( "rf_ssv_algorithm", "1" );

inline float getShadowVolumeInf()
{
//	return 4096.f;
    return rf_camera.getZFar() * 0.5f;
}

rIndexedShadowVolume_c::rIndexedShadowVolume_c()
{
    c_allocatedShadowVolumes++;
}
rIndexedShadowVolume_c::~rIndexedShadowVolume_c()
{
    c_allocatedShadowVolumes--;
    //printf("%i active shadow volumes\n",c_allocatedShadowVolumes);
}
u32 rIndexedShadowVolume_c::registerPoint( const vec3_c& p )
{
    bounds.addPoint( p );
    if( rf_ssv_hashVertices.getInt() )
    {
        return points.registerVec3( p );
    }
    return points.addPoint( p );
}
void rIndexedShadowVolume_c::addTriangle( const vec3_c& p0, const vec3_c& p1, const vec3_c& p2, const vec3_c& light )
{
    plane_c triPlane;
    triPlane.fromThreePoints( p0, p1, p2 );
    float d = triPlane.distance( light );
    if( d < 0 )
        return;
    vec3_c p0Projected = p0 - light;
    p0Projected.normalizeFast();
    p0Projected *= getShadowVolumeInf();
    p0Projected += p0;
    vec3_c p1Projected = p1 - light;
    p1Projected.normalizeFast();
    p1Projected *= getShadowVolumeInf();
    p1Projected += p1;
    vec3_c p2Projected = p2 - light;
    p2Projected.normalizeFast();
    p2Projected *= getShadowVolumeInf();
    p2Projected += p2;
    
    u32 i0 = this->registerPoint( p0 );
    u32 i1 = this->registerPoint( p1 );
    u32 i2 = this->registerPoint( p2 );
    u32 pi0 = this->registerPoint( p0Projected );
    u32 pi1 = this->registerPoint( p1Projected );
    u32 pi2 = this->registerPoint( p2Projected );
    
    indices.addTriangle( i2, i1, i0 );
    indices.addTriangle( pi0, pi1, pi2 );
    c_capTriPairsAdded++;
    indices.addQuad( i0, i1, pi0, pi1 );
    c_edgeQuadsAdded++;
    indices.addQuad( i1, i2, pi1, pi2 );
    c_edgeQuadsAdded++;
    indices.addQuad( i2, i0, pi2, pi0 );
    c_edgeQuadsAdded++;
}
void rIndexedShadowVolume_c::addFrontCapAndBackCapForTriangle( const vec3_c& p0, const vec3_c& p1, const vec3_c& p2, const vec3_c& light )
{
    vec3_c p0Projected = p0 - light;
    p0Projected.normalizeFast();
    p0Projected *= getShadowVolumeInf();
    p0Projected += p0;
    vec3_c p1Projected = p1 - light;
    p1Projected.normalizeFast();
    p1Projected *= getShadowVolumeInf();
    p1Projected += p1;
    vec3_c p2Projected = p2 - light;
    p2Projected.normalizeFast();
    p2Projected *= getShadowVolumeInf();
    p2Projected += p2;
    
    u32 i0 = this->registerPoint( p0 );
    u32 i1 = this->registerPoint( p1 );
    u32 i2 = this->registerPoint( p2 );
    u32 pi0 = this->registerPoint( p0Projected );
    u32 pi1 = this->registerPoint( p1Projected );
    u32 pi2 = this->registerPoint( p2Projected );
    
    indices.addTriangle( i2, i1, i0 );
    indices.addTriangle( pi0, pi1, pi2 );
    
    c_capTriPairsAdded++;
}
void rIndexedShadowVolume_c::addFrontCapAndBackCapForIndexedVertsList( const rIndexBuffer_c& ibo, const rVertexBuffer_c& vbo, const vec3_c& light )
{
    for( u32 i = 0; i < ibo.getNumIndices(); i += 3 )
    {
        u32 i0 = ibo[i + 0];
        u32 i1 = ibo[i + 1];
        u32 i2 = ibo[i + 2];
        const vec3_c& v0 = vbo[i0].xyz;
        const vec3_c& v1 = vbo[i1].xyz;
        const vec3_c& v2 = vbo[i2].xyz;
        addFrontCapAndBackCapForTriangle( v0, v1, v2, light );
    }
}
void rIndexedShadowVolume_c::addEdge( const vec3_c& p0, const vec3_c& p1, const vec3_c& light )
{
    vec3_c p0Projected = p0 - light;
    p0Projected.normalizeFast();
    p0Projected *= getShadowVolumeInf();
    p0Projected += p0;
    vec3_c p1Projected = p1 - light;
    p1Projected.normalizeFast();
    p1Projected *= getShadowVolumeInf();
    p1Projected += p1;
    
    u32 i0 = this->registerPoint( p0 );
    u32 i1 = this->registerPoint( p1 );
    u32 pi0 = this->registerPoint( p0Projected );
    u32 pi1 = this->registerPoint( p1Projected );
    
    indices.addQuad( i0, i1, pi0, pi1 );
    
    c_edgeQuadsAdded++;
}
void rIndexedShadowVolume_c::addDrawCall()
{
    RF_AddShadowVolumeDrawCall( &this->points, &this->indices );
}
void rIndexedShadowVolume_c::createShadowVolumeForEntity( class rEntityImpl_c* ent, const vec3_c& light, float lightRadius )
{
    clear();
    // save the last light position
    //this->lightPos = light;
    
    if( ent == 0 )
        return;
    if( ent->getModel() == 0 )
        return;
    if( ent->isAnimated() && rf_skipAnimatedObjectsShadows.getInt() )
        return;
    const r_model_c* m = ent->getCurrentRModelInstance();
    if( m )
    {
        fromRModel( m, light, lightRadius );
    }
    else
    {
        rModelAPI_i* modAPI = ent->getModel();
        if( modAPI )
        {
            if( modAPI->isInlineBSPModel() )
            {
                r_model_c tmp;
                modAPI->getModelData( &tmp );
                //tmp.transform(ent->getMatrix());
                fromRModel( &tmp, light, lightRadius );
            }
            else if( modAPI->isStatic() )
            {
                class model_c* m = dynamic_cast<model_c*>( modAPI );
                if( m )
                {
                    fromRModel( m->getRModel(), light, lightRadius );
                }
            }
        }
    }
    if( rf_printShadowVolumesStats.getInt() )
    {
        g_core->Print( "rIndexedShadowVolume_c::createShadowVolumeForEntity: (time %i) %i points, %i tris for model %s (%i edge quads, %i cap pairs)\n",
                       rf_curTimeMsec, points.size(), indices.getNumIndices() / 3, ent->getModelName(), this->c_edgeQuadsAdded, this->c_capTriPairsAdded );
    }
}
void rIndexedShadowVolume_c::createDirectionalShadowVolumeForEntity( class rEntityImpl_c* ent, const vec3_c& lightDirection, float lightInfinity )
{
    clear();
    // save the last light position
    //this->lightPos = light;
    
    if( ent == 0 )
        return;
    if( ent->getModel() == 0 )
        return;
    if( ent->isAnimated() && rf_skipAnimatedObjectsShadows.getInt() )
        return;
    const r_model_c* m = ent->getCurrentRModelInstance();
    if( m )
    {
        fromDirectionalRModel( m, lightDirection, lightInfinity );
    }
    else
    {
        rModelAPI_i* modAPI = ent->getModel();
        if( modAPI )
        {
            if( modAPI->isInlineBSPModel() )
            {
                r_model_c tmp;
                modAPI->getModelData( &tmp );
                //tmp.transform(ent->getMatrix());
                fromRModel( &tmp, lightDirection, lightInfinity );
            }
            else if( modAPI->isStatic() )
            {
                class model_c* m = dynamic_cast<model_c*>( modAPI );
                if( m )
                {
                    fromDirectionalRModel( m->getRModel(), lightDirection, lightInfinity );
                }
                else
                {
                    g_core->RedWarning( "rIndexedShadowVolume_c::createDirectionalShadowVolumeForEntity: dynamic_cast to model_c failed for model %s.\n",
                                        modAPI->getName() );
                }
            }
        }
    }
    if( rf_printShadowVolumesStats.getInt() )
    {
        g_core->Print( "rIndexedShadowVolume_c::createDirectionalShadowVolumeForEntity: (time %i) %i points, %i tris for model %s (%i edge quads, %i cap pairs)\n",
                       rf_curTimeMsec, points.size(), indices.getNumIndices() / 3, ent->getModelName(), this->c_edgeQuadsAdded, this->c_capTriPairsAdded );
    }
}
bool IsAABBInsideSphere( const aabb& bb, const vec3_c& sphereCenter, float sphereRadius )
{
    float radSq = Square( sphereRadius );
    for( u32 i = 0; i < 8; i++ )
    {
        vec3_c p = bb.getPoint( i );
        if( p.distSQ( sphereCenter ) > radSq )
            return false;
    }
    return true;
}
#if 1
#define OPTIMIZE_SLOW_ADDTRIANGLE
#endif

#define ADD_TRIANGLE(ptr, i0, i1, i2) *ptr = i0; ptr++; *ptr = i1; ptr++; *ptr = i2; ptr++;
#define ADD_QUAD(ptr, i0, i1, i2, i3) ADD_TRIANGLE(ptr,i0,i1,i2); ADD_TRIANGLE(ptr,i1,i3,i2);

void rIndexedShadowVolume_c::addIndexedVertexList( const rIndexBuffer_c& oIndices, const rVertexBuffer_c& oVerts, const vec3_c& light, const class planeArray_c* extraPlanesArray, float lightRadius, const class aabb* bounds )
{
#if 1
    // for a single triangle, in worst case we might need to create:
    // front cap + end cap + 3 edge quads
    // 1 + 1 + 3 * 2 = 2 + 6 = 8 triangles
    indices.ensureAllocated_indices( indices.getNumIndices() + oIndices.getNumTriangles() * 8 * 3 );
    points.ensureAllocated( points.size() + oVerts.size() * 2 );
#endif
#ifdef OPTIMIZE_SLOW_ADDTRIANGLE
    if( indices.getU16Ptr() == 0 )
        return;
    u16* pFirstIndex = ( ( u16* )indices.getU16Ptr() ) + indices.getNumIndices();
    u16* pNextIndex = pFirstIndex;
#endif
    
    if( rf_ssv_algorithm.getInt() == 0 )
    {
        for( u32 i = 0; i < oIndices.getNumIndices(); i += 3 )
        {
            u32 i0 = oIndices[i + 0];
            u32 i1 = oIndices[i + 1];
            u32 i2 = oIndices[i + 2];
            const vec3_c& v0 = oVerts[i0].xyz;
            const vec3_c& v1 = oVerts[i1].xyz;
            const vec3_c& v2 = oVerts[i2].xyz;
            addTriangle( v0, v1, v2, light );
        }
    }
    else
    {
        bool bMeshFullyInsideLight;
        if( bounds )
        {
            bMeshFullyInsideLight = IsAABBInsideSphere( *bounds, light, lightRadius );
        }
        else
        {
            bMeshFullyInsideLight = false;
        }
        // do the same thing as above, but a little faster way
        static arraySTD_c<byte> bPointTransformed;
        if( bPointTransformed.size() < oVerts.size() )
        {
            bPointTransformed.resize( oVerts.size() );
        }
        bPointTransformed.nullMemory();
        static arraySTD_c<vec3_c> pointsTransformed;
        if( pointsTransformed.size() < oVerts.size() )
        {
            pointsTransformed.resize( oVerts.size() );
        }
        
        u32 tri = 0;
        for( u32 i = 0; i < oIndices.getNumIndices(); i += 3, tri++ )
        {
            u32 vi0 = oIndices[i + 0];
            u32 vi1 = oIndices[i + 1];
            u32 vi2 = oIndices[i + 2];
            const vec3_c& p0 = oVerts[vi0].xyz;
            const vec3_c& p1 = oVerts[vi1].xyz;
            const vec3_c& p2 = oVerts[vi2].xyz;
            
            // cull triangles that are outside light radius
            // This is a good optimisation for very large models intersecting very small lights
            if( rf_ssv_cullTrianglesOutSideLightSpheres.getInt() )
            {
                // we can't cull that way any triangles if mesh bounds are entirely inside light sphere
                if( bMeshFullyInsideLight == false )
                {
                    if( CU_IntersectSphereTriangle( light, lightRadius, p0, p1, p2 ) == false )
                    {
                        continue;
                    }
                }
            }
            float d;
            if( extraPlanesArray == 0 )
            {
                plane_c triPlane;
                triPlane.fromThreePoints( p2, p1, p0 );
                d = triPlane.distance( light );
            }
            else
            {
                d = extraPlanesArray->getArray()[tri].distance( light );
            }
            if( d > 0 )
            {
                continue;
            }
            vec3_c& p0Projected = pointsTransformed[vi0];
            if( bPointTransformed[vi0] == 0 )
            {
                bPointTransformed[vi0] = 1;
                p0Projected = p0 - light;
                p0Projected.normalizeFast();
                p0Projected *= getShadowVolumeInf();
                p0Projected += p0;
            }
            vec3_c& p1Projected = pointsTransformed[vi1];
            if( bPointTransformed[vi1] == 0 )
            {
                bPointTransformed[vi1] = 1;
                p1Projected = p1 - light;
                p1Projected.normalizeFast();
                p1Projected *= getShadowVolumeInf();
                p1Projected += p1;
            }
            vec3_c& p2Projected = pointsTransformed[vi2];
            if( bPointTransformed[vi2] == 0 )
            {
                bPointTransformed[vi2] = 1;
                p2Projected = p2 - light;
                p2Projected.normalizeFast();
                p2Projected *= getShadowVolumeInf();
                p2Projected += p2;
            }
            u32 i0 = this->registerPoint( p0 );
            u32 i1 = this->registerPoint( p1 );
            u32 i2 = this->registerPoint( p2 );
            u32 pi0 = this->registerPoint( p0Projected );
            u32 pi1 = this->registerPoint( p1Projected );
            u32 pi2 = this->registerPoint( p2Projected );
#ifdef OPTIMIZE_SLOW_ADDTRIANGLE
            ADD_TRIANGLE( pNextIndex, i2, i1, i0 );
            ADD_TRIANGLE( pNextIndex, pi0, pi1, pi2 );
            ADD_QUAD( pNextIndex, i0, i1, pi0, pi1 );
            ADD_QUAD( pNextIndex, i1, i2, pi1, pi2 );
            ADD_QUAD( pNextIndex, i2, i0, pi2, pi0 );
#else
            indices.addTriangle( i2, i1, i0 );
            indices.addTriangle( pi0, pi1, pi2 );
            c_capTriPairsAdded++;
            indices.addQuad( i0, i1, pi0, pi1 );
            c_edgeQuadsAdded++;
            indices.addQuad( i1, i2, pi1, pi2 );
            c_edgeQuadsAdded++;
            indices.addQuad( i2, i0, pi2, pi0 );
            c_edgeQuadsAdded++;
#endif
        }
    }
#ifdef OPTIMIZE_SLOW_ADDTRIANGLE
    u32 numAddedIndices = pNextIndex - pFirstIndex;
    indices.forceSetIndexCount( indices.getNumIndices() + numAddedIndices );
#endif
}
#include <shared/extraSurfEdgesData.h>
// NOTE: this method works faster for meshes that dont have much unmatched edges
void rIndexedShadowVolume_c::addIndexedVertexListWithEdges( const rIndexBuffer_c& ibo, const rVertexBuffer_c& vbo, const vec3_c& light, const class planeArray_c* extraPlanesArray, const struct extraSurfEdgesData_s* edges )
{
    arraySTD_c<byte> bFrontFacing;
    u32 numTris = ibo.getNumTriangles();
    bFrontFacing.resize( numTris );
    bFrontFacing.nullMemory();
    // mark triangles frontfacing the light and
    // add front cap and back cap made
    // from triangles frontfacing the light
    const plane_c* pl = extraPlanesArray->getArray();
    for( u32 i = 0; i < numTris; i++, pl++ )
    {
        float d = pl->distance( light );
        if( d < 0 )
        {
            bFrontFacing[i] = 1;
            u32 i0 = ibo[i * 3 + 0];
            u32 i1 = ibo[i * 3 + 1];
            u32 i2 = ibo[i * 3 + 2];
            const vec3_c& v0 = vbo[i0].xyz;
            const vec3_c& v1 = vbo[i1].xyz;
            const vec3_c& v2 = vbo[i2].xyz;
            this->addFrontCapAndBackCapForTriangle( v0, v1, v2, light );
        }
    }
    if( edges->getNumUnmatchedEdges() == 0 )
    {
        // the ideal case: model has no unmatched edges
        const rEdge16_s* e = edges->getArray();
        for( u32 i = 0; i < edges->size(); i++, e++ )
        {
            if( e->isMatched() == false )
            {
                continue;
            }
            u32 t0 = e->tris[0];
            u32 t1 = e->tris[1];
            if( bFrontFacing[t0] == bFrontFacing[t1] )
                continue;
            // this is a SILHUETTE edge!
            const vec3_c& v0 = vbo[e->verts[0]].xyz;
            const vec3_c& v1 = vbo[e->verts[1]].xyz;
            if( bFrontFacing[t0] )
            {
                this->addEdge( v0, v1, light );
            }
            else
            {
                this->addEdge( v1, v0, light );
            }
        }
    }
    else
    {
        // try to handle models with unmatched edges as well
        //arraySTD_c<byte> bTriAdded;
        //bTriAdded.resize(tris.size());
        //bTriAdded.nullMemory();
        const rEdge16_s* e = edges->getArray();
        for( u32 i = 0; i < edges->size(); i++, e++ )
        {
            if( e->isMatched() == false )
            {
                if( bFrontFacing[e->tris[0]] == false )
                    continue;
#if 1
                const vec3_c& v0 = vbo[e->verts[0]].xyz;
                const vec3_c& v1 = vbo[e->verts[1]].xyz;
                this->addEdge( v0, v1, light );
#else
                u32 t = e->tris[0];
                if( bTriAdded[t] )
                {
                    continue;
                }
                bTriAdded[t] = 1;
                const vec3_c& tv0 = points.getVec3( tris[t].indices[0] );
                const vec3_c& tv1 = points.getVec3( tris[t].indices[1] );
                const vec3_c& tv2 = points.getVec3( tris[t].indices[2] );
                thi->addEdge( tv0, tv1, light );
                thi->addEdge( tv1, tv2, light );
                thi->addEdge( tv2, tv0, light );
#endif
                continue;
            }
            u32 t0 = e->tris[0];
            u32 t1 = e->tris[1];
            if( bFrontFacing[t0] == bFrontFacing[t1] )
                continue;
            // this is a SILHUETTE edge!
            const vec3_c& v0 = vbo[e->verts[0]].xyz;
            const vec3_c& v1 = vbo[e->verts[1]].xyz;
            if( bFrontFacing[t0] )
            {
                this->addEdge( v0, v1, light );
            }
            else
            {
                this->addEdge( v1, v0, light );
            }
        }
    }
}
void rIndexedShadowVolume_c::addRSurface( const class r_surface_c* sf, const vec3_c& light, const struct extraSurfEdgesData_s* edges, float lightRadius )
{
    const rVertexBuffer_c& verts = sf->getVerts();
    const rIndexBuffer_c& indices = sf->getIndices();
    const planeArray_c& triPlanes = sf->getTriPlanes();
    // see if we have plane equations of triangles
    if( triPlanes.size() )
    {
        // see if we have extra edges data
        if( edges && ( rf_dontUseExtraEdgeArrays.getInt() == 0 ) )
        {
            addIndexedVertexListWithEdges( indices, verts, light, &triPlanes, edges );
        }
        else
        {
            addIndexedVertexList( indices, verts, light, &triPlanes, lightRadius, &sf->getBB() );
        }
    }
    else
    {
        addIndexedVertexList( indices, verts, light, 0, lightRadius, &sf->getBB() );
    }
}
void rIndexedShadowVolume_c::addDirectionalRSurface( const class r_surface_c* sf, const vec3_c& direction, float lightInfinity )
{
    const rVertexBuffer_c& verts = sf->getVerts();
    const rIndexBuffer_c& indices = sf->getIndices();
    const planeArray_c& triPlanes = sf->getTriPlanes();
    if( triPlanes.size() )
    {
        addDirectionalIndexedVertexList( indices, verts, direction, &triPlanes, lightInfinity, &sf->getBB() );
    }
    else
    {
        addDirectionalIndexedVertexList( indices, verts, direction, 0, lightInfinity, &sf->getBB() );
    }
}
void rIndexedShadowVolume_c::addDirectionalIndexedVertexList( const rIndexBuffer_c& oIndices, const rVertexBuffer_c& oVerts, const vec3_c& lightDir, const class planeArray_c* extraPlanesArray, float lightInfinity, const class aabb* bounds )
{
#if 1
    // for a single triangle, in worst case we might need to create:
    // front cap + end cap + 3 edge quads
    // 1 + 1 + 3 * 2 = 2 + 6 = 8 triangles
    indices.ensureAllocated_indices( indices.getNumIndices() + oIndices.getNumTriangles() * 8 * 3 );
    points.ensureAllocated( points.size() + oVerts.size() * 2 );
#endif
#ifdef OPTIMIZE_SLOW_ADDTRIANGLE
    if( indices.getU16Ptr() == 0 )
        return;
    u16* pFirstIndex = ( ( u16* )indices.getU16Ptr() ) + indices.getNumIndices();
    u16* pNextIndex = pFirstIndex;
#endif
    vec3_c delta = lightDir * lightInfinity;
    //if(rf_ssv_algorithm.getInt() == 0) {
    //	for(u32 i = 0; i < oIndices.getNumIndices(); i+=3){
    //		u32 i0 = oIndices[i+0];
    //		u32 i1 = oIndices[i+1];
    //		u32 i2 = oIndices[i+2];
    //		const vec3_c &v0 = oVerts[i0].xyz;
    //		const vec3_c &v1 = oVerts[i1].xyz;
    //		const vec3_c &v2 = oVerts[i2].xyz;
    //		addTriangle(v0,v1,v2,light);
    //	}
    //} else
    {
        //bool bMeshFullyInsideLight;
        //if(bounds) {
        //	bMeshFullyInsideLight = IsAABBInsideSphere(*bounds,light,lightRadius);
        //} else {
        //	bMeshFullyInsideLight = false;
        //}
        // do the same thing as above, but a little faster way
        static arraySTD_c<byte> bPointTransformed;
        if( bPointTransformed.size() < oVerts.size() )
        {
            bPointTransformed.resize( oVerts.size() );
        }
        bPointTransformed.nullMemory();
        static arraySTD_c<vec3_c> pointsTransformed;
        if( pointsTransformed.size() < oVerts.size() )
        {
            pointsTransformed.resize( oVerts.size() );
        }
        
        u32 tri = 0;
        for( u32 i = 0; i < oIndices.getNumIndices(); i += 3, tri++ )
        {
            u32 vi0 = oIndices[i + 0];
            u32 vi1 = oIndices[i + 1];
            u32 vi2 = oIndices[i + 2];
            const vec3_c& p0 = oVerts[vi0].xyz;
            const vec3_c& p1 = oVerts[vi1].xyz;
            const vec3_c& p2 = oVerts[vi2].xyz;
            
            // cull triangles that are outside light radius
            // This is a good optimisation for very large models intersecting very small lights
            //if(rf_ssv_cullTrianglesOutSideLightSpheres.getInt()) {
            //	// we can't cull that way any triangles if mesh bounds are entirely inside light sphere
            //	if(bMeshFullyInsideLight == false) {
            //		if(CU_IntersectSphereTriangle(light,lightRadius,p0,p1,p2) == false) {
            //			continue;
            //		}
            //	}
            //}
            float d;
            if( extraPlanesArray == 0 )
            {
                plane_c triPlane;
                triPlane.fromThreePoints( p2, p1, p0 );
                d = triPlane.norm.dotProduct( lightDir );
            }
            else
            {
                d = extraPlanesArray->getArray()[tri].norm.dotProduct( lightDir );
            }
            if( d > 0 )
            {
                continue;
            }
            vec3_c& p0Projected = pointsTransformed[vi0];
            if( bPointTransformed[vi0] == 0 )
            {
                bPointTransformed[vi0] = 1;
                p0Projected = p0 - delta;
            }
            vec3_c& p1Projected = pointsTransformed[vi1];
            if( bPointTransformed[vi1] == 0 )
            {
                bPointTransformed[vi1] = 1;
                p1Projected = p1 - delta;
            }
            vec3_c& p2Projected = pointsTransformed[vi2];
            if( bPointTransformed[vi2] == 0 )
            {
                bPointTransformed[vi2] = 1;
                p2Projected = p2 - delta;
            }
            u32 i0 = this->registerPoint( p0 );
            u32 i1 = this->registerPoint( p1 );
            u32 i2 = this->registerPoint( p2 );
            u32 pi0 = this->registerPoint( p0Projected );
            u32 pi1 = this->registerPoint( p1Projected );
            u32 pi2 = this->registerPoint( p2Projected );
#ifdef OPTIMIZE_SLOW_ADDTRIANGLE
            ADD_TRIANGLE( pNextIndex, i2, i1, i0 );
            ADD_TRIANGLE( pNextIndex, pi0, pi1, pi2 );
            ADD_QUAD( pNextIndex, i0, i1, pi0, pi1 );
            ADD_QUAD( pNextIndex, i1, i2, pi1, pi2 );
            ADD_QUAD( pNextIndex, i2, i0, pi2, pi0 );
#else
            indices.addTriangle( i2, i1, i0 );
            indices.addTriangle( pi0, pi1, pi2 );
            c_capTriPairsAdded++;
            indices.addQuad( i0, i1, pi0, pi1 );
            c_edgeQuadsAdded++;
            indices.addQuad( i1, i2, pi1, pi2 );
            c_edgeQuadsAdded++;
            indices.addQuad( i2, i0, pi2, pi0 );
            c_edgeQuadsAdded++;
#endif
        }
    }
#ifdef OPTIMIZE_SLOW_ADDTRIANGLE
    u32 numAddedIndices = pNextIndex - pFirstIndex;
    indices.forceSetIndexCount( indices.getNumIndices() + numAddedIndices );
#endif
}

void rIndexedShadowVolume_c::fromRModel( const class r_model_c* m, const vec3_c& light, float lightRadius )
{
    clear();
    if( m == 0 )
    {
        g_core->RedWarning( "rIndexedShadowVolume_c::fromRModel: NULL model pointer.\n" );
        return;
    }
    this->points.setEqualVertexEpsilon( 0.f );
#if 0
    ( ( r_model_c* )m )->precalculateStencilShadowCaster();
#endif
    if( m->getStencilShadowCaster() && rf_dontUsePrecomputedSSVCasters.getInt() == 0 )
    {
        this->fromPrecalculatedStencilShadowCaster( m->getStencilShadowCaster(), light );
        return;
    }
    for( u32 i = 0; i < m->getNumSurfs(); i++ )
    {
        const r_surface_c* sf = m->getSurf( i );
        addRSurface( sf, light, sf->getExtraSurfEdgesData(), lightRadius );
    }
}
void rIndexedShadowVolume_c::fromDirectionalRModel( const class r_model_c* m, const vec3_c& lightDirection, float lightInfinity )
{
    clear();
    this->points.setEqualVertexEpsilon( 0.f );
#if 0
    ( ( r_model_c* )m )->precalculateStencilShadowCaster();
#endif
    //if(m->getStencilShadowCaster() && rf_dontUsePrecomputedSSVCasters.getInt() == 0) {
    //	this->fromPrecalculatedStencilShadowCaster(m->getStencilShadowCaster(),light);
    //	return;
    //}
    for( u32 i = 0; i < m->getNumSurfs(); i++ )
    {
        const r_surface_c* sf = m->getSurf( i );
        addDirectionalRSurface( sf, lightDirection, lightInfinity );
    }
}
void rIndexedShadowVolume_c::fromPrecalculatedStencilShadowCaster( const class r_stencilShadowCaster_c* ssvCaster, const vec3_c& light )
{
    ssvCaster->generateShadowVolume( this, light );
}
void rEntityShadowVolume_c::addDrawCall()
{
    rf_currentEntity = this->ent;
    this->data.addDrawCall();
}
