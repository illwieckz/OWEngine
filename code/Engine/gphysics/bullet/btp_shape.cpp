////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2013 V.
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
//  File name:   btp_shape.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "btp_shape.h"
#include "btp_headers.h"
#include <api/cmAPI.h>
#include <api/coreAPI.h>
#include <shared/cmSurface.h>
#include "btp_convert.h"
#include "btp_cMod2BulletShape.h"

bulletColShape_c::bulletColShape_c()
{
	bulletShape = 0;
	centerOfMassTransform.identity();
}
bulletColShape_c::~bulletColShape_c()
{
	if ( bulletShape )
	{
		delete bulletShape;
	}
}

bool bulletColShape_c::init( const class cMod_i* newCModel, bool newBIStatic )
{
	this->cModel = newCModel;
	
	vec3_c centerOfMass;
	if ( newBIStatic )
	{
		// static models dont need to have center of mass fixed
		bHasCenterOfMassTransform = false;
		centerOfMass.zero();
	}
	else
	{
		aabb bb;
		newCModel->getBounds( bb );
		centerOfMass = bb.getCenter();
		this->centerOfMassTransform.identity();
		if ( centerOfMass.isAlmostZero() )
		{
			bHasCenterOfMassTransform = false;
		}
		else
		{
			bHasCenterOfMassTransform = true;
			this->centerOfMassTransform.setOrigin( ( centerOfMass * QIO_TO_BULLET ).floatPtr() );
		}
	}
	bulletShape = BT_CModelToBulletCollisionShape( newCModel, newBIStatic, &centerOfMass );
	return false; // no error
}

