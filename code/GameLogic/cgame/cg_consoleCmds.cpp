////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2014 V.
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
//  File name:   cg_consoleCmds.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: cgame console commands
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include <shared/autoCmd.h>
#include <api/coreAPI.h>
#include <api/rAPI.h>
#include <shared/rendererSurfaceRef.h>

void CG_SetCrosshairSurfaceMaterial_f()
{
	if ( g_core->Argc() < 2 )
	{
		g_core->Print( "Usage: cg_setCrosshairSurfaceMaterial <material_name>\n" );
		return;
	}
	const char* matName = g_core->Argv( 1 );
	rendererSurfaceRef_s ref;
	rf->getLookatSurfaceInfo( ref );
	// send remap command to server so it can broadcast it to all clients
	g_core->Cbuf_ExecuteText( EXEC_APPEND, va( "net_setWorldSurfaceMaterial %i %i %s", ref.areaNum, ref.surfaceNum, matName ) );
	//rf->setWorldSurfaceMaterial(matName,ref.surfaceNum,ref.areaNum);
}

static aCmd_c cg_setCrosshairSurfaceMaterial_f( "cg_setCrosshairSurfaceMaterial", CG_SetCrosshairSurfaceMaterial_f );

