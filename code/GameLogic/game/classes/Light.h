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
//  File name:   Light.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Dynamic light entity
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "BaseEntity.h"

class Light : public BaseEntity {
	float radius;
	float spotLightRadius;
public:
	Light();

	DECLARE_CLASS( Light );

	void setRadius(float newRadius);
	void setSpotLightRadius(float newSpotLightRadius);

	virtual void setKeyValue(const char *key, const char *value); 
	virtual void getLocalBounds(aabb &out) const;
	virtual void processEvent(class eventBaseAPI_i *ev);
};

#endif // __LIGHT_H__
