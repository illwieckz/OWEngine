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
//  File name:   modelDeclAPI.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __MODELDECLAPI_H__
#define __MODELDECLAPI_H__

class animDefAPI_i
{
public:
    //virtual const char *getAlias() const = 0;
    //virtual const char *getAnimFileName() const = 0;
    //virtual class skelAnimAPI_i *getAnim() const = 0;
};

class modelDeclAPI_i
{
public:
    virtual const char* getModelDeclName() const = 0;
    virtual const char* getMeshName() const = 0;
    virtual class skelModelAPI_i* getSkelModel() = 0;
    virtual const class vec3_c& getOffset() const = 0;
    
    virtual u32 getNumBones() = 0;
    virtual u32 getNumSurfaces() = 0;
    virtual u32 getTotalTriangleCount() = 0;
    
    // bones / tags
    virtual int getBoneNumForName( const char* boneName ) const = 0;
    virtual const char* getBoneName( u32 boneIndex ) const = 0;
    
    //// animations list access
    virtual u32 getNumAnims() const = 0;
    //virtual const char *animAliasForNum(u32 index) = 0;
    //virtual const char *animFileNameForNum(u32 index) = 0;
    //virtual const class animDefAPI_i *animDefForNum(u32 index) = 0;
    virtual int getAnimIndexForAnimAlias( const char* alias ) const = 0;
    virtual const class skelAnimAPI_i* getSkelAnimAPIForAlias( const char* alias ) const = 0;
    virtual const class skelAnimAPI_i* getSkelAnimAPIForLocalIndex( int localIndex ) const = 0;
    virtual bool hasAnim( const char* animName ) const = 0;
    virtual int getAnimationTimeMSec( const char* alias ) const = 0;
    // debug output
    virtual void printBoneNames() const = 0;
};

#endif // __MODELDECLAPI_H__