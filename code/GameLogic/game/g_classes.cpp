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
//  File name:   g_classes.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Game classes system
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "g_local.h"
#include "g_classes.h"
#include "classes/BaseEntity.h"

// class definitions
static gClassDef_c *g_classList = 0;

gClassDef_c::gClassDef_c(const char *newClassName, const char *newParentClass, allocNewInstanceFunc_t allocNewInstanceFunc) {
	strcpy(className,newClassName);
	strcpy(parentClass,newParentClass);

	this->newInstance = allocNewInstanceFunc;
	
	// add to the list
	this->nextDef = g_classList;
	g_classList = this;
}

// class aliases
static gClassAlias_c *g_aliasList = 0;
gClassAlias_c::gClassAlias_c(const char *newClassName, const char *newClassAlias) {
	strcpy(className,newClassName);
	strcpy(classAlias,newClassAlias);

	// add to the list
	this->next = g_aliasList;
	g_aliasList = this;
}

const char *G_TranslateClassAlias(const char *classNameOrAlias) {
	gClassAlias_c *ca = g_aliasList;
	while(ca) {
		if(!stricmp(ca->getClassAlias(),classNameOrAlias))
			return ca->getClassName();
		ca = ca->getNext();
	}
	return classNameOrAlias;
}

gClassDef_c *G_FindClassDefBasic(const char *origClassName) {
	// translate class aliases to our internal class names
	const char *className = G_TranslateClassAlias(origClassName);
	gClassDef_c *cd = g_classList;
	while(cd) {
		if(!stricmp(cd->getClassName(),className))
			return cd;
		cd = cd->getNext();
	}
	return 0;
}

void *G_SpawnClassDef(const char *className) {
	gClassDef_c *cd = G_FindClassDefBasic(className);
	if(cd == 0)
		return 0;
	// hack
	if(cd->hasClassName("Player") || cd->hasClassName("FakePlayer")) {	
		edict_s *newEdict = G_Spawn();
		BE_SetForcedEdict(newEdict);
	}
	return cd->allocNewInstance();
}