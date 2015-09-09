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
//  File name:   quake3AnimationConfig.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Quake 3 animation.cfg files loader
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SHARED_QUAKE3ANIMATIONCONFIG_H__
#define __SHARED_QUAKE3ANIMATIONCONFIG_H__

#include <shared/typedefs.h>
#include <shared/array.h>

// first frame, num frames, looping frames, frames per second
struct q3AnimDef_s
{
    u32 firstFrame;
    u32 numFrames;
    u32 loopingFrames;
    float FPS;
    float frameTime;
    float totalTime;
    
    inline void calcFrameTimeFromFPS()
    {
        frameTime = 1.f / FPS;
    }
    inline void calcTotalTimeFromFrameTime()
    {
        totalTime = float( numFrames ) * frameTime;
    }
};

class q3AnimCfg_c
{
    arraySTD_c<q3AnimDef_s> anims;
public:
    bool parse( const char* fileName );
    const struct q3AnimDef_s* getAnimCFGForIndex( u32 localAnimIndex ) const
    {
        if( anims.size() <= localAnimIndex )
            return 0;
        return &anims[localAnimIndex];
    }
};

#endif // __SHARED_QUAKE3ANIMATIONCONFIG_H__

