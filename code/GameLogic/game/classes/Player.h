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
//  File name:   Player.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Game Client class header
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ModelEntity.h"

#include <shared/str.h>
#include <qcommon/q_shared.h> // for ucmd
#include <protocol/userCmd.h>
#include <protocol/playerState.h>

typedef enum {
	CON_DISCONNECTED,
	CON_CONNECTING,
	CON_CONNECTED
} clientConnected_t;

//
#define MAX_NETNAME			36

// client data that stays across multiple respawns, but is cleared
// on each level change or team change at ClientBegin()
typedef struct {
	clientConnected_t	connected;	
	userCmd_s	cmd;				// we would lose angles if not persistant
	bool	localClient;		// true if "ip" info key is "localhost"
	//int			maxHealth;			// for handicapping
	int			enterTime;			// level.time the client entered the game
} clientPersistant_t;

enum weaponState_e {
	// player has no weapon
	WP_NONE,
	// player is raising a new weapon (curWeapon pointer is valid, nextWeapon pointer is NULL)
	WP_RAISE,
	// player is holstering old weapon (curWeapon pointer is valid, nextWeapon is not-null while switching weapons)
	WP_PUTAWAY,
	// player weapon is ready to fire (curWeapon pointer is valid, nextWeapon is NULL)
	WP_IDLE,
	// player is reloading his weapon
	WP_RELOADING,
	// weapon is firing
	WP_FIRING,
};

// this structure is cleared on each ClientSpawn(),
// except for 'client->pers' and 'client->sess'

class Player : public ModelEntity {
	class physCharacterControllerAPI_i *characterController;
	str netName;
	safePtr_c<Weapon> curWeapon;
	safePtr_c<Weapon> nextWeapon;
	weaponState_e weaponState;
	u32 weaponTime;
	vec3_c characterControllerOffset;
	bool onGround; // this is always false if player is using "noclip"
	class playerAnimControllerAPI_i *animHandler;
	u32 lastDeathTime; // in msec
	u32 lastPainTime;
	u32 curPainAnimationTime;
	str curPainAnimationName;

	void updateCurWeaponAttachment();
	void setViewModelAnim(const char *animName, int animFlags);
	void updateCurWeaponClipSize();

	void setPlayerAnimBoth(enum sharedGameAnim_e type);
	void setPlayerAnimTorso(enum sharedGameAnim_e type);

	bool isPainAnimActive() const;

protected:
	void playPainAnimation(const char *newPainAnimationName, u32 animTime = 0);
public:
	Player();
	virtual ~Player();

	DECLARE_CLASS( Player );

	playerState_s	ps;				// communicated by server to clients

	// the rest of the structure is private to game
	clientPersistant_t	pers;

	bool noclip;

	int buttons;
	int oldButtons;
	int latchedButtons;
	
	bool useHeld; // we might use latchedButtons instead of it
	bool fireHeld;
	bool secondaryFireHeld;

	class VehicleCar *vehicle;
	// Half Life2 - style entity carrying
	safePtr_c<ModelEntity> carryingEntity;

	void disableCharacterController();
	void enableCharacterController();
	void setCharacterControllerZOffset(float ofs);
	void createCharacterControllerCapsule(float cHeight, float cRadius);
	void touchTriggers();
	void updatePlayerWeapon();
	void runPlayer();
	void onUseKeyDown();
	void onFireKeyHeld();
	void onFireKeyDown();
	void onSecondaryFireKeyHeld();
	void onSecondaryFireKeyDown();
	void onSecondaryFireKeyUp();
	void pickupPhysicsProp(class ModelEntity *ent);
	bool isCarryingEntity() const;
	void dropCarryingEntity();
	void setClientViewAngle(const vec3_c &angle);
	void setNetName(const char *newNetName);
	const char *getNetName() const;
	int getViewHeight() const;
	vec3_c getEyePos() const;
	virtual vec3_c getCModelOrigin() const {
		return getOrigin()+characterControllerOffset;
	}
	const vec3_c &getViewAngles() const {
		return ps.viewangles;
	}
	virtual void setOrigin(const vec3_c &newXYZ);
	virtual void setLinearVelocity(const vec3_c &newVel);
	void setVehicle(class VehicleCar *newVeh);
	void setPlayerModel(const char *newPlayerModelName);
	bool hasActiveWeapon() const {
		if(curWeapon.getPtr())
			return true;
		return false;
	}
	void toggleNoclip();

	void cmdSay(const char *msg);

	struct playerState_s *getPlayerState();

	virtual void postSpawn();

	virtual void onBulletHit(const vec3_c &hitPosWorld, const vec3_c &dirWorld, int damageCount);
	virtual void onDeath();

	virtual void runFrame() {
		runPlayer();
	}
	// called from Weapon::doUse
	void addWeapon(class Weapon *newWeapon);
	bool canPickUpWeapon(class Weapon *newWeapon);

	void dropCurrentWeapon();
};

#endif // __PLAYER_H__
