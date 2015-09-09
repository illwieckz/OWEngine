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
//  File name:   quake3Anims.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: .MD3 animation indexes
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __QUAKE3ANIMS_H__
#define __QUAKE3ANIMS_H__

// animations (from Quake3 bg_public.h)
enum q3AnimNumber_e
{
    BOTH_DEATH1,
    BOTH_DEAD1,
    BOTH_DEATH2,
    BOTH_DEAD2,
    BOTH_DEATH3,
    BOTH_DEAD3,
    
    TORSO_GESTURE,
    
    TORSO_ATTACK,
    TORSO_ATTACK2,
    
    TORSO_DROP,
    TORSO_RAISE,
    
    TORSO_STAND,
    TORSO_STAND2,
    
    LEGS_WALKCR,
    LEGS_WALK,
    LEGS_RUN,
    LEGS_BACK,
    LEGS_SWIM,
    
    LEGS_JUMP,
    LEGS_LAND,
    
    LEGS_JUMPB,
    LEGS_LANDB,
    
    LEGS_IDLE,
    LEGS_IDLECR,
    
    LEGS_TURN,
    
    TORSO_GETFLAG,
    TORSO_GUARDBASE,
    TORSO_PATROL,
    TORSO_FOLLOWME,
    TORSO_AFFIRMATIVE,
    TORSO_NEGATIVE,
    
    Q3_MAX_ANIMATIONS,
    
    LEGS_BACKCR,
    LEGS_BACKWALK,
    FLAG_RUN,
    FLAG_STAND,
    FLAG_STAND2RUN,
    
    MAX_TOTALANIMATIONS
};

#endif // __QUAKE3ANIMS_H__
