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
//  File name:   mat_public.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __MAT_PUBLIC_H__
#define __MAT_PUBLIC_H__

#include <shared/typedefs.h>

// material stage type.
enum stageType_e
{
    ST_NOT_SET,
    ST_LIGHTMAP, // draw only lightmap (0th texture slot)
    ST_COLORMAP, // draw only colormap (0th texture slot)
    ST_COLORMAP_LIGHTMAPPED, // draw lightmapped colormap (colormap at slot 0, lightmap at slot 1)
    // added for Doom3
    ST_BUMPMAP, // normal map
    ST_SPECULARMAP,
    // added by me, heightmaps are not used directly in Doom3
    // (they are converted to bump maps)
    ST_HEIGHTMAP,
    // skybox stage (with a cubemap texture)
    // Set by "stage skyboxmap" in material,
    // also set by Doom3 "texgen skybox" command.
    // Used in Doom3 sky materials.
    // Also used for sky in test_tree.pk3.
    ST_CUBEMAP_SKYBOX,
    // glass reflection stage (with a cubemap texture)
    // Set by "stage reflectionMap" in material,
    // also set by Doom3 "texgen reflect" command.
    // Used in Doom3 glass materials.
    ST_CUBEMAP_REFLECTION,
    // this will automatically use nearest 'env_cubemap' for reflections
    ST_ENV_CUBEMAP,
};

// hardcoded alpha func test values (for non-blended transparency)
enum alphaFunc_e
{
    // Quake3 alphaFuncs
    AF_NONE,
    AF_GT0,
    AF_LT128,
    AF_GE128,
    // Doom3 "alphaTest" <expression>
    // (The expression is evaluated at runtime)
    AF_D3_ALPHATEST,
};

// texCoordGens automatically
// overwrites existing texcoords
enum texCoordGen_e
{
    TCG_NONE,
    TCG_ENVIRONMENT,
    // added for Doom3
    //TCG_SKYBOX,
    //TCG_REFLECT,
    TCG_NUM_GENS,
};

// vertex color generator (red, green, blue)
enum rgbGen_e
{
    RGBGEN_NONE,
    RGBGEN_WAVE,
    RGBGEN_VERTEX,
    RGBGEN_CONST,
    RGBGEN_IDENTITY,
    RGBGEN_IDENTITYLIGHTING,
    RGBGEN_STATIC,
    RGBGEN_LIGHTINGSPHERICAL,
    RGBGEN_EXACTVERTEX,
    RGBGEN_CONSTLIGHTING,
    RGBGEN_LIGHTINGDIFFUSE,
    // now D3 rgbgens:
    RGBGEN_AST, // one D3 material script expression for r,g,b
};

// blend modes; abstracted so they can apply to both opengl and dx
// if you edit this enum, remember to mirror your changes in blendModeEnumToGLBlend array.
enum blendMode_e
{
    BM_NOT_SET,
    BM_ZERO,
    BM_ONE,
    BM_ONE_MINUS_SRC_COLOR,
    BM_ONE_MINUS_DST_COLOR,
    BM_ONE_MINUS_SRC_ALPHA,
    BM_ONE_MINUS_DST_ALPHA,
    BM_DST_COLOR,
    BM_DST_ALPHA,
    BM_SRC_COLOR,
    BM_SRC_ALPHA,
    BM_SRC_ALPHA_SATURATE,
    
    BM_NUM_BLEND_TYPES,
};

struct blendDef_s
{
    u16 src;
    u16 dst;
    
    blendDef_s()
    {
        src = BM_NOT_SET;
        dst = BM_NOT_SET;
    }
    bool isNonZero() const
    {
        if( src )
            return true;
        if( dst )
            return true;
        return false;
    }
};

class skyBoxAPI_i
{
public:
    virtual class textureAPI_i* getUp() const = 0;
    virtual class textureAPI_i* getDown() const = 0;
    virtual class textureAPI_i* getRight() const = 0;
    virtual class textureAPI_i* getLeft() const = 0;
    virtual class textureAPI_i* getFront() const = 0;
    virtual class textureAPI_i* getBack() const = 0;
    
    virtual bool isValid() const = 0;
};

class skyParmsAPI_i
{
public:
    virtual float getCloudHeight() const = 0;
    virtual const skyBoxAPI_i* getFarBox() const = 0;
    virtual const skyBoxAPI_i* getNearBox() const = 0;
};

class sunParmsAPI_i
{
public:
    virtual const class vec3_c& getSunDir() const = 0;
    virtual const class vec3_c& getSunColor() const = 0;
};

#endif // __MAT_PUBLIC_H__
