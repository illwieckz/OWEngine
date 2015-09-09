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
//  File name:   Logic_Blinker.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "Logic_Blinker.h"
#include "../g_local.h"

DEFINE_CLASS(Logic_Blinker, "BaseEntity");
DEFINE_CLASS_ALIAS(Logic_Blinker, qio_logic_blinker);

Logic_Blinker::Logic_Blinker() {
	bBlinkerState = false;
	wait_enabled = 1000;
	wait_disabled = 1000;
	lastSwitchTime = level.time;
}
void Logic_Blinker::runFrame() {
	u32 timePassed = level.time - lastSwitchTime;
	u32 cycleTime = bBlinkerState ? wait_disabled : wait_enabled;
	while(timePassed > cycleTime) {
		lastSwitchTime += cycleTime;
		if(bBlinkerState) {
			bBlinkerState = false;
			//if(target) {
				G_HideEntitiesWithTargetName(getTarget());
			//}
		} else {
			bBlinkerState = true;
			//if(target) {
				G_ShowEntitiesWithTargetName(getTarget());
			//}
		}
		timePassed = level.time - lastSwitchTime;
		cycleTime = bBlinkerState ? wait_disabled : wait_enabled;
	}
}
void Logic_Blinker::setKeyValue(const char *key, const char *value) {
	if(!stricmp(key,"wait_enabled")) {
		wait_enabled = atoi(value);
	} else if(!stricmp(key,"wait_disabled")) {
		wait_disabled = atoi(value);
	} else {
		BaseEntity::setKeyValue(key,value);
	}
}
