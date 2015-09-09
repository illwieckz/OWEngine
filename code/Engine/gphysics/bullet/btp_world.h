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
//  File name:   btp_world.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __BTP_WORLD_H__
#define __BTP_WORLD_H__

#include <shared/hashTableTemplate.h>
#include <shared/str.h>
#include <api/physAPI.h>
#include "btp_staticMapLoader.h"

class bulletPhysicsWorld_c : public physWorldAPI_i
{
		str debugName;
		vec3_c gravity;
		// static world collision
		btpStaticMapLoader_c staticWorld;
		// Bullet shapes cache (single shape can be used by several rigid bodies)
		hashTableTemplateExt_c<class bulletColShape_c> shapes;
		// single rigid bodies (NOTE: they can be connected by constraints)
		arraySTD_c<class bulletRigidBody_c*> bodies;
		// character controllers
		arraySTD_c<class btpCharacterController_c*> characters;
		// constraints
		arraySTD_c<class btpConstraintBase_c*> constraints;
		// vehicles
		arraySTD_c<class btVehicle_c*> vehicles;
		// BulletPhysics variables
		class btBroadphaseInterface* broadphase;
		class btDefaultCollisionConfiguration* collisionConfiguration;
		class btCollisionDispatcher* dispatcher;
		class btSequentialImpulseConstraintSolver* solver;
		class btDiscreteDynamicsWorld* dynamicsWorld;
		
		bulletColShape_c* registerShape( const class cMod_i* cmodel, bool isStatic );
		
	public:
		bulletPhysicsWorld_c( const char* newDebugName );
		~bulletPhysicsWorld_c();
		
		virtual void init( const vec3_c& newGravity );
		virtual bool loadMap( const char* mapName );
		virtual void runFrame( float frameTime );
		virtual void shutdown();
		
		// mass 0 means that object is static (non moveable)
		virtual physObjectAPI_i* createPhysicsObject( const struct physObjectDef_s& def );
		virtual void destroyPhysicsObject( class physObjectAPI_i* p );
		
		virtual class physConstraintAPI_i* createConstraintBall( const vec3_c& pos, physObjectAPI_i* b0, physObjectAPI_i* b1 );
		virtual class physConstraintAPI_i* createConstraintHinge( const vec3_c& pos, const vec3_c& axis, physObjectAPI_i* b0, physObjectAPI_i* b1 );
		virtual void destroyPhysicsConstraint( physConstraintAPI_i* p );
		
		virtual class physCharacterControllerAPI_i* createCharacter( const class vec3_c& pos, float characterHeight,  float characterWidth );
		virtual void freeCharacter( class physCharacterControllerAPI_i* p );
		
		virtual class physVehicleAPI_i* createVehicle( const vec3_c& pos, const vec3_c& angles, class cMod_i* cm );
		virtual void removeVehicle( class physVehicleAPI_i* v );
		
		virtual void setGravity( const vec3_c& newGravity );
		virtual const vec3_c& getGravity() const;
		virtual bool traceRay( class trace_c& tr );
		
		void doDebugDrawing( class rDebugDrawer_i* dd );
		
		class btDynamicsWorld* getBTDynamicsWorld()
		{
				return dynamicsWorld;
		}
};

#endif // __BTP_WORLD_H__
