/*
============================================================================
Copyright (C) 2013 V.

This file is part of Qio source code.

Qio source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

Qio source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA,
or simply visit <http://www.gnu.org/licenses/>.
============================================================================
*/
// cg_emitter_d3.h - Doom3 particles (prt) emitter
#ifndef __CG_EMITTER_D3_H__
#define __CG_EMITTER_D3_H__

#include "cg_emitter_base.h"
#include <math/vec3.h>

// Doom3 particle emitter (requires Doom3 .prt particles decl to function)
class emitterD3_c : public emitterBase_c
{
    vec3_c origin;
    class particleDeclAPI_i* decl;
    
    // customRenderObjectAPI_i impl
    virtual void instanceModel( class staticModelCreatorAPI_i* out, const class axis_c& viewerAxis );
public:
    emitterD3_c();
    virtual ~emitterD3_c();
    
    virtual void setOrigin( const vec3_c& newOrigin );
    virtual void setParticleDecl( class particleDeclAPI_i* newDecl );
};

#endif // __CG_EMITTER_D3_H__

