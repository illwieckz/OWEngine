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
//  File name:   Weapon.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "../g_local.h"
#include "Weapon.h"
#include "Player.h"
#include "Light.h"
#include <api/coreAPI.h>
#include <api/declManagerAPI.h>
#include <api/entityDeclAPI.h>
#include <api/modelDeclAPI.h>
#include <api/entDefAPI.h>
#include <api/serverAPI.h>

DEFINE_CLASS(Weapon, "ModelEntity");
DEFINE_CLASS_ALIAS(Weapon, idItem);
// for Prey weapons (HumanHead items)
DEFINE_CLASS_ALIAS(Weapon, hhItem);
DEFINE_CLASS_ALIAS(Weapon, hhWeaponRifle);

Weapon::Weapon() {
	owner = 0;
	autoFire = true;
	delayBetweenShots = 250;
	lastShotTime = 0;
	invWeaponDecl = 0;
	clipSize = 1;
	curClipSize = 1;
	raiseTime = 0;
	lowerTime = 0;
	reloadTime = 0;
	flashColor.set(1,1,1);
	flashRadius = 100.f;
	shotBulletCount = 1;
	spreadDist = 1000.f;
	maxSpread = 1.f;
}
Weapon::~Weapon() {

}
void Weapon::setViewModel(const char *newViewModelName) {
	model_view = newViewModelName;

	modelDeclAPI_i *decl = g_declMgr->registerModelDecl(model_view);
	if(decl) {
		raiseTime = decl->getAnimationTimeMSec("raise");
		lowerTime = decl->getAnimationTimeMSec("putaway");
		delayBetweenShots = decl->getAnimationTimeMSec("fire");
		if(delayBetweenShots == 0) {
			delayBetweenShots = 10;
		}
		reloadTime = decl->getAnimationTimeMSec("reload");
		if(reloadTime == 0) {
			reloadTime = 10;
		}
	}
}
void Weapon::setKeyValue(const char *key, const char *value) {
	if(!stricmp(key,"model_view")) {
		// use this model for first person view
		this->setViewModel(value);
	} else if(invWeaponDecl && !stricmp(key,"model") && model_view.length()==0) {
		// "model" keyword inside a "inv_weapon" entdefs sets the weapons viewModel
		this->setViewModel(value);
#if 0
	} else if(!stricmp(key,"model")) {
		this->setRenderModel(value);
		this->setColModel(value);
#endif
	} else if(!stricmp(key,"inv_weapon")) {	
		if(invWeaponDecl)
			return;
		invWeaponDecl = g_declMgr->registerEntityDecl(value);
		if(invWeaponDecl) {
			applyKeyValues(invWeaponDecl->getEntDefAPI());
			invWeaponDecl = 0;
		}
	} else if(!stricmp(key,"clipSize")) {
		clipSize = atoi(value);
		curClipSize = clipSize;
	} else if(!stricmp(key,"ddaName")) {
		ddaName = value;
	} else if(!stricmp(key,"weaponName")) {
		weaponName = value;
	} else if(!stricmp(key,"continuousFire")) {
	} else if(!stricmp(key,"ammoRequired")) {
	} else if(!stricmp(key,"ammoType")) {

	} else if(!stricmp(key,"fireRate")) {

	} else if(!stricmp(key,"def_viewStyle")) {
		// Quake4 (???) viewStyle dict
		entityDeclAPI_i *viewStyleDef = g_declMgr->registerEntityDecl(value);
		if(viewStyleDef) {
			applyKeyValues(viewStyleDef->getEntDefAPI());
		}
	} else if(!stricmp(key,"viewoffset")) {
		// set from "def_viewStyle" entityDef
		viewOffset.fromString(value);
	} else if(!stricmp(key,"viewangles")) {
		// set from "def_viewStyle" entityDef
		viewAngles.fromString(value);
	} else if(!stricmp(key,"def_projectile")) {
		def_projectile = value;
	} else if(!stricmp(key,"smoke_muzzle")) {
		smoke_muzzle = value;
	} else if(!stricmp(key,"flashColor")) {
		// example usage: "flashColor"	"1 0.8 0.4"
		flashColor.fromString(value);
	} else if(!stricmp(key,"flashRadius")) {
		// example usage: "flashRadius"	"120"
		flashRadius = atof(value);
	} else if(!stricmp(key,"shotBulletCount")) {
		shotBulletCount = atoi(value);
	} else if(!stricmp(key,"maxSpread")) {
		maxSpread = atof(value);
	} else if(!stricmp(key,"spreadDist")) {
		spreadDist = atof(value);
	} else {
		ModelEntity::setKeyValue(key,value);
	}
}
bool Weapon::doUse(class Player *activator) {
	if(owner) {
		g_core->RedWarning("Weapon::doUse: weapon is already in use\n");
		return true; // this item cannot be carried
	}
	if(activator->canPickUpWeapon(this)==false)
		return true;
	owner = activator;
	activator->addWeapon(this);
	this->destroyPhysicsObject();
	this->unlink();
	// run callback (so Weapon child classes know they were picked up)
	this->onWeaponPickedUp();
	return true; // this item cannot be carried
}

BaseEntity *Weapon::getOwner() const {
	return owner.getPtr();
}
void Weapon::onFireKeyHeld() {
	if(autoFire) {
		if(canFireAgain()) {
			this->lastShotTime = level.time;
			doWeaponAttack();
		}
	}
}
void Weapon::onFireKeyDown() {
	if(canFireAgain()) {
		this->lastShotTime = level.time;
		doWeaponAttack();
	}
}
void Weapon::onSecondaryFireKeyHeld() {

}
void Weapon::onSecondaryFireKeyDown() {

}
void Weapon::onSecondaryFireKeyUp() {

}

void Weapon::doWeaponAttack() {
	vec3_c muzzlePos, muzzleDir;
	BaseEntity *skip;
	if(owner) {
		skip = owner;
		curClipSize--;
		muzzlePos = owner->getEyePos();
		muzzleDir = owner->getViewAngles().getForward();
	} else {
		skip = this;
		muzzlePos = this->getOrigin();
		muzzleDir = this->getForward();
	}
	if(flashRadius > 0.f) {
#if 0
		Light *l = new Light;
		l->setOrigin(muzzlePos+muzzleDir*20.f);
		l->setRadius(flashRadius);
		l->removeAfterDelay(100);
#else
		g_server->SendServerCommand(-1,va("doLocalMuzzleFlash %f",flashRadius));
#endif
	}
	if(def_projectile.size()) {
		G_FireProjectile(def_projectile.c_str(),muzzlePos,muzzleDir,skip);
	} else {
		//G_BulletAttack(muzzlePos,muzzleDir,skip);
		G_MultiBulletAttack(muzzlePos,muzzleDir,skip,shotBulletCount,maxSpread,spreadDist);
	}
}
void Weapon::doWeaponAttackSecondary() {

}
bool Weapon::canFireAgain() const {
	u32 timeElapsed = level.time - this->lastShotTime;
	if(timeElapsed < this->delayBetweenShots) {
		g_core->Print("Weapon::canFireAgain: cant fire because elapsed time is %i and delay is %i\n",timeElapsed,this->delayBetweenShots);
		return false;
	}
	g_core->Print("Weapon::canFireAgain: CAN FIRE, because elapsed time is %i and delay is %i\n",timeElapsed,this->delayBetweenShots);
	return true;
}