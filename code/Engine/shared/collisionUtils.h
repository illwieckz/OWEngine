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
//  File name:   collisionUtils.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __COLLISIONUTILS_H__
#define __COLLISIONUTILS_H__

class vec3_c;

// intersect_RayTriangle(): intersect a ray with a 3D triangle
//    Input:  a ray R, and a triangle T
//    Output: *I = intersection point (when it exists)
//    Return: -1 = triangle is degenerate (a segment or point)
//             0 = disjoint (no intersect)
//             1 = intersect in unique point I1
//             2 = are in the same plane
int CU_RayTraceTriangle( const vec3_c& rayStart, const vec3_c& rayEnd,
						 const vec3_c& pointA, const vec3_c& pointB, const vec3_c& pointC, vec3_c* out );

//Check collision between a line segment and an AABB
//Param:    Start point of line segement, End point of line segment,
//          One corner of AABB, opposite corner of AABB,
//          Location where line hits the AABB (OUT)
//Return:   True if a collision occurs, False otherwise
//Note:     If no collision occurs, OUT param is not reassigned and is not considered useable
bool CU_IntersectLineAABB( const vec3_c& s, const vec3_c& e, const class aabb& bb, vec3_c& hitPoint );

bool CU_IntersectSphereTriangle( const vec3_c& center, float radius, const vec3_c& v0, const vec3_c& v1,  const vec3_c& v2 );

#endif // __COLLISIONUTILS_H__