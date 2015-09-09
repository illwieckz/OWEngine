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
//  File name:   g_weapons.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Helper functions for Weapon class
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "g_local.h"
#include "explosionInfo.h"
#include "classes/BaseEntity.h"
#include "classes/Projectile.h"
#include <shared/trace.h>
#include <shared/autoCvar.h>
#include <api/rApi.h>
#include <api/serverApi.h>
#include <api/physAPI.h>
#include <api/declManagerAPI.h>
#include <api/coreAPI.h>

static aCvar_c g_showBulletTraces( "g_showBulletTraces", "0" );
static aCvar_c g_printBulletAttackCalls( "g_printBulletAttackCalls", "0" );
static aCvar_c g_printRailgunAttackCalls( "g_printRailgunAttackCalls", "0" );

void G_BulletAttack( const vec3_c& muzzle, const vec3_c& dir, BaseEntity* baseSkip, const char* markMaterial )
{
	trace_c tr;
	tr.setupRay( muzzle, muzzle + dir * 10000.f );
	g_physWorld->traceRay( tr );
	if ( g_printBulletAttackCalls.getInt() )
	{
		G_Printf( "G_BulletAttack: hit %f %f %f\n", tr.getHitPos().x, tr.getHitPos().y, tr.getHitPos().z );
	}
	if ( rf && g_showBulletTraces.getInt() )
	{
		rf->addDebugLine( tr.getStartPos(), tr.getHitPos(), vec3_c( 1, 0, 0 ), 5.f );
	}
	if ( markMaterial == 0 )
	{
#if 0
		// default Q3 mark material
		markMaterial = "gfx/damage/bullet_mrk"
#else
		// default our mark material
		markMaterial = "xrealBulletMark";
#endif
	}
				   g_server->SendServerCommand( -1, va( "test_bulletAttack %s %f %f %f %f %f %f %i", markMaterial, muzzle.x, muzzle.y, muzzle.z,
						   dir.x, dir.y, dir.z, baseSkip->getEntNum() ) );
	if ( tr.hasHit() )
	{
		BaseEntity* h = tr.getHitEntity();
		if ( h )
		{
			h->onBulletHit( tr.getHitPos(), dir, 10 );
		}
	}
}
void G_RailGunAttack( const vec3_c& muzzle, const vec3_c& dir, BaseEntity* baseSkip, const railgunAttackMaterials_s* mats )
{
	trace_c tr;
	tr.setupRay( muzzle, muzzle + dir * 10000.f );
	g_physWorld->traceRay( tr );
	if ( g_printRailgunAttackCalls.getInt() )
	{
		G_Printf( "G_RailGunAttack: hit %f %f %f\n", tr.getHitPos().x, tr.getHitPos().y, tr.getHitPos().z );
	}
	
	railgunAttackMaterials_s defaultMats;
	if ( mats == 0 )
	{
		mats = &defaultMats;
	}
	
	//g_server->SendServerCommand(-1,va("doRailgunEffect railCore railDisc railExplosion gfx/damage/plasma_mrk %f %f %f %f %f %f %i",muzzle.x,muzzle.y,muzzle.z,
	g_server->SendServerCommand( -1, va( "doRailgunEffect %s %s %s %s %f %f %f %f %f %f %i",
										 mats->railCore.c_str(), mats->railDisc.c_str(), mats->railExplosion.c_str(), mats->markMaterial.c_str(), muzzle.x, muzzle.y, muzzle.z,
										 dir.x, dir.y, dir.z, baseSkip->getEntNum() ) );
	if ( tr.hasHit() )
	{
		BaseEntity* h = tr.getHitEntity();
		if ( h )
		{
			h->onBulletHit( tr.getHitPos(), dir, 100 );
			// apply extra impulse
			h->applyPointImpulse( dir * 500.f, tr.getHitPos() );
		}
	}
}
void G_FireProjectile( const char* projectileDefName, const vec3_c& muzzle, const vec3_c& dir, BaseEntity* baseSkip )
{
	BaseEntity* e = G_SpawnClass( projectileDefName );
	Projectile* p = dynamic_cast<Projectile*>( e );
	if ( p == 0 )
	{
		g_core->RedWarning( "G_FireProjectile: spawned entity was not a subclass of projectile\n" );
		delete e;
		return;
	}
	p->setOrigin( muzzle + dir * 32 );
	p->setAngles( dir.toAngles() );
	if ( p->hasStartVelocitySet() )
	{
		vec3_c left, down;
		dir.makeNormalVectors( left, down );
		vec3_c startVel = p->getStartVelocity();
		p->setLinearVelocity( dir * startVel.x - left * startVel.y - down * startVel.z );
	}
	else
	{
		p->setLinearVelocity( dir * 1000.f );
	}
}
float G_randomFloat( float min, float max )
{
	float random = ( ( float ) rand() ) / ( float ) RAND_MAX;
	float range = max - min;
	return ( random * range ) + min;
}
void G_MultiBulletAttack( const vec3_c& muzzle, const vec3_c& dir, BaseEntity* baseSkip, u32 numBullets, float maxSpread, float spreadDist, const char* markMaterial )
{
	vec3_c u, r;
	u = dir.getPerpendicular();
	r.crossProduct( u, dir );
	u.normalize();
	r.normalize();
	for ( u32 i = 0; i < numBullets; i++ )
	{
		vec3_c end = muzzle + dir * spreadDist + u * G_randomFloat( -maxSpread, maxSpread ) + r * G_randomFloat( -maxSpread, maxSpread );
		vec3_c newDir = ( end - muzzle );
		newDir.normalize();
		G_BulletAttack( muzzle, newDir, baseSkip, markMaterial );
	}
}

void G_Explosion( const vec3_c& pos, const struct explosionInfo_s& explosionInfo, const char* extraDamageDefName )
{
	if ( explosionInfo.radius <= 0.f )
	{
		g_core->RedWarning( "G_Explosion: radius is %f\n", explosionInfo.radius );
		return;
	}
	aabb bb;
	bb.fromPointAndRadius( pos, explosionInfo.radius );
	arraySTD_c<BaseEntity*> ents;
	G_BoxEntities( bb, ents );
	int damage = 200;
	for ( u32 i = 0; i < ents.size(); i++ )
	{
		BaseEntity* be = ents[i];
		vec3_c dir = be->getOrigin() - pos;
		float dist = dir.len();
		if ( dist > explosionInfo.radius )
			continue;
		float frac = 1.f - dist / explosionInfo.radius;
		int damageScaled = int( float( damage ) * frac );
		be->damage( damageScaled );
		if ( be->hasPhysicsObject() == false )
			continue;
		dir.normalize();
		dir *= ( explosionInfo.force * frac );
		be->applyCentralForce( dir );
	}
	// add clientside effect
	if ( explosionInfo.materialName.length() )
	{
		g_server->SendServerCommand( -1, va( "doExplosionEffect %f %f %f %f %s", pos.x, pos.y, pos.z,
											 explosionInfo.spriteRadius, explosionInfo.materialName.c_str() ) );
	}
}


