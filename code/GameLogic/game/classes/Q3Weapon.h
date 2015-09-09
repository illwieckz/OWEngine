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
//  File name:   Q3Weapon.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __Q3WEAPON_H__
#define __Q3WEAPON_H__

#include "Weapon.h"

class Q3Weapon : public Weapon
{
		enum quake3WeaponType_e q3WeaponType;
		struct railgunAttackMaterials_s* railMats;
	public:
		Q3Weapon();
		~Q3Weapon();
		
		DECLARE_CLASS( Q3Weapon );
		
		virtual void doWeaponAttack();
		
		virtual void setKeyValue( const char* key, const char* value );
};

#endif // __Q3WEAPON_H__
