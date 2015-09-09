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
//  File name:   afRagdollHelper.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __AFRAGDOLLHELPER_H__
#define __AFRAGDOLLHELPER_H__

#include <shared/skelUtils.h> // boneOrArray_c
#include <shared/array.h>
#include <shared/str.h>

class afRagdollHelper_c
{
	protected:
		boneOrArray_c bones;
		const class boneDefArray_c* boneDefs;
		const class skelAnimAPI_i* anim;
		const class afDeclAPI_i* af;
		const struct afPublicData_s* afd;
		const class modelDeclAPI_i* model;
		
		vec3_c getAFVec3Value( const struct afVec3_s& v );
		bool createConvexPointSoupForAFModel( const struct afModel_s& m, arraySTD_c<vec3_c>& outPoints );
		bool getBodyTransform( u32 bodyNum, matrix_c& out );
	public:
		afRagdollHelper_c();
		bool setupRagdollHelper( const char* afName );
		bool calcBoneParentBody2BoneOfsets( const char* afName, arraySTD_c<matrix_c>& out );
};

void UTIL_ContainedJointNamesArrayToJointIndexes( const arraySTD_c<str>& containedJoints, arraySTD_c<u32>& boneNumbers, const class skelAnimAPI_i* anim );

#endif // __AFRAGDOLLHELPER_H__
