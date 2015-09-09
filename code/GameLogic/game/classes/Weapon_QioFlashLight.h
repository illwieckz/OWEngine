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
//  File name:   Weapon_QioFlashLight.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: FlashLight weapon
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __WEAPON_QIOFLASHLIGHT_H__
#define __WEAPON_QIOFLASHLIGHT_H__

#include "Weapon.h"
#include "Light.h"

class Weapon_QioFlashLight : public Weapon {
	safePtr_c<Light> myLight;
public:
	Weapon_QioFlashLight();
	~Weapon_QioFlashLight();

	DECLARE_CLASS( Weapon_QioFlashLight );

	virtual bool hasEmptyClip() const {
		return false;
	}

	virtual void doWeaponAttack() {

	}
	virtual void doWeaponAttackSecondary() {

	}

	void disableFlashLight();
	void enableFlashLight();

	virtual void onFireKeyDown();
};

#endif // __WEAPON_QIOFLASHLIGHT_H__
