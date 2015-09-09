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
//  File name:   frustum.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Simple frustum class (defined by 6 planes)
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

// frustum.cpp
#include "frustum.h"
#include "axis.h"
#include "aabb.h"
#include <shared/autoCvar.h>

static aCvar_c frustum_noCull( "frustum_noCull", "0" );

cullResult_e frustum_c::cull( const aabb& bb ) const
{
	if ( frustum_noCull.getInt() )
	{
		return CULL_IN;
	}
#if 0
	for ( u32 i = 0; i < FRP_NUM_FRUSTUM_PLANES; i++ )
	{
		if ( planes[i].onSide( bb ) == SIDE_BACK )
			return CULL_OUT;
	}
	return CULL_CLIP;
#else
	// I need to differentiate CULL_CLIP and CULL_IN
	// in order to avoid some reduntant frustum culling
	// in BSP rendering code
	bool clip = false;
	for ( u32 i = 0; i < FRP_NUM_FRUSTUM_PLANES; i++ )
	{
		int side = planes[i].onSide( bb );
		if ( side == SIDE_BACK )
			return CULL_OUT;
		if ( side == SIDE_CROSS )
		{
			clip = true;
		}
	}
	if ( clip )
	{
		return CULL_CLIP;
	}
	return CULL_IN;
#endif
}
cullResult_e frustum_c::cullSphere( const class vec3_c& p, float radius ) const
{
	if ( frustum_noCull.getInt() )
	{
		return CULL_IN;
	}
	bool clip = false;
	for ( u32 i = 0; i < FRP_NUM_FRUSTUM_PLANES; i++ )
	{
		int side = planes[i].onSide( p, radius );
		if ( side == SIDE_BACK )
			return CULL_OUT;
		if ( side == SIDE_CROSS )
		{
			clip = true;
		}
	}
	if ( clip )
	{
		return CULL_CLIP;
	}
	return CULL_IN;
}

void frustum_c::setup( float fovX, float fovY, float zFar, const axis_c& axis, const vec3_c& origin )
{
	//this->eye = origin;
#if 1
	float ang = fovX / 180 * M_PI * 0.5f;
	float xs = sin( ang );
	float xc = cos( ang );
	
	planes[0].norm = axis[0] * xs;
	planes[0].norm.vectorMA( planes[0].norm, axis[1], xc );
	
	planes[1].norm = axis[0] * xs;
	planes[1].norm.vectorMA( planes[1].norm, axis[1], -xc );
	
	ang = fovY / 180 * M_PI * 0.5f;
	xs = sin( ang );
	xc = cos( ang );
	
	planes[2].norm = axis[0] * xs;
	planes[2].norm.vectorMA( planes[2].norm, axis[2], xc );
	
	planes[3].norm = axis[0] * xs;
	planes[3].norm.vectorMA( planes[3].norm, axis[2], -xc );
	
	for ( u32 i = 0; i < 4; i++ )
	{
		//planes[i].type = PLANE_NON_AXIAL;
		planes[i].dist = -origin.dotProduct( planes[i].norm );
		//planes[i].setSignBits();
	}
#if 1
	//fifth plane (closing one...)
	planes[4].norm = -axis[0];
	planes[4].dist = -planes[4].norm.dotProduct( origin + ( axis[0] * zFar ) );
	//planes[4].type = PLANE_NON_AXIAL;
	//planes[4].setSignBits();
#endif
#else
	
#endif
}
void frustum_c::setupExt( float fovX, float viewWidth, float viewHeight, float zFar, const class axis_c& axis, const class vec3_c& origin )
{
	float x = viewWidth / tan( fovX / 360 * M_PI );
	float fovY = atan2( viewHeight, x ) * 360 / M_PI;
	setup( fovX, fovY, zFar, axis, origin );
}
bool PlanesGetIntersectionPoint( const plane_c& plane1, const plane_c& plane2, const plane_c& plane3, vec3_c& out )
{
	// http://www.cgafaq.info/wiki/Intersection_of_three_planes
	
	vec3_c n1n2;
	n1n2.crossProduct( plane1.norm, plane2.norm );
	vec3_c n2n3;
	n2n3.crossProduct( plane2.norm, plane3.norm );
	vec3_c n3n1;
	n3n1.crossProduct( plane3.norm, plane1.norm );
	
	float denom = plane1.norm.dotProduct( n2n3 );
	
	out.clear();
	// check if the denominator is zero (which would mean that no intersection is to be found
	if ( denom == 0 )
	{
		// no intersection could be found, return <0,0,0>
		return false;
	}
	out.vectorMA( out, n2n3, -plane1.dist );
	out.vectorMA( out, n3n1, -plane2.dist );
	out.vectorMA( out, n1n2, -plane3.dist );
	
	out *= ( 1.0f / denom );
	
	float d0 = plane1.distance( out );
	float d1 = plane2.distance( out );
	float d2 = plane3.distance( out );
	
	return true;
}
void frustum_c::getBounds( class aabb& out ) const
{
	vec3_c farCorners[4];
	PlanesGetIntersectionPoint( planes[FRP_LEFT_PLANE], planes[FRP_TOP_PLANE], planes[FRP_FAR_PLANE], farCorners[0] );
	PlanesGetIntersectionPoint( planes[FRP_RIGHT_PLANE], planes[FRP_TOP_PLANE], planes[FRP_FAR_PLANE], farCorners[1] );
	PlanesGetIntersectionPoint( planes[FRP_RIGHT_PLANE], planes[FRP_BOTTOM_PLANE], planes[FRP_FAR_PLANE], farCorners[2] );
	PlanesGetIntersectionPoint( planes[FRP_LEFT_PLANE], planes[FRP_BOTTOM_PLANE], planes[FRP_FAR_PLANE], farCorners[3] );
	out.addPoint( farCorners[0] );
	out.addPoint( farCorners[1] );
	out.addPoint( farCorners[2] );
	out.addPoint( farCorners[3] );
#if 0
	vec3_c nearCorners[4];
	PlanesGetIntersectionPoint( planes[FRP_LEFT_PLANE], planes[FRP_TOP_PLANE], planes[FRP_FAR_NEAR], nearCorners[0] );
	PlanesGetIntersectionPoint( planes[FRP_RIGHT_PLANE], planes[FRP_TOP_PLANE], planes[FRP_FAR_NEAR], nearCorners[1] );
	PlanesGetIntersectionPoint( planes[FRP_RIGHT_PLANE], planes[FRP_BOTTOM_PLANE], planes[FRP_FAR_NEAR], nearCorners[2] );
	PlanesGetIntersectionPoint( planes[FRP_LEFT_PLANE], planes[FRP_BOTTOM_PLANE], planes[FRP_FAR_NEAR], nearCorners[3] );
	out.addPoint( nearCorners[0] );
	out.addPoint( nearCorners[1] );
	out.addPoint( nearCorners[2] );
	out.addPoint( nearCorners[3] );
#else
	vec3_c nearCorner;
	PlanesGetIntersectionPoint( planes[FRP_LEFT_PLANE], planes[FRP_RIGHT_PLANE], planes[FRP_BOTTOM_PLANE], nearCorner );
	out.addPoint( nearCorner );
	//out.addPoint(eye);
#endif
}