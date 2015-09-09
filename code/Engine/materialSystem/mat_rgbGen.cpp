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
//  File name:   mat_rgbGen.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "mat_rgbGen.h"
#include <shared/parser.h>
#include <shared/ast.h>

// NOTES:
// it seems that "vertexColor" can be used along with "rgb <expression>" in Doom3.
// See D3's "textures/decals/bfg_wallmark" from "senetemp.mtr"
// example:
/*
textures/test/somaMaterial
{

	{
		blend		blend
		map			textures/test/somaMaterial
		rgb			0.5 + randomTable[time*0.5]
		vertexColor
	}
}
*/
rgbGen_c::rgbGen_c()
{
    type = RGBGEN_NONE;
    asts[0] = 0;
    asts[1] = 0;
    asts[2] = 0;
}
rgbGen_c::~rgbGen_c()
{
    this->clear();
}
void rgbGen_c::clear()
{
    if( type == RGBGEN_AST )
    {
        if( asts[0] && asts[0] == asts[1] && asts[1] == asts[2] )
        {
            // single AST for RGB (free it once)
            asts[0]->destroyAST();
        }
        else
        {
            // separate ATSs
            for( u32 i = 0; i < 3; i++ )
            {
                if( asts[i] )
                {
                    asts[i]->destroyAST();
                }
            }
        }
    }
}
bool rgbGen_c::parse( class parser_c& p )
{
    if( p.atWord( "wave" ) )
    {
        type = RGBGEN_WAVE;
        wave.parse( p );
    }
    else if( p.atWord( "const" ) )
    {
        type = RGBGEN_CONST;
        p.getFloatMat_braced( constValue, 3 );
    }
    else if( p.atWord( "vertex" ) )
    {
        type = RGBGEN_VERTEX;
    }
    else if( p.atWord( "identity" ) )
    {
        type = RGBGEN_IDENTITY;
    }
    else if( p.atWord( "static" ) )
    {
        // seem to be used in MoHAA static models.
        // It might be related to MoHAA precomputed static models lighting (vertex lighting)
        type = RGBGEN_STATIC;
    }
    else if( p.atWord( "lightingSpherical" ) )
    {
        // MoHAA-specific spherical lighting?
        type = RGBGEN_LIGHTINGSPHERICAL;
    }
    else if( p.atWord( "identityLighting" ) )
    {
        type = RGBGEN_IDENTITYLIGHTING;
    }
    else if( p.atWord( "exactVertex" ) )
    {
        type = RGBGEN_EXACTVERTEX;
    }
    else if( p.atWord( "constLighting" ) )
    {
        type = RGBGEN_CONSTLIGHTING; // added for Call Of Duty??
    }
    else if( p.atWord( "lightingdiffuse" ) )
    {
        type = RGBGEN_LIGHTINGDIFFUSE; // added for RTCW?
    }
    else if( p.atWord( "entity" ) )
    {
        // this is used by Quake3 railgun model to set shader color from cgame code
    }
    else
    {
        str tok = p.getToken();
        g_core->RedWarning( "rgbGen_c::parse: unknown rgbGen %s\n", tok.c_str() );
        return true;
    }
    return false;
}
void rgbGen_c::setRGBGenAST( class astAPI_i* newAST )
{
    this->clear();
    type = RGBGEN_AST;
    asts[0] = asts[1] = asts[2] = newAST;
}
void rgbGen_c::setRedAST( class astAPI_i* newAST )
{
    type = RGBGEN_AST;
    // don't overwrite pointer to existing AST, free it.
    if( asts[0] )
        delete asts[0];
    asts[0] = newAST;
}
void rgbGen_c::setGreenAST( class astAPI_i* newAST )
{
    type = RGBGEN_AST;
    // don't overwrite pointer to existing AST, free it.
    if( asts[1] )
        delete asts[1];
    asts[1] = newAST;
}
void rgbGen_c::setBlueAST( class astAPI_i* newAST )
{
    type = RGBGEN_AST;
    // don't overwrite pointer to existing AST, free it.
    if( asts[2] )
        delete asts[2];
    asts[2] = newAST;
}
void rgbGen_c::evaluateRGBGen( const class astInputAPI_i* in, float* out3Floats ) const
{
    if( type == RGBGEN_AST )
    {
        if( asts[0] == asts[1] && asts[1] == asts[2] )
        {
            // single AST for rgb
            float result = asts[0]->execute( in );
            out3Floats[0] = result;
            out3Floats[1] = result;
            out3Floats[2] = result;
        }
        else
        {
            // separate ASTs for each color
            for( u32 i = 0; i < 3; i++ )
            {
                if( asts[i] == 0 )
                {
                    out3Floats[i] = 1.f;
                }
                else
                {
                    out3Floats[i] = asts[i]->execute( in );
                }
            }
        }
    }
}
void rgbGen_c::setVertex()
{
    this->clear();
    type = RGBGEN_VERTEX;
}
