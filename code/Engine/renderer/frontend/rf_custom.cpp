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
//  File name:   rf_custom.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include <shared/array.h>
#include <api/mtrAPI.h>
#include <api/customRenderObjectAPI.h>
#include "rf_drawCall.h"
#include "rf_surface.h"
#include "rf_local.h"

class customRenderObjectBackend_c
{
		customRenderObjectAPI_i* api;
		r_model_c instance;
	public:
		customRenderObjectBackend_c( class customRenderObjectAPI_i* nAPI )
		{
			api = nAPI;
		}
		void addDrawCall()
		{
			api->instanceModel( &instance, rf_camera.getAxis() );
			instance.addDrawCalls( 0, true );
		}
		inline customRenderObjectAPI_i* getAPI() const
		{
			return api;
		}
};

static arraySTD_c<customRenderObjectBackend_c*> rf_customs;

void RF_AddCustomDrawCalls()
{
	for ( u32 i = 0; i < rf_customs.size(); i++ )
	{
		customRenderObjectBackend_c* sf = rf_customs[i];
		sf->addDrawCall();
	}
}

void RF_AddCustomRenderObject( class customRenderObjectAPI_i* api )
{
	rf_customs.push_back( new customRenderObjectBackend_c( api ) );
}
void RF_RemoveCustomRenderObject( class customRenderObjectAPI_i* api )
{
	for ( u32 i = 0; i < rf_customs.size(); i++ )
	{
		customRenderObjectBackend_c* sf = rf_customs[i];
		if ( sf->getAPI() == api )
		{
			rf_customs.erase( i );
			delete sf;
			return;
		}
	}
}




