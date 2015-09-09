////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2013 V.
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
//  File name:   btp_staticMapLoader.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __BTP_STATICMAPLOADER_H__
#define __BTP_STATICMAPLOADER_H__

//class brushCreatorAPI_i {
//public:
//	virtual void beginBrush(int contents, u32 numSides) = 0;
//	virtual void addBrushSide(const float planeEq[4]) = 0;
//	virtual void finishBrush() = 0;
//};

#include "btp_headers.h"
#include <shared/array.h>
#include <shared/cmSurface.h>


class btpStaticMapLoader_c
{
    class bulletPhysicsWorld_c* myPhysWorld;
    arraySTD_c<class btCollisionShape*> shapes;
    arraySTD_c<class btRigidBody*> bodies;
    btCollisionShape* mainWorldShape;
    btRigidBody* mainWorldBody;
    arraySTD_c<class cmSurface_c*> surfs;
    cmSurface_c mainWorldSurface;
    btCollisionShape* mainWorldSurface_shape;
    btRigidBody* mainWorldSurface_body;
    
    bool loadFromBSPFile( const char* fname );
    bool loadFromPROCFile( const char* fname );
    bool loadFromMAPFile( const char* fname );
public:
    btpStaticMapLoader_c();
    ~btpStaticMapLoader_c();
    
    void createEmptyMap();
    bool loadMap( const char* mapName, class bulletPhysicsWorld_c* pWorld );
    void freeMemory();
    
    // creates a static convex hull object used for world collision detection
    void createWorldBrush( const btAlignedObjectArray<btVector3>& vertices );
    // creates a static trisurf object used for world collision detection
    void createWorldSurface( class cmSurface_c* sf );
    // adds a trisurf to main world mesh
    void addWorldSurface( class cmSurface_c& sf );
};

#endif // __BTP_STATICMAPLOADER_H__
