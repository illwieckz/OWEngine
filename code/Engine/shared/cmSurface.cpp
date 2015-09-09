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
//  File name:   cmSurface.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: simple trimesh surface for collision detection
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "cmSurface.h"

#include <api/coreAPI.h>
#include <shared/parser.h>

// load vertices/triangles data directly from Doom3 .proc file
bool cmSurface_c::loadDoom3ProcFileWorldModel( const char* fname )
{
	parser_c p;
	if ( p.openFile( fname ) )
	{
		g_core->RedWarning( "cmSurface_c::loadDoom3ProcFileWorldModel: cannot open %s\n", fname );
		return true; // error
	}
	
	// check for Doom3 ident first
	if ( p.atWord( "mapProcFile003" ) == false )
	{
		if ( p.atWord( "PROC" ) )
		{
			// Quake4 ident
			str version = p.getToken();
		}
		else
		{
			g_core->RedWarning( "cmSurface_c::loadDoom3ProcFileWorldModel: %s has bad ident %s, should be %s or %s\n", fname, p.getToken(), "mapProcFile003", "PROC" );
			return true; // error
		}
	}
	
	while ( p.atEOF() == false )
	{
		if ( p.atWord( "model" ) )
		{
			if ( p.atWord( "{" ) == false )
			{
				g_core->RedWarning( "cmSurface_c::loadDoom3ProcFileWorldModel:  expected '{' to follow \"model\" in file %s at line %i, found %s\n",
									p.getDebugFileName(), p.getCurrentLineNumber(), p.getToken() );
				return true; // error
			}
			str modName = p.getToken();
			if ( Q_stricmpn( modName, "_area", 5 ) )
			{
				// skip curly braced block
				p.skipCurlyBracedBlock( false );
			}
			else
			{
				u32 numSurfs = p.getInteger();
				if ( numSurfs )
				{
					for ( u32 i = 0; i < numSurfs; i++ )
					{
						int sky = -1;
						if ( p.atWord( "{" ) == false )
						{
							// check for extra 'sky' parameter used in Q4 proc files
							str token = p.getToken();
							if ( token.isNumerical() && p.atWord( "{" ) )
							{
								sky = atoi( token );
							}
							else
							{
								g_core->RedWarning( "cmSurface_c::loadDoom3ProcFileWorldModel: expected '{' to follow \"model\"'s surface in file %s at line %i, found %s\n",
													p.getDebugFileName(), p.getCurrentLineNumber(), p.getToken() );
								return true; // error
							}
						}
						str matName = p.getToken();
						u32 numSurfVerts = p.getInteger();
						u32 numSurfIndices = p.getInteger();
						u32 numPrevVerts = verts.size();
						u32 numPrevIndices = indices.size();
						// read verts
						verts.resize( numPrevVerts + numSurfVerts );
						vec3_c* sfVerts = verts.getArray() + numPrevVerts;
						for ( u32 i = 0; i < numSurfVerts; i++ )
						{
							if ( p.atWord( "(" ) == false )
							{
								g_core->RedWarning( "cmSurface_c::loadDoom3ProcFileWorldModel: expected '(' to follow vertex %i in file %s at line %i, found %s\n",
													i, p.getDebugFileName(), p.getCurrentLineNumber(), p.getToken() );
								return true; // error
							}
							p.getFloatMat( sfVerts[i], 3 );
							vec2_c tc;
							vec3_c normal;
							p.getFloatMat( tc, 2 );
							p.getFloatMat( normal, 3 );
							if ( p.atWord( ")" ) == false )
							{
								g_core->RedWarning( "cmSurface_c::loadDoom3ProcFileWorldModel: expected '(' after vertex %i in file %s at line %i, found %s\n",
													i, p.getDebugFileName(), p.getCurrentLineNumber(), p.getToken() );
								return true; // error
							}
						}
						// read triangles
						indices.resize( numPrevIndices + numSurfIndices );
						u32* sfIndices = indices.getArray() + numPrevIndices;
						for ( u32 i = 0; i < numSurfIndices; i++ )
						{
							sfIndices[i] = numPrevVerts + p.getInteger();
						}
						if ( p.atWord( "}" ) == false )
						{
							g_core->RedWarning( "cmSurface_c::loadDoom3ProcFileWorldModel: expected closing '}' for \"model\"'s surface block in file %s at line %i, found %s\n",
												p.getDebugFileName(), p.getCurrentLineNumber(), p.getToken() );
							return true; // error
						}
					}
				}
				if ( p.atWord( "}" ) == false )
				{
					g_core->RedWarning( "cmSurface_c::loadDoom3ProcFileWorldModel: expected closing '}' for \"model\" block in file %s at line %i, found %s\n",
										p.getDebugFileName(), p.getCurrentLineNumber(), p.getToken() );
					return true; // error
				}
			}
		}
		else if ( p.atWord( "interAreaPortals" ) )
		{
			p.skipCurlyBracedBlock();
		}
		else if ( p.atWord( "nodes" ) )
		{
			p.skipCurlyBracedBlock();
		}
		else if ( p.atWord( "shadowModel" ) )
		{
			p.skipCurlyBracedBlock();
		}
		else
		{
			g_core->RedWarning( "cmSurface_c::loadDoom3ProcFileWorldModel: skipping unknown token %s in file %s at line %i\n", p.getToken(), fname, p.getCurrentLineNumber() );
		}
	}
	return false; // OK
}
