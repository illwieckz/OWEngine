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
//  File name:   VehicleCar.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __VEHICLECAR_H__
#define __VEHICLECAR_H__

#include "ModelEntity.h"

class VehicleCar : public ModelEntity {
public:
	DECLARE_CLASS( VehicleCar );

	class physVehicleAPI_i *physVehicle;
	class Player *driver;

	VehicleCar();
	~VehicleCar();

	void spawnPhysicsVehicle();
	void destroyPhysicsVehicle();

	virtual void setOrigin(const class vec3_c &newPos);
	// calls "spawnPhysicsVehicle"
	virtual void postSpawn();

	void steerUCmd(const struct userCmd_s *ucmd);

	void detachPlayer(class Player *pl) {
		if(pl == driver) {
			driver = 0;
		}
	}
	virtual bool doUse(class Player *activator);
	virtual void runPhysicsObject();
};

#endif // __VEHICLECAR_H__
