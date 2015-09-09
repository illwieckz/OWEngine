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
//  File name:   btp_characterController.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __BTP_CHARACTERCONTROLLER_H__
#define __BTP_CHARACTERCONTROLLER_H__

#include <api/physCharacterControllerAPI.h>
#include <math/vec3.h>

class btpCharacterController_c : public physCharacterControllerAPI_i
{
    class btKinematicCharacterController* ch;
    class btConvexShape* characterShape;
    class bulletPhysicsWorld_c* myWorld;
    mutable vec3_c lastPos;
    //vec3_c velocity;
public:
    btpCharacterController_c();
    ~btpCharacterController_c();
    
    virtual void setCharacterVelocity( const class vec3_c& newVel );
    virtual void setCharacterEntity( class BaseEntity* ent );
    virtual void update( const class vec3_c& dir );
    //virtual const class vec3_c &getVelocity() const;
    virtual const class vec3_c& getPos() const;
    virtual bool isOnGround() const;
    virtual bool tryToJump();
    
    void init( class bulletPhysicsWorld_c* pWorld, const class vec3_c& pos, float characterHeight, float characterWidth );
    void destroyCharacter();
};

#endif // __BTP_CHARACTERCONTROLLER_H__
