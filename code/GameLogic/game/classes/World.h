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
//  File name:   World.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __WORLD_H__
#define __WORLD_H__

#include <shared/str.h>

class World
{
		str skyMaterial;
		float waterLevel;
		bool hasWaterLevel;
		float farPlane;
		
		void runGlobalWaterPhysics();
	public:
		World();
		
		// called once on game startup
		//void initWorldSpawn();
		
		// called every frame
		void runWorldFrame();
		
		// .map file - > Entity key values communication
		virtual void setKeyValue( const char* key, const char* value );
		// Entity -> .map file key values communiaction
		virtual void iterateKeyValues( class keyValuesListener_i* listener ) const;
};

extern World g_world;

#endif // __WORLD_H__

