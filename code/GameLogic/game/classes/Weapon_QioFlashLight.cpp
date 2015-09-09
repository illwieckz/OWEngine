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
//  File name:   Weapon_QioFlashLight.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: FlashLight weapon
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "Weapon_QioFlashLight.h"

DEFINE_CLASS( Weapon_QioFlashLight, "Weapon" );

Weapon_QioFlashLight::Weapon_QioFlashLight()
{
	this->setDelayBetweenShots( 0 );
	this->fillClip( 0 );
}
Weapon_QioFlashLight::~Weapon_QioFlashLight()
{
	disableFlashLight();
}

void Weapon_QioFlashLight::disableFlashLight()
{
	delete myLight.getPtr();
}
void Weapon_QioFlashLight::enableFlashLight()
{
	if ( myLight.getPtr() )
		return;
	Light* l = new Light;
	l->setSpotLightRadius( 32.f );
	l->setRadius( 400.f );
	l->setParent( this, 0 );
	myLight = l;
}

void Weapon_QioFlashLight::onFireKeyDown()
{
	// toggle the light
	if ( myLight.getPtr() )
	{
		disableFlashLight();
		this->fillClip( 0 );
	}
	else
	{
		enableFlashLight();
		this->fillClip( 1 );
	}
}
