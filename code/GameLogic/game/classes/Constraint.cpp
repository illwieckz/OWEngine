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
//  File name:   Constraint.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "Constraint.h"
#include "../g_local.h"
#include <shared/eventBaseAPI.h>
#include <api/physAPI.h>
#include <api/coreAPI.h>

DEFINE_CLASS(Constraint, "BaseEntity");
DEFINE_CLASS_ALIAS(Constraint, func_constraint);

Constraint::Constraint() {
	type = PCT_BALL;
	physConstraint = 0;
	this->postEvent(0,"finishSpawningConstraint");
}
Constraint::~Constraint() {
	destroyConstraint();
}
void Constraint::destroyConstraint() {
	if(this->physConstraint) {
		g_physWorld->destroyPhysicsConstraint(physConstraint);
		physConstraint = 0;
	}
}
void Constraint::finishSpawningConstraint() {
	destroyConstraint();

	e0 = G_FindFirstEntityWithTargetName(e0TargetName);
	e1 = G_FindFirstEntityWithTargetName(e1TargetName);
	if(e0 == 0) {
		if(e1 == 0) {
			return;
		}
		e0 = e1;
		e1 = 0;
	}
	physObjectAPI_i *body0 = e0->getRigidBody();
	physObjectAPI_i *body1;
	if(e1) {
		body1 = e1->getRigidBody();
	} else {
		body1 = 0;
	}
	if(body0 == 0) {
		if(body1) {
			body0 = body1;
			body1 = 0;
		}
		g_core->RedWarning("Constraint::finishSpawningConstraint(): constraint entities has no physics objects\n");
		return;
	}
	// body0 must be not-NULL here
	// if body1 is null, static world is used instead of it
	if(this->type == PCT_BALL) {
		this->physConstraint = g_physWorld->createConstraintBall(this->getOrigin(),body0,body1);
	} else if(this->type == PCT_HINGE) {
		vec3_c axis(0,0,1);
		this->physConstraint = g_physWorld->createConstraintHinge(this->getOrigin(),axis,body0,body1);
	}
}
void Constraint::processEvent(class eventBaseAPI_i *ev) {
	if(!stricmp(ev->getEventName(),"finishSpawningConstraint")) {
		finishSpawningConstraint();
	}
}
void Constraint::setKeyValue(const char *key, const char *value) {
	if(!stricmp(key,"body0")) {
		e0TargetName = value;
	} else if(!stricmp(key,"body1")) {
		e1TargetName = value;
	} else if(!stricmp(key,"type")) {
		if(!stricmp(value,"hinge")) {
			type = PCT_HINGE;
		}	
	} else {
		BaseEntity::setKeyValue(key,value);
	}
}
