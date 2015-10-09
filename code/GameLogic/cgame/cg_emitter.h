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
//  File name:   cg_emitter.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Default emitter object
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __CG_EMITTER_H__
#define __CG_EMITTER_H__

#include "cg_emitter_base.h"
#include <shared/array.h>
#include <math/vec3.h>

struct emitterParticle_s
{
	vec3_c origin;
	int spawnTime;
};

// default emitter, requires only material to function
class emitterDefault_c : public emitterBase_c
{
		arraySTD_c<emitterParticle_s> particles;
		
		class mtrAPI_i* mat;
		float spriteRadius;
		int spawnInterval;
		int particleLife;
		
		vec3_c origin;
		int lastSpawnTime;
		int lastUpdateTime;
		
		void spawnSingleParticle( int curTime );
		
		// customRenderObjectAPI_i impl
		virtual void instanceModel( class staticModelCreatorAPI_i* out, const class axis_c& viewerAxis );
	public:
		emitterDefault_c( int timeNow );
		~emitterDefault_c();
		
		virtual void updateEmitter( int newTime );
		
		virtual void setOrigin( const vec3_c& newOrigin );
		virtual void setRadius( float newSpriteRadius );
		virtual void setInterval( int newSpawnInterval );
		virtual void setMaterial( class mtrAPI_i* newMat );
};

#endif // __CG_EMITTER_H__
