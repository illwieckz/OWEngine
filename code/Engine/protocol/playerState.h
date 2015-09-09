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
//  File name:   playerState.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __PLAYERSTATE_H__
#define __PLAYERSTATE_H__

#include <protocol/entityState.h>

// playerState_s is the information needed by both the client and server
// to predict player motion and actions
// nothing outside of pmove should modify these, or some degree of prediction error
// will occur

// you can't add anything to this without modifying the code in msg.c

// playerState_s is a full superset of entityState_s as it is used by players,
// so if a playerState_s is transmitted, the entityState_s can be fully derived
// from it.
struct playerState_s : public entityState_s
{
	int         commandTime;    // cmd->serverTime of last executed command
	
	int         clientNum;      // ranges from 0 to MAX_CLIENTS-1
	
	vec3_c      velocity;
	
	int         delta_angles[3];    // add to command angles to get view direction
	// changed by spawns, rotating objects, and teleporters
	
	//  int         groundEntityNum;// ENTITYNUM_NONE = in air
	
	// current mainhand weapon / item
	int         curWeaponEntNum;
	
	// index in CS_MODELS
	int         customViewRModelIndex; // 0 means that there is no custom view render model
	// index in CS_ANIMATIONS
	int         viewModelAnim;
	// animation flags
	int         viewModelAnimFlags;
	
	vec3_c      viewangles;     // for fixed views
	int         viewheight;
	// viewmodel (weapon) offset/angles,
	// relative to player eye
	// (this is set through "def_viewStyle" key in weapon .def)
	vec3_c      viewModelAngles;
	vec3_c      viewModelOffset;
	//  view weapon ammo data (only for display)
	int         viewWeaponMaxClipSize;
	int         viewWeaponCurClipSize;
	// int      viewWeaponAmmoTypeIndex;
	
	// not communicated over the net at all
	int         ping;           // server to game info for scoreboard
	
	playerState_s()
	{
		delta_angles[0] = delta_angles[1] = delta_angles[2] = 0;
		curWeaponEntNum = ENTITYNUM_NONE;
		customViewRModelIndex = 0;
		viewangles.set( 0, 0, 0 );
		viewheight = 0;
		ping = 0;
		viewWeaponMaxClipSize = 0;
		viewWeaponCurClipSize = 0;
		viewModelAngles.set( 0, 0, 0 );
		viewModelOffset.set( 0, 0, 0 );
	}
	bool isOnGround() const
	{
		return ( groundEntityNum != ENTITYNUM_NONE );
	}
};
#endif // __PLAYERSTATE_H__
