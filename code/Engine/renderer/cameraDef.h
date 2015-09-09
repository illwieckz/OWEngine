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
//  File name:   cameraDef.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __CAMERADEF_H__
#define __CAMERADEF_H__

#include <math/vec3.h>
#include <math/axis.h>
#include <math/frustum.h>

#include <api/rbAPI.h>

class cameraDef_c
{
    projDef_s proj;
    vec3_c origin;
    // pvsOrigin is different than this->origin for mirrors and portals
    // (it's the real, non-reflected origin)
    vec3_c pvsOrigin;
    //vec3_c angles;
    axis_c axis;
    frustum_c frustum;
    bool thirdPersonRendering;
    bool bIsPortal;
    bool bIsMirror;
    plane_c portalPlane;
    u32 portalDepth;
public:
    cameraDef_c()
    {
        thirdPersonRendering = false;
        bIsPortal = false;
        bIsMirror = false;
        portalDepth = 0;
    }
    void setup( const vec3_c& newOrigin, const axis_c& newAxis, const projDef_s& pd, bool bThirdPersonRendering = false )
    {
        origin = newOrigin;
        pvsOrigin = newOrigin;
        axis = newAxis;
        proj = pd;
        frustum.setup( proj.fovX, proj.fovY, proj.zFar, axis, origin );
        thirdPersonRendering = bThirdPersonRendering;
    }
    void setup( const vec3_c& newOrigin, const axis_c& newAxis )
    {
        origin = newOrigin;
        pvsOrigin = newOrigin;
        axis = newAxis;
        frustum.setup( proj.fovX, proj.fovY, proj.zFar, axis, origin );
    }
    void setPVSOrigin( const vec3_c& newPVSOrigin )
    {
        pvsOrigin = newPVSOrigin;
    }
    void setPortalPlane( const plane_c& pl )
    {
        portalPlane = pl;
        // add epsilon value so mirrors don't blink
        portalPlane.addDist( 0.1f );
        bIsPortal = true;
        portalDepth++;
    }
    void setIsMirror( bool newBIsMirror )
    {
        bIsMirror = newBIsMirror;
    }
    u32 getPortalDepth() const
    {
        return portalDepth;
    }
    
    bool isThirdPerson() const
    {
        return thirdPersonRendering;
    }
    const frustum_c& getFrustum() const
    {
        return frustum;
    }
    const vec3_c& getOrigin() const
    {
        return origin;
    }
    const vec3_c& getPVSOrigin() const
    {
        return pvsOrigin;
    }
    const axis_c& getAxis() const
    {
        return axis;
    }
    const vec3_c& getForward() const
    {
        return axis.getForward();
    }
    const projDef_s& getProjDef() const
    {
        return proj;
    }
    float getZFar() const
    {
        return proj.getZFar();
    }
    const plane_c& getPortalPlane() const
    {
        return portalPlane;
    }
    bool isPortal() const
    {
        return bIsPortal;
    }
    bool isMirror() const
    {
        return bIsMirror;
    }
};

#endif // __CAMERADEF_H__
