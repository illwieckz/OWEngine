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
//  File name:   mapFileConverter.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Simple .MAP to trimesh converter
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include <shared/parser.h>
#include <shared/array.h>
#include <shared/cmWinding.h>
#include <math/vec3.h>
#include <math/vec2.h>
#include <math/plane.h>
#include <api/coreAPI.h>
#include <api/staticModelCreatorAPI.h>
#include <api/materialSystemAPI.h>
#include <api/mtrAPI.h>

// ==========================================
vec3_t	baseAxis[18] =
{
    {0, 0, 1}, {1, 0, 0}, {0, -1, 0},			// floor
    {0, 0, -1}, {1, 0, 0}, {0, -1, 0},		// ceiling
    {1, 0, 0}, {0, 1, 0}, {0, 0, -1},			// west wall
    { -1, 0, 0}, {0, 1, 0}, {0, 0, -1},		// east wall
    {0, 1, 0}, {1, 0, 0}, {0, 0, -1},			// south wall
    {0, -1, 0}, {1, 0, 0}, {0, 0, -1}			// north wall
};
void MOD_TextureAxisFromNormal( const vec3_c& normal, vec3_c& xv, vec3_c& yv )
{
    float best = 0;
    int bestaxis = 0;
    
    for( u32 i = 0; i < 6; i++ )
    {
        float dot = normal.dotProduct( baseAxis[i * 3] );
        if( dot > best )
        {
            best = dot;
            bestaxis = i;
        }
    }
    
    xv = baseAxis[bestaxis * 3 + 1];
    yv = baseAxis[bestaxis * 3 + 2];
}


struct r_brushSide_s
{
    plane_c plane;
    str matName;
    float oldVecs[2][4];
    vec3_c texMat[2];
    
    void setQ3TexVecs( const vec2_c& shift, const float rotate, const vec2_c& scale )
    {
        int		sv, tv;
        vec_t	ang, sinv, cosv;
        vec_t	ns, nt;
        
        vec3_c	vecs[2];
        MOD_TextureAxisFromNormal( plane.norm, vecs[0], vecs[1] );
        
        // fix scale values
        vec2_c _scale = scale;
        if( !_scale[0] )
            _scale[0] = 1.f;
        if( !_scale[1] )
            _scale[1] = 1.f;
            
        // rotate axis
        if( rotate == 0 )
        {
            sinv = 0;
            cosv = 1;
        }
        else if( rotate == 90 )
        {
            sinv = 1;
            cosv = 0;
        }
        else if( rotate == 180 )
        {
            sinv = 0;
            cosv = -1;
        }
        else if( rotate == 270 )
        {
            sinv = -1;
            cosv = 0;
        }
        else
        {
            ang = rotate / 180 * M_PI;
            sinv = sin( ang );
            cosv = cos( ang );
        }
        
        if( vecs[0][0] )
            sv = 0;
        else if( vecs[0][1] )
            sv = 1;
        else
            sv = 2;
            
        if( vecs[1][0] )
            tv = 0;
        else if( vecs[1][1] )
            tv = 1;
        else
            tv = 2;
            
        for( u32 i = 0; i < 2; i++ )
        {
            ns = cosv * vecs[i][sv] - sinv * vecs[i][tv];
            nt = sinv * vecs[i][sv] +  cosv * vecs[i][tv];
            vecs[i][sv] = ns;
            vecs[i][tv] = nt;
        }
        
        for( u32 i = 0; i < 2; i++ )
        {
            for( u32 j = 0; j < 3; j++ )
            {
                this->oldVecs[i][j] = vecs[i][j] / _scale[i];
            }
        }
        
        this->oldVecs[0][3] = shift[0];
        this->oldVecs[1][3] = shift[1];
    }
    void calcTexCoordsForPoint( const vec3_c& xyz, const mtrAPI_i* mat, vec2_c& out ) const
    {
        // calculate texture s/t
        out.x = this->oldVecs[0][3] + xyz.dotProduct( this->oldVecs[0] );
        out.y = this->oldVecs[1][3] + xyz.dotProduct( this->oldVecs[1] );
        
        out.x /= mat->getImageWidth();
        out.y /= mat->getImageHeight();
    }
    void calcTexCoordsForPointNEW( const vec3_c& xyz, const mtrAPI_i* mat, vec2_c& out, const plane_c& pl ) const
    {
        vec3_c texX, texY;
        
        vec4_t newVecs[2];
        MOD_TextureAxisFromNormal( pl.norm, texX, texY );
        for( u32 i = 0; i < 2; i++ )
        {
            newVecs[i][0] = texX[0] * texMat[i][0] + texY[0] * texMat[i][1];
            newVecs[i][1] = texX[1] * texMat[i][0] + texY[1] * texMat[i][1];
            newVecs[i][2] = texX[2] * texMat[i][0] + texY[2] * texMat[i][1];
            newVecs[i][3] = texMat[i][2];// + ( origin.dotProduct(v[i]));
        }
        
        out.x = xyz.dotProduct( newVecs[0] ) + newVecs[0][3];
        out.y = xyz.dotProduct( newVecs[1] ) + newVecs[1][3];
    }
};

#include <qcommon/q_shared.h>
inline const char* G_strFind( const char* buf, const char* s )
{
    u32 sLen = strlen( s );
    const char* p = buf;
    while( *p )
    {
        if( !Q_stricmpn( p, s, sLen ) )
            return p;
        p++;
    }
    return 0;
}
static bool MOD_NeedsMaterial( const char* matName, staticModelCreatorAPI_i* c )
{
    if( G_strFind( matName, "nodraw" ) )
        return false;
    // Id Tech 4 editor/visportal material
    if( G_strFind( matName, "visportal" ) )
        return false;
    if( G_strFind( matName, "caulk" ) )
        return false;
    return true;
}

static bool MOD_ConvertBrushD3( class parser_c& p, staticModelCreatorAPI_i* out )
{
    arraySTD_c<r_brushSide_s> sides;
    
    // new brush format (with explicit plane equations)
    // Number of sides isnt explicitly specified,
    // so parse until the closing brace is hit
    if( p.atWord( "{" ) == false )
    {
        g_core->RedWarning( "brush_c::parseBrushD3: expected { to follow brushDef3, found %s at line %i of file %s\n",
                            p.getToken(), p.getCurrentLineNumber(), p.getDebugFileName() );
        return true;
    }
    while( p.atWord_dontNeedWS( "}" ) == false )
    {
        //  ( -0 -0 -1 1548 ) ( ( 0.0078125 0 -4.0625004768 ) ( -0 0.0078125 0.03125 ) ) "textures/common/clip" 0 0 0
        
        if( p.atWord( "(" ) == false )
        {
            g_core->RedWarning( "brush_c::parseBrushD3: expected ( to follow brushSide plane equation, found %s at line %i of file %s\n",
                                p.getToken(), p.getCurrentLineNumber(), p.getDebugFileName() );
            return true;
        }
        
        r_brushSide_s& newSide = sides.pushBack();
        
        p.getFloatMat( newSide.plane.norm, 3 );
        newSide.plane.dist = p.getFloat();
        
        if( p.atWord( ")" ) == false )
        {
            g_core->RedWarning( "brush_c::parseBrushD3: expected ) after brushSide plane equation, found %s at line %i of file %s\n",
                                p.getToken(), p.getCurrentLineNumber(), p.getDebugFileName() );
            return true;
        }
        
        if( p.getFloatMat2D_braced( 2, 3, &newSide.texMat[0].x ) )
        {
            g_core->RedWarning( "brush_c::parseBrushD3: failed to read brush texMat at line %i of file %s\n",
                                p.getToken(), p.getCurrentLineNumber(), p.getDebugFileName() );
            return true;
        }
        
        // get material name
        newSide.matName = p.getToken();
        // after material name we usually have 3 zeroes
        
        if( p.isAtEOL() == false )
        {
            p.skipLine();
        }
    }
    if( p.atWord( "}" ) == false )
    {
        g_core->RedWarning( "brush_c::parseBrushD3: expected } after brushDef3, found %s at line %i of file %s\n",
                            p.getToken(), p.getCurrentLineNumber(), p.getDebugFileName() );
        return true;
    }
    arraySTD_c<simpleVert_s> outVerts;
    //
    // 2. convert brush to triangle soup
    //
    u32 c_badSides = 0;
    for( u32 i = 0; i < sides.size(); i++ )
    {
        const r_brushSide_s& bs = sides[i];
        if( MOD_NeedsMaterial( bs.matName, out ) == false )
            continue; // we dont need this side for rendering
        cmWinding_c winding;
        // create an "infinite" rectangle lying on the current side plane
        winding.createBaseWindingFromPlane( bs.plane );
        for( u32 j = 0; j < sides.size(); j++ )
        {
            if( i == j )
                continue; // dont clip by self
            // clip it by other side planes
            winding.clipWindingByPlane( sides[j].plane.getOpposite() );
        }
        if( winding.size() == 0 )
        {
            c_badSides++;
            continue;
        }
        // ensure that we have enough of space in output vertices array
        if( outVerts.size() < winding.size() )
        {
            outVerts.resize( winding.size() );
        }
        // unfortunatelly I need to access brushSide's material here,
        // because its image dimensions (width and height)
        // are needed to calculate valid texcoords
        // from q3 texVecs
        mtrAPI_i* material = g_ms->registerMaterial( bs.matName );
        // now we have XYZ positions of brush vertices
        // next step is to calculate the texture coordinates for each vertex
        simpleVert_s* o = outVerts.getArray();
        for( u32 j = 0; j < winding.size(); j++, o++ )
        {
            const vec3_c& p = winding[j];
            o->xyz = p;
            // calculate o->tc
            bs.calcTexCoordsForPointNEW( o->xyz, material, o->tc, sides[i].plane );
        }
        // now we have a valid polygon with texcoords
        // let's triangulate it
        u32 prev = 1;
        for( u32 j = 2; j < winding.size(); j++ )
        {
            out->addTriangle( bs.matName, outVerts[0], outVerts[prev], outVerts[j] );
            prev = j;
        }
    }
    return false; // no error
}

static bool MOD_ConvertBrushQ3( class parser_c& p, staticModelCreatorAPI_i* out )
{
    arraySTD_c<r_brushSide_s> sides;
    
    //
    // 1. parse brush data from .map file
    //
    // old brush format
    // Number of sides isnt explicitly specified,
    // so parse until the closing brace is hit
    while( p.atWord( "}" ) == false )
    {
        // ( 2304 -512 1024 ) ( 2304 -768 1024 ) ( -2048 -512 1024 ) german/railgun_flat 0 0 0 0.5 0.5 0 0 0
        vec3_c p0;
        if( p.getFloatMat_braced( p0, 3 ) )
        {
            g_core->RedWarning( "MOD_ConvertBrushQ3: failed to read old brush def first point in file %s at line %i\n", p.getDebugFileName(), p.getCurrentLineNumber() );
            return true; // error
        }
        vec3_c p1;
        if( p.getFloatMat_braced( p1, 3 ) )
        {
            g_core->RedWarning( "MOD_ConvertBrushQ3: failed to read old brush def second point in file %s at line %i\n", p.getDebugFileName(), p.getCurrentLineNumber() );
            return true; // error
        }
        vec3_c p2;
        if( p.getFloatMat_braced( p2, 3 ) )
        {
            g_core->RedWarning( "MOD_ConvertBrushQ3: failed to read old brush def third point in file %s at line %i\n", p.getDebugFileName(), p.getCurrentLineNumber() );
            return true; // error
        }
        r_brushSide_s& newSide = sides.pushBack();
        newSide.plane.fromThreePointsINV( p0, p1, p2 );
        str matNameToken = p.getToken();
        newSide.matName = "textures/";
        newSide.matName.append( matNameToken );
        
        // so called "old brush format".
        // quake texture vecs should looks like this: 0 0 0 0.5 0.5 0 0 0
        // shift[0], shift[1], rotation, scale[0] (here 0.5), scale[1] (here 0.5), ? ? ?
        vec2_c shift, scale;
        float rotation;
        p.getFloatMat( shift, 2 );
        rotation = p.getFloat();
        p.getFloatMat( scale, 2 );
        newSide.setQ3TexVecs( shift, rotation, scale );
        //int mapContents = p.getInteger();
        
        p.skipLine();
        
        // check for extra surfaceParms (MoHAA-specific ?)
        if( p.atWord_dontNeedWS( "+" ) )
        {
            if( p.atWord( "surfaceparm" ) )
            {
                const char* surfaceParmName = p.getToken();
                
            }
            else
            {
                g_core->RedWarning( "cmBrush_c::parseBrushQ3: unknown extra parameters %s\n", p.getToken() );
                p.skipLine();
            }
        }
    }
    arraySTD_c<simpleVert_s> outVerts;
    //
    // 2. convert brush to triangle soup
    //
    u32 c_badSides = 0;
    for( u32 i = 0; i < sides.size(); i++ )
    {
        const r_brushSide_s& bs = sides[i];
        if( MOD_NeedsMaterial( bs.matName, out ) == false )
            continue; // we dont need this side for rendering
        cmWinding_c winding;
        // create an "infinite" rectangle lying on the current side plane
        winding.createBaseWindingFromPlane( bs.plane );
        for( u32 j = 0; j < sides.size(); j++ )
        {
            if( i == j )
                continue; // dont clip by self
            // clip it by other side planes
            winding.clipWindingByPlane( sides[j].plane.getOpposite() );
        }
        if( winding.size() == 0 )
        {
            c_badSides++;
            continue;
        }
        // ensure that we have enough of space in output vertices array
        if( outVerts.size() < winding.size() )
        {
            outVerts.resize( winding.size() );
        }
        // unfortunatelly I need to access brushSide's material here,
        // because its image dimensions (width and height)
        // are needed to calculate valid texcoords
        // from q3 texVecs
        mtrAPI_i* material = g_ms->registerMaterial( bs.matName );
        // now we have XYZ positions of brush vertices
        // next step is to calculate the texture coordinates for each vertex
        simpleVert_s* o = outVerts.getArray();
        for( u32 j = 0; j < winding.size(); j++, o++ )
        {
            const vec3_c& p = winding[j];
            o->xyz = p;
            // calculate o->tc
            bs.calcTexCoordsForPoint( o->xyz, material, o->tc );
        }
        // now we have a valid polygon with texcoords
        // let's triangulate it
        u32 prev = 1;
        for( u32 j = 2; j < winding.size(); j++ )
        {
            out->addTriangle( bs.matName, outVerts[0], outVerts[prev], outVerts[j] );
            prev = j;
        }
    }
    return false; // no error
}

bool MOD_LoadConvertMapFileToStaticTriMesh( const char* fname, staticModelCreatorAPI_i* out )
{
    parser_c p;
    if( p.openFile( fname ) )
    {
        g_core->RedWarning( "MOD_LoadConvertMapFileToStaticTriMesh: cannot open %s\n", fname );
        return 0;
    }
    bool parseError = false;
    
    if( p.atWord( "version" ) )
    {
        p.getToken(); // skip doom3/quake4 version ident
    }
    u32 entityNum = 0;
    while( p.atEOF() == false && parseError == false )
    {
        if( p.atWord( "{" ) )
        {
            // enter new entity
            out->onNewMapEntity( entityNum );
            while( p.atWord( "}" ) == false && parseError == false )
            {
                if( p.atEOF() )
                {
                    g_core->RedWarning( "CM_LoadModelFromMapFile: unexpected end of file hit while parsing %s\n", fname );
                    break;
                }
                if( p.atWord( "{" ) )
                {
                    p.skipToNextToken();
                    const char* primitiveTypeTokenAt = p.getCurDataPtr();
                    // enter new primitive
                    if( p.atWord( "brushDef3" ) )
                    {
#if 0
                        if( p.skipCurlyBracedBlock() )
                        {
                            g_core->RedWarning( "MOD_LoadConvertMapFileToStaticTriMesh: error while parsing brushDef3 at line %i of %s\n", p.getCurrentLineNumber(), fname );
                            parseError = true;
                            break;
                        }
#else
                        if( MOD_ConvertBrushD3( p, out ) )
                        {
                            g_core->RedWarning( "MOD_LoadConvertMapFileToStaticTriMesh: error while parsing brushDef3 at line %i of %s\n", p.getCurrentLineNumber(), fname );
                            parseError = true;
                            break;
                        }
#endif
                    }
                    else if( p.atWord( "patchDef2" ) || p.atWord( "patchDef3" ) )
                    {
                        if( p.skipCurlyBracedBlock() )
                        {
                            g_core->RedWarning( "MOD_LoadConvertMapFileToStaticTriMesh: error while parsing patchDef2 at line %i of %s\n", p.getCurrentLineNumber(), fname );
                            parseError = true;
                            break;
                        }
                        if( p.atWord( "}" ) == false )
                        {
                            g_core->RedWarning( "MOD_LoadConvertMapFileToStaticTriMesh: error while parsing patchDef2 at line %i of %s\n", p.getCurrentLineNumber(), fname );
                            parseError = true;
                            break;
                        }
                        const char* patchDefEnd = p.getCurDataPtr();
                        // let the caller handle bezier patches
                        out->onBezierPatch( primitiveTypeTokenAt, patchDefEnd );
                    }
                    else
                    {
                        if( MOD_ConvertBrushQ3( p, out ) )
                        {
                            g_core->RedWarning( "MOD_LoadConvertMapFileToStaticTriMesh: error while parsing old brush format at line %i of %s\n", p.getCurrentLineNumber(), fname );
                            parseError = true;
                            break;
                        }
                    }
                }
                else
                {
                    // parse key pair
                    str key, val;
                    p.getToken( key );
                    p.getToken( val );
                    //
                    //out->onMapEntityKeyValue(entityNum,key,val);
                }
            }
            entityNum++;
        }
        else
        {
            int line = p.getCurrentLineNumber();
            str token = p.getToken();
            g_core->RedWarning( "MOD_LoadConvertMapFileToStaticTriMesh: unknown token %s at line %i of %s\n", token.c_str(), line, fname );
            parseError = true;
        }
    }
    return false; // no error
}



