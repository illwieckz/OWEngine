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
//  File name:   cg_emitter.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "cg_emitter.h"
#include <api/rAPI.h>
#include <api/coreAPI.h>
#include <api/staticModelCreatorAPI.h>

emitterDefault_c::emitterDefault_c( int timeNow )
{
	lastSpawnTime = timeNow;
	
	particleLife = 500;
}
emitterDefault_c::~emitterDefault_c()
{
	rf->removeCustomRenderObject( this );
}

void emitterDefault_c::instanceModel( class staticModelCreatorAPI_i* out, const class axis_c& viewerAxis )
{
	g_core->Print( "emitterDefault_c::instanceModel: at %f %f %f\n", origin.x, origin.y, origin.z );
	
	out->clear();
	for ( int i = particles.size() - 1; i >= 0; i-- )
	{
		const emitterParticle_s& p = particles[i];
		int elapsed = lastUpdateTime - p.spawnTime;
		float lifeFracInv = float( elapsed ) / float( particleLife );
		float lifeFrac = 1.f - lifeFracInv;
		float radius = spriteRadius + 30.f * lifeFracInv;
		byte alpha = 50 * lifeFrac;
		out->addSprite( p.origin, radius, this->mat, viewerAxis, alpha );
	}
}
void emitterDefault_c::spawnSingleParticle( int curTime )
{
	emitterParticle_s np;
	np.spawnTime = curTime;
	np.origin = origin;
	particles.push_back( np );
}
void emitterDefault_c::updateEmitter( int newTime )
{
	lastUpdateTime = newTime;
	// spawn new particles
	while ( newTime - lastSpawnTime > spawnInterval )
	{
		spawnSingleParticle( lastSpawnTime + 1 );
		lastSpawnTime += spawnInterval;
	}
	// remove old particles
	for ( int i = 0; i < particles.size(); i++ )
	{
		if ( newTime - particles[i].spawnTime > particleLife )
		{
			particles.erase( i );
			i--;
		}
	}
}
void emitterDefault_c::setOrigin( const vec3_c& newOrigin )
{
	origin = newOrigin;
}
void emitterDefault_c::setRadius( float newSpriteRadius )
{
	spriteRadius = newSpriteRadius;
}
void emitterDefault_c::setInterval( int newSpawnInterval )
{
	spawnInterval = newSpawnInterval;
}
void emitterDefault_c::setMaterial( class mtrAPI_i* newMat )
{
	mat = newMat;
}



