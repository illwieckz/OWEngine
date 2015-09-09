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
//  File name:   cg_testEmitter.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: client side emitter testing
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "cg_local.h"
#include "cg_emitter.h"
#include "cg_emitter_d3.h"
#include <shared/autoCvar.h>
#include <api/rAPI.h>
#include <api/declManagerAPI.h>
#include <renderer/rModelAPI.h>
#include <math/matrix.h>
#include <math/axis.h>

static aCvar_c cg_testEmitter( "cg_testEmitter", "" );
static aCvar_c cg_testEmitter_attachToCamera( "cg_testEmitter_attachToCamera", "0" );
static aCvar_c cg_testEmitter_cameraDistance( "cg_testEmitter_cameraDistance", "64" );

static class emitterBase_c* cg_testEmitterInstance = 0;

static void CG_FreeTestEmitter()
{
    if( cg_testEmitterInstance )
    {
        rf->removeCustomRenderObject( cg_testEmitterInstance );
        delete cg_testEmitterInstance;
        cg_testEmitterInstance = 0;
    }
}
void CG_RunTestEmitter()
{
    if( cg_testEmitter.getStr()[0] == 0 || cg_testEmitter.getStr()[0] == '0' )
    {
        CG_FreeTestEmitter();
        return;
    }
    class particleDeclAPI_i* pDecl = g_declMgr->registerParticleDecl( cg_testEmitter.getStr() );
    if( pDecl == 0 )
    {
        CG_FreeTestEmitter();
        return;
    }
    if( cg_testEmitterInstance == 0 )
    {
        cg_testEmitterInstance = new emitterD3_c;
        rf->addCustomRenderObject( cg_testEmitterInstance );
        cg_testEmitterInstance->setOrigin( cg.refdefViewOrigin + cg.refdefViewAxis.getForward()*cg_testEmitter_cameraDistance.getFloat() );
    }
    else
    {
        if( cg_testEmitter_attachToCamera.getInt() )
        {
            cg_testEmitterInstance->setOrigin( cg.refdefViewOrigin + cg.refdefViewAxis.getForward()*cg_testEmitter_cameraDistance.getFloat() );
        }
    }
    cg_testEmitterInstance->setParticleDecl( pDecl );
    
}