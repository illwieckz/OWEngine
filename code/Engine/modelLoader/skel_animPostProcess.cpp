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
//  File name:   skel_animPostProcess.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: skanpp files execution
//               .skanpp files are used to alter animations after
//               loading them from various files formats.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include <shared/str.h>
#include <shared/parser.h>
#include <math/aabb.h>
#include <api/coreAPI.h>
#include <api/skelAnimPostProcessFuncs.h>

bool SK_ApplyAnimPostProcess( const char* modName, class skelAnimPostProcessFuncs_i* inout )
{
	str skanppName = modName;
	skanppName.setExtension( "skanpp" );
	parser_c p;
	if ( p.openFile( skanppName ) )
	{
		return true; // optional mdlpp file is not present
	}
	while ( p.atEOF() == false )
	{
		if ( p.atWord( "scaleanimspeed" ) )
		{
			float scale = p.getFloat();
			inout->scaleAnimationSpeed( scale );
		}
		else if ( p.atWord( "clearmd5bonecomponentflags" ) )
		{
			str boneName = p.getToken();
			inout->clearMD5BoneComponentFlags( boneName );
		}
		else if ( p.atWord( "looplastframe" ) )
		{
			inout->setLoopLastFrame( true );
		}
		else
		{
			int line = p.getCurrentLineNumber();
			str token = p.getToken();
			g_core->RedWarning( "SK_ApplyAnimPostProcess: unknown anim postprocess command %s at line %i of file %s\n",
								token.c_str(), line, skanppName.c_str() );
		}
	}
	return false;
}
