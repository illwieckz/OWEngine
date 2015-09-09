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
//  File name:   portalizedWorldAPI.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __PORTALIZEDWORLDAPI_H__
#define __PORTALIZEDWORLDAPI_H__

class portalAPI_i
{
public:
    virtual int getFrontArea() const = 0;
    virtual int getBackArea() const = 0;
    virtual int getOtherAreaNum( int oneOfAreas ) const = 0;
    virtual const class cmWinding_c* getWindingP() const = 0;
    virtual u32 getBlockingBits() const = 0;
};
class portalizedWorldAPI_i
{
public:
    // NOTE: if there is only one area,
    // "getNumPortals()" should always return 0.
    virtual u32 getNumAreas() const = 0;
    virtual u32 getNumPortals() const = 0;
    virtual u32 getNumPortalsInArea( u32 areaNum ) const = 0;
    
    virtual int pointAreaNum( const vec3_c& p ) const = 0;
    virtual u32 boxAreaNums( const aabb& bb, int* areaNums, int maxAreaNums ) const = 0;
    
    virtual const class portalAPI_i* getPortal( u32 areaNum, u32 localPortalNum ) const = 0;
};

#endif // __PORTALIZEDWORLDAPI_H__
