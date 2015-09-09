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
//  File name:   world.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "World.h"
#include "BaseEntity.h"
#include "../g_local.h"
#include <api/serverAPI.h>
#include <shared/autoCvar.h>
#include <shared/keyValuesListener.h>

static aCvar_c g_printGlobalWaterForces("g_printGlobalWaterForces","0");

World g_world;

World::World() {
	hasWaterLevel = false;
	waterLevel = -1.f;
	farPlane = 8000.f;
}
void World::setKeyValue(const char *key, const char *value) {
	if(!stricmp(key,"skymaterial")) {
		this->skyMaterial = value;
		g_server->SetConfigstring(CS_WORLD_SKYMATERIAL, value);
	} else if(!stricmp(key,"waterlevel")) {
		this->waterLevel = atof(value);
		hasWaterLevel = true;
		g_server->SetConfigstring(CS_WORLD_WATERLEVEL, value);
	} else if(!stricmp(key,"farPlane")) {
		// zFar value
		this->farPlane = atof(value);
		//hasFarPlane = true;
		g_server->SetConfigstring(CS_WORLD_FARPLANE, value);
	} else {

	}
}
void World::iterateKeyValues(class keyValuesListener_i *listener) const {
	if(this->skyMaterial.length()) {
		listener->addKeyValue("skymaterial",this->skyMaterial);
	}
	if(this->hasWaterLevel) {
		listener->addKeyValue("waterlevel",this->waterLevel);
	}
}
void World::runGlobalWaterPhysics() {
	for(u32 i = MAX_CLIENTS; i < level.num_entities; i++) {
		edict_s *ed =  &g_entities[i];
		if(ed->s == 0)
			continue;
		BaseEntity *be = ed->ent;
		if(be == 0)
			return;
		if(be->getOrigin().z > waterLevel)
			continue;
		if(g_printGlobalWaterForces.getInt()) {
			G_Printf("World::runGlobalWaterPhysics: entity %i is in water\n",i);
		}
		be->runWaterPhysics(waterLevel);
	}
}
void World::runWorldFrame() {
	if(hasWaterLevel) {
		runGlobalWaterPhysics();
	}
}