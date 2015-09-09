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
//  File name:   q3PlayerModelDeclAPI.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __Q3PLAYERMODELDECLAPI_H__
#define __Q3PLAYERMODELDECLAPI_H__

class q3PlayerModelAPI_i
{
	public:
		virtual u32 getNumTotalSurfaces() const = 0;
		virtual u32 getTotalTriangleCount() const = 0;
		virtual const class kfModelAPI_i* getLegsModel() const = 0;
		virtual const class kfModelAPI_i* getTorsoModel() const = 0;
		virtual const class kfModelAPI_i* getHeadModel() const = 0;
		virtual const char* getLegsModelName() const = 0;
		virtual const char* getTorsoModelName() const = 0;
		virtual const char* getHeadModelName() const = 0;
		virtual const struct q3AnimDef_s* getAnimCFGForIndex( u32 localAnimIndex ) const = 0;
		virtual int getTagNumForName( const char* boneName ) const = 0;
		virtual bool getTagOrientation( int tagNum, const struct singleAnimLerp_s& legs, const struct singleAnimLerp_s& torso, class matrix_c& out ) const = 0;
};

#endif // __Q3PLAYERMODELDECLAPI_H__

