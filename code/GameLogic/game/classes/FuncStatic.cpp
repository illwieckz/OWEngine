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
//  File name:   FuncStatic.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "FuncStatic.h"
#include <shared/entityType.h>

DEFINE_CLASS(FuncStatic, "ModelEntity");
DEFINE_CLASS_ALIAS(FuncStatic, func_static);
//DEFINE_CLASS_ALIAS(FuncStatic, func_group);
DEFINE_CLASS_ALIAS(FuncStatic, func_mover);

// func_static entities are inmovable
FuncStatic::FuncStatic() {
	//bPhysicsBodyKinematic = true;
	mass = 0.f;
	this->setEntityType(ET_FUNC_STATIC);
}
void FuncStatic::setKeyValue(const char *key, const char *value) {
	if(!stricmp(key,"model")) {
		this->setRenderModel(value);
		this->setColModel(value);
	} else {
		ModelEntity::setKeyValue(key,value);		
	}
}
