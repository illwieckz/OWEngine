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
//  File name:   btp_cMod2BulletShape.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

class btConvexHullShape* BT_CModelHullToConvex( const class cmHull_i* h, const vec3_c* ofs = 0 );
class btConvexHullShape* BT_ConvexHullShapeFromVerticesArray( const btAlignedObjectArray<btVector3>& vertices );
void BT_ConvertVerticesArrayFromQioToBullet( btAlignedObjectArray<btVector3>& vertices );
btConvexHullShape* BT_CModelTriMeshToConvex( const class cmTriMesh_i* triMesh, const vec3_c* ofs = 0 );
btCollisionShape* BT_CModelToBulletCollisionShape( const class cMod_i* cModel, bool bIsStatic, class vec3_c* extraCenterOfMassOffset = 0 );
void BT_AddCModelToCompoundShape( btCompoundShape* compound, const class btTransform& localTrans, const class cMod_i* cmodel );

