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
//  File name:   Trigger.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Base class for all triggers
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "Trigger.h"
#include "../g_local.h"
#include <api/cmAPI.h>
#include <api/coreAPI.h>
#include <shared/entityType.h>

static arraySTD_c<Trigger*> g_triggers;

DEFINE_CLASS( Trigger, "BaseEntity" );
DEFINE_CLASS_ALIAS( Trigger, idTrigger_Multi );

Trigger::Trigger()
{
	this->setEntityType( ET_TRIGGER );
	g_triggers.push_back( this );
	this->triggerModel = 0;
}
Trigger::~Trigger()
{
	g_triggers.remove( this );
}
void Trigger::setTriggerModel( const char* modName )
{
	if ( cm == 0 )
	{
		g_core->RedWarning( "Trigger::setTriggerModel: CM module not present\n" );
		return;
	}
	this->triggerModel = cm->registerModel( modName );
	this->recalcABSBounds();
}
void Trigger::onTriggerContact( class ModelEntity* ent )
{

}
void Trigger::setKeyValue( const char* key, const char* value )
{
	if ( !stricmp( key, "model" ) )
	{
		this->setTriggerModel( value );
	}
	else
	{
		BaseEntity::setKeyValue( key, value );
	}
}
void Trigger::getLocalBounds( aabb& out ) const
{
	if ( triggerModel )
	{
		triggerModel->getBounds( out );
	}
	else
	{
		out.fromHalfSize( 4.f );
	}
}

u32 G_BoxTriggers( const aabb& bb, arraySTD_c<Trigger*>& out )
{
	out.clear();
	// TODO: speed this up with kd tree
	for ( u32 i = 0; i < g_triggers.size(); i++ )
	{
		Trigger* t = g_triggers[i];
		const aabb& tBB = t->getAbsBounds();
		if ( tBB.intersect( bb ) )
		{
			out.push_back( t );
		}
	}
	return out.size();
}

