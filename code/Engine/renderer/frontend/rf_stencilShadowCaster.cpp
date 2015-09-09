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
//  File name:   rf_stencilShadowCaster.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "rf_stencilShadowCaster.h"
#include "rf_surface.h"
#include <shared/autoCvar.h>
#include <api/coreAPI.h>

static aCvar_c rf_stencilShadowCaster_verboseGeneration( "rf_stencilShadowCaster_verboseGeneration", "0" );

void r_stencilShadowCaster_c::addTriangle( const vec3_c& p0, const vec3_c& p1, const vec3_c& p2 )
{
	u32 i0 = points.registerVec3( p0 );
	u32 i1 = points.registerVec3( p1 );
	u32 i2 = points.registerVec3( p2 );
	const vec3_c& v0 = points[i0].v;
	const vec3_c& v1 = points[i1].v;
	const vec3_c& v2 = points[i2].v;
	rTri32_s& nt = tris.pushBack();
	nt.indices[0] = i0;
	nt.indices[1] = i1;
	nt.indices[2] = i2;
	nt.plane.fromThreePoints( v0, v1, v2 );
}
void r_stencilShadowCaster_c::addEdge( u32 triNum, u32 v0, u32 v1 )
{
	rEdge_s* e = edges.getArray();
	for ( u32 i = 0; i < edges.size(); i++, e++ )
	{
		if ( e->tris[0] != e->tris[1] )
			continue; // already matched
		if ( e->verts[0] == v0 && e->verts[1] == v1 )
		{
			e->tris[1] = triNum;
			c_matchedEdges++;
			return;
		}
		if ( e->verts[0] == v1 && e->verts[1] == v0 )
		{
			e->tris[1] = -triNum - 1;
			c_matchedEdges++;
			return;
		}
	}
	rEdge_s& newEdge = edges.pushBack();
	newEdge.verts[0] = v0;
	newEdge.verts[1] = v1;
	newEdge.tris[0] = newEdge.tris[1] = triNum;
}
void r_stencilShadowCaster_c::calcEdges()
{
	c_matchedEdges = 0;
	const rTri32_s* t = tris.getArray();
	for ( u32 i = 0; i < tris.size(); i++, t++ )
	{
		addEdge( i, t->indices[0], t->indices[1] );
		addEdge( i, t->indices[1], t->indices[2] );
		addEdge( i, t->indices[2], t->indices[0] );
	}
	c_unmatchedEdges = edges.size() - c_matchedEdges;
	if ( rf_stencilShadowCaster_verboseGeneration.getInt() )
	{
		g_core->Print( "r_stencilShadowCaster_c::calcEdges: %i edges (%i matched, %i unmatched) from %i triangles\n",
					   edges.size(), c_matchedEdges, c_unmatchedEdges, tris.size() );
	}
}
void r_stencilShadowCaster_c::addRSurface( const class r_surface_c* sf )
{
	for ( u32 i = 0; i < sf->getNumTris(); i++ )
	{
		vec3_c points[3];
		sf->getTriangle( i, points[0], points[1], points[2] );
		this->addTriangle( points[0], points[1], points[2] );
	}
}
void r_stencilShadowCaster_c::addRModel( const class r_model_c* mod )
{
	this->points.setEqualVertexEpsilon( 0.f );
	for ( u32 i = 0; i < mod->getNumSurfs(); i++ )
	{
		addRSurface( mod->getSurf( i ) );
	}
}
#include "rf_shadowVolume.h"
void r_stencilShadowCaster_c::generateShadowVolume( class rIndexedShadowVolume_c* out, const vec3_c& light ) const
{
	arraySTD_c<byte> bFrontFacing;
	bFrontFacing.resize( tris.size() );
	bFrontFacing.nullMemory();
	// mark triangles frontfacing the light and
	// add front cap and back cap made
	// from triangles frontfacing the light
	const rTri32_s* t = tris.getArray();
	for ( u32 i = 0; i < tris.size(); i++, t++ )
	{
		float d = t->plane.distance( light );
		if ( d > 0 )
		{
			bFrontFacing[i] = 1;
			const vec3_c& v0 = points.getVec3( t->indices[0] );
			const vec3_c& v1 = points.getVec3( t->indices[1] );
			const vec3_c& v2 = points.getVec3( t->indices[2] );
			out->addFrontCapAndBackCapForTriangle( v0, v1, v2, light );
		}
	}
	if ( this->c_unmatchedEdges == 0 )
	{
		// the ideal case: model has no unmatched edges
		const rEdge_s* e = edges.getArray();
		for ( u32 i = 0; i < edges.size(); i++, e++ )
		{
			if ( e->isMatched() == false )
			{
				continue;
			}
			int t0 = e->getTriangleIndex( 0 );
			int t1 = e->getTriangleIndex( 1 );
			if ( bFrontFacing[t0] == bFrontFacing[t1] )
				continue;
			const vec3_c& v0 = points.getVec3( e->verts[0] );
			const vec3_c& v1 = points.getVec3( e->verts[1] );
			if ( bFrontFacing[t0] )
			{
				out->addEdge( v0, v1, light );
			}
			else
			{
				out->addEdge( v1, v0, light );
			}
		}
	}
	else
	{
		// try to handle models with unmatched edges as well
		//arraySTD_c<byte> bTriAdded;
		//bTriAdded.resize(tris.size());
		//bTriAdded.nullMemory();
		const rEdge_s* e = edges.getArray();
		for ( u32 i = 0; i < edges.size(); i++, e++ )
		{
			if ( e->isMatched() == false )
			{
				if ( bFrontFacing[e->tris[0]] == false )
					continue;
#if 1
				const vec3_c& v0 = points.getVec3( e->verts[0] );
				const vec3_c& v1 = points.getVec3( e->verts[1] );
				out->addEdge( v0, v1, light );
#else
				u32 t = e->tris[0];
				if ( bTriAdded[t] )
				{
					continue;
				}
				bTriAdded[t] = 1;
				const vec3_c& tv0 = points.getVec3( tris[t].indices[0] );
				const vec3_c& tv1 = points.getVec3( tris[t].indices[1] );
				const vec3_c& tv2 = points.getVec3( tris[t].indices[2] );
				out->addEdge( tv0, tv1, light );
				out->addEdge( tv1, tv2, light );
				out->addEdge( tv2, tv0, light );
#endif
				continue;
			}
			int t0 = e->getTriangleIndex( 0 );
			int t1 = e->getTriangleIndex( 1 );
			if ( bFrontFacing[t0] == bFrontFacing[t1] )
				continue;
			const vec3_c& v0 = points.getVec3( e->verts[0] );
			const vec3_c& v1 = points.getVec3( e->verts[1] );
			if ( bFrontFacing[t0] )
			{
				out->addEdge( v0, v1, light );
			}
			else
			{
				out->addEdge( v1, v0, light );
			}
		}
	}
}
