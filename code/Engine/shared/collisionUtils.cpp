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
//  File name:   collisionUtils.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "../math/vec3.h"
#include "../math/aabb.h"
#include <float.h>

// swap the content of two types
template< class T >
inline void G_swap( T& a, T& b )
{
	T c = a;
	a = b;
	b = c;
}

template<class T> inline T  max( T x, T y )
{
	return ( x > y ) ? x : y;
}
template<class T> inline T  min( T x, T y )
{
	return ( x < y ) ? x : y;
}

// Copyright 2001, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

// Assume that classes are already given for the objects:
//    Point and vec3_c with
//        coordinates {float x, y, z;}
//        operators for:
//            == to test equality
//            != to test inequality
//            (vec3_c)0 = (0,0,0)         (null vector)
//            Point  = Point � vec3_c
//            vec3_c = Point - Point
//            vec3_c = Scalar * vec3_c    (scalar product)
//            vec3_c = vec3_c * vec3_c    (cross product)
//    Line and Ray and Segment with defining points {Point P0, P1;}
//        (a Line is infinite, Rays and Segments start at P0)
//        (a Ray extends beyond P1, but a Segment ends at P1)
//    Plane with a point and a normal {Point V0; vec3_c n;}
//    Triangle with defining vertices {Point V0, V1, V2;}
//    Polyline and Polygon with n vertices {int n; Point *V;}
//        (a Polygon has V[n]=V[0])
//===================================================================

#define SMALL_NUM  0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)

// intersect_RayTriangle(): intersect a ray with a 3D triangle
//    Input:  a ray R, and a triangle T
//    Output: *I = intersection point (when it exists)
//    Return: -1 = triangle is degenerate (a segment or point)
//             0 = disjoint (no intersect)
//             1 = intersect in unique point I1
//             2 = are in the same plane
int CU_RayTraceTriangle( const vec3_c& rayStart, const vec3_c& rayEnd,
						 const vec3_c& pointA, const vec3_c& pointB, const vec3_c& pointC, vec3_c* out )
{
	vec3_c    u, v, n;             // triangle vectors
	vec3_c    dir, w0, w;          // ray vectors
	float     r, a, b;             // params to calc ray-plane intersect
	
	// get triangle edge vectors and plane normal
	u = pointB - pointA;
	v = pointC - pointA;
	
	n = v.crossProduct( u ); //n = u * v;             // cross product
	//    n = u.crossProduct(v);//n = u * v;             // cross product
	if ( n.x == 0 && n.y == 0 && n.z == 0 )          // triangle is degenerate
		return -1;                 // do not deal with this case
		
	dir = rayEnd - rayStart;             // ray direction vector
	w0 = rayStart - pointA;
	a = -dot( n, w0 );
	b = dot( n, dir );
	if ( fabs( b ) < SMALL_NUM )   // ray is parallel to triangle plane
	{
		if ( a == 0 )              // ray lies in triangle plane
			return 2;
		else return 0;             // ray disjoint from plane
	}
	
	// get intersect point of ray with triangle plane
	r = a / b;
	if ( r < 0.0 )                 // ray goes away from triangle
		return 0;                  // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect
	
	*out = rayStart + r * dir;           // intersect point of ray and plane
	
	// is out inside T?
	float    uu, uv, vv, wu, wv, D;
	uu = dot( u, u );
	uv = dot( u, v );
	vv = dot( v, v );
	w = *out - pointA;
	wu = dot( w, u );
	wv = dot( w, v );
	D = uv * uv - uu * vv;
	
	// get and test parametric coords
	float s, t;
	s = ( uv * wv - vv * wu ) / D;
	if ( s < 0.0 || s > 1.0 )      // out is outside T
		return 0;
	t = ( uv * wu - uu * wv ) / D;
	if ( t < 0.0 || ( s + t ) > 1.0 ) // out is outside T
		return 0;
		
	return 1;                      // out is in T
}

//Helper function for Line/AABB test.  Tests collision on a single dimension
//Param:    Start of line, Direction/length of line,
//          Min value of AABB on plane, Max value of AABB on plane
//          Enter and Exit "timestamps" of intersection (OUT)
//Return:   True if there is overlap between Line and AABB, False otherwise
//Note:     Enter and Exit are used for calculations and are only updated in case of intersection
static bool Line_AABB_1d( float start, float dir, float min, float max, float& enter, float& exit )
{
	//If the line segment is more of a point, just check if it's within the segment
	if ( fabs( dir ) < 1.0E-8 )
		return ( start >= min && start <= max );
		
	//Find if the lines overlap
	float   ooDir = 1.0f / dir;
	float   t0 = ( min - start ) * ooDir;
	float   t1 = ( max - start ) * ooDir;
	
	//Make sure t0 is the "first" of the intersections
	if ( t0 > t1 )
		G_swap( t0, t1 );
		
	//Check if intervals are disjoint
	if ( t0 > exit || t1 < enter )
		return false;
		
	//Reduce interval based on intersection
	if ( t0 > enter )
		enter = t0;
	if ( t1 < exit )
		exit = t1;
		
	return true;
}

//Check collision between a line segment and an AABB
//Param:    Start point of line segement, End point of line segment,
//          One corner of AABB, opposite corner of AABB,
//          Location where line hits the AABB (OUT)
//Return:   True if a collision occurs, False otherwise
//Note:     If no collision occurs, OUT param is not reassigned and is not considered useable
bool CU_IntersectLineAABB( const vec3_c& s, const vec3_c& e, const aabb& bb, vec3_c& hitPoint )
{
#if 1
	float       enter = 0.0f;
	float       exit = 1.0f;
	vec3_c    dir = e - s;
	
	//Check each dimension of Line/AABB for intersection
	if ( !Line_AABB_1d( s.x, dir.x, bb.mins.x, bb.maxs.x, enter, exit ) )
		return false;
	if ( !Line_AABB_1d( s.y, dir.y, bb.mins.y, bb.maxs.y, enter, exit ) )
		return false;
	if ( !Line_AABB_1d( s.z, dir.z, bb.mins.z, bb.maxs.z, enter, exit ) )
		return false;
		
	//If there is intersection on all dimensions, report that point
	hitPoint = s + dir * enter;
	return true;
#else
	vec3_c delta = ( e - s );
	vec3_c dir = delta;
	dir.normalize();
	vec3_c dirfrac;
	// r.dir is unit direction vector of ray
	dirfrac.x = 1.0f / dir.x;
	dirfrac.y = 1.0f / dir.y;
	dirfrac.z = 1.0f / dir.z;
	// bb.mins is the corner of AABB with minimal coordinates - left bottom, bb.maxs is maximal corner
	// s is origin of ray
	float t1 = ( bb.mins.x - s.x ) * dirfrac.x;
	float t2 = ( bb.maxs.x - s.x ) * dirfrac.x;
	float t3 = ( bb.mins.y - s.y ) * dirfrac.y;
	float t4 = ( bb.maxs.y - s.y ) * dirfrac.y;
	float t5 = ( bb.mins.z - s.z ) * dirfrac.z;
	float t6 = ( bb.maxs.z - s.z ) * dirfrac.z;
	
	float tmin = max( max( min( t1, t2 ), min( t3, t4 ) ), min( t5, t6 ) );
	float tmax = min( min( max( t1, t2 ), max( t3, t4 ) ), max( t5, t6 ) );
	
	// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	if ( tmax < 0 )
	{
		//t = tmax;
		return false;
	}
	
	// if tmin > tmax, ray doesn't intersect AABB
	if ( tmin > tmax )
	{
		//  t = tmax;
		return false;
	}
	
	//t = tmin;
	hitPoint = tmin * dir + s;
	return true;
	
#endif
}

// The next two functions are
// from Real-Time Collision Detection by Christer Ericson, published
// by Morgan Kaufmann Publishers, Copyright 2005 Elsevier Inc
vec3_c CU_ClosestPointTriangle( const vec3_c& p, const vec3_c& a, const vec3_c& b, const vec3_c& c )
{
	vec3_c ab = b - a;
	vec3_c ac = c - a;
	vec3_c bc = c - b;
	
	// Compute parametric position s for projection P' of P on AB,
	// P' = A + s*AB, s = snom/(snom+sdenom)
	float snom = ( p - a ).dotProduct( ab );
	float sdenom = ( p - b ).dotProduct( a - b );
	
	// Compute parametric position t for projection P' of P on AC,
	// P' = A + t*AC, s = tnom/(tnom+tdenom)
	float tnom = ( p - a ).dotProduct( ac );
	float tdenom = ( p - c ).dotProduct( a - c );
	
	if ( snom <= 0.0f && tnom <= 0.0f )
		return a; // Vertex region early out
		
	// Compute parametric position u for projection P' of P on BC,
	// P' = B + u*BC, u = unom/(unom+udenom)
	float unom = ( p - b ).dotProduct( bc );
	float udenom = ( p - c ).dotProduct( b - c );
	
	if ( sdenom <= 0.0f && unom <= 0.0f )
		return b; // Vertex region early out
	if ( tdenom <= 0.0f && udenom <= 0.0f )
		return c; // Vertex region early out
		
		
	// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
	vec3_c n = ( b - a ).crossProduct( c - a );
	float vc = n.dotProduct( ( a - p ).crossProduct( b - p ) );
	// If P outside AB and within feature region of AB,
	// return projection of P onto AB
	if ( vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f )
		return a + snom / ( snom + sdenom ) * ab;
		
	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
	float va = n.dotProduct( ( b - p ).crossProduct( c - p ) );
	// If P outside BC and within feature region of BC,
	// return projection of P onto BC
	if ( va <= 0.0f && unom >= 0.0f && udenom >= 0.0f )
		return b + unom / ( unom + udenom ) * bc;
		
	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
	float vb = n.dotProduct( ( c - p ).crossProduct( a - p ) );
	// If P outside CA and within feature region of CA,
	// return projection of P onto CA
	if ( vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f )
		return a + tnom / ( tnom + tdenom ) * ac;
		
	// P must project inside face region. Compute Q using barycentric coordinates
	float u = va / ( va + vb + vc );
	float v = vb / ( va + vb + vc );
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}

bool CU_IntersectSphereTriangle( const vec3_c& center, float radius, const vec3_c& v0, const vec3_c& v1,  const vec3_c& v2 )
{
	vec3_c closest = CU_ClosestPointTriangle( center, v0, v1, v2 );
	float distSQ = closest.distSQ( center );
	float radiusSQ = radius * radius;
	if ( distSQ < radiusSQ )
		return true;
	return false;
}




