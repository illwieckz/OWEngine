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
//  File name:   mtrAPI.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: material class interface
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __MTR_API_H__
#define __MTR_API_H__

#include <shared/safePtr.h>

enum deformType_e
{
    DEFORM_BAD,
    // Doom3 "sprite" deform
    DEFORM_AUTOSPRITE,
};

class deformArrayAPI_i
{
public:
    virtual u32 getNumDeforms() const = 0;
    virtual deformType_e getDeformType( u32 idx ) const = 0;
};

class mtrAPI_i //: public safePtrObject_c
{
public:
    // returns the material name (usually the image name without extension)
    virtual const char* getName() const = 0;
    // returns the material def source file name - .shader file or image file (if loaded directly)
    virtual const char* getSourceFileName() const = 0;
    virtual u32 getNumStages() const = 0;
    virtual const class mtrStageAPI_i* getStage( u32 stageNum ) const = 0;
    virtual const class skyParmsAPI_i* getSkyParms() const = 0;
    virtual const class sunParmsAPI_i* getSunParms() const = 0;
    virtual enum drawCallSort_e getSort() const = 0;
    virtual enum cullType_e getCullType() const = 0;
    virtual bool hasTexGen() const = 0;
    virtual bool hasRGBGen() const = 0;
    virtual bool hasBlendFunc() const = 0;
    virtual bool hasStageWithoutBlendFunc() const = 0;
    virtual bool hasStageOfType( enum stageType_e type ) const = 0;
    virtual bool hasOnlyStagesOfType( enum stageType_e type ) const = 0;
    virtual bool hasAlphaTest() const = 0;
    virtual const class mtrStageAPI_i* getFirstStageWithAlphaFunc() const = 0;
    virtual bool isPortalMaterial() const = 0; // returns true if material definition had "portal" keyword
    virtual bool isMirrorMaterial() const = 0; // returns true if material definition had "mirror" keyword
    virtual bool hasDeforms() const = 0;
    virtual class deformArrayAPI_i* getDeformsArray() const = 0;
    virtual bool hasDeformOfType( enum deformType_e type ) const = 0;
    virtual bool hasStageWithCubeMap() const = 0;
    virtual bool isSkyMaterial() const = 0;
    // returns true if material should generate light interactions
    // This should return false for glass materials and explosion sprites.
    virtual bool isNeededForLightPass() const = 0;
    
    // bGenericSky is set in .vmt files by $compilesky key
    // (it's set to 1 in materials/tools/toolskybox.vmt)
    virtual bool isGenericSky() const = 0;
    
    virtual int getImageWidth() const = 0;
    virtual int getImageHeight() const = 0;
    // for animated images ("animmap")
    virtual u32 getColorMapImageFrameCount() const = 0;
    // returns false if all material stages are using "program" (or "vertexProgram"/"fragmentProgram") keyword
    virtual bool hasStageWithoutCustomProgram() const = 0;
    
    virtual float getPolygonOffset() const = 0;
};

#endif // __MTR_API_H__
