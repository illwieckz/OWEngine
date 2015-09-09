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
//  File name:   g_saveMapFile.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Saves current scene to .map file which we can
//               later load (inboth Radiant and OWEngine)
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "g_local.h"
#include "classes/World.h"
#include "classes/BaseEntity.h"
#include <api/writeStreamAPI.h>
#include <api/coreAPI.h>
#include <shared/fileStreamHelper.h>
#include <shared/mapFileWriter.h>

// NOTE: .map file primitives writing is not supported (yet..)

bool G_SaveCurrentSceneToMapFile(const char *outFName) {
	mapFileWriter_c file;
	if(file.beginWritingMapFile(outFName)) {
		g_core->RedWarning("G_SaveCurrentSceneToMapFile: cannot open %s for writing\n",outFName);
		return true; // error
	}
	// === write worldspawn =======
	file.beginEntity("worldspawn");
	g_world.iterateKeyValues(&file);
	file.endEntity();
	// ============================
	// write worldspawn entity first
	for(u32 i = MAX_CLIENTS; i < level.num_entities; i++) {
		edict_s *ed = &g_entities[i];
		if(ed->s == 0)
			continue;
		if(ed->ent == 0)
			continue;
		BaseEntity *bent = ed->ent;
		const char *className = bent->getClassName();
		// write an entity
		file.beginEntity(className);
		bent->iterateKeyValues(&file);
		file.endEntity();
	}

	return false; // no error
}