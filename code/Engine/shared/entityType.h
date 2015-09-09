////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2014 V.
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
//  File name:   entityType.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SHARED_ENTITYTYPE_H__
#define __SHARED_ENTITYTYPE_H__

//
// entityState_s->eType
//
enum entityType_e
{
    ET_GENERAL,
    ET_PLAYER,
    // dynamic light entity
    ET_LIGHT,
    // camera view portal; used for example on q3dm0
    ET_PORTAL, // classname: misc_portal_surface
    // server-only entity for triggers
    ET_TRIGGER,
    // path node for AI navigation (server-only)
    // the classname is: "info_pathnode"
    ET_PATHNODE,
    // static object (used heavily on Doom3 maps)
    ET_FUNC_STATIC,
    // info_location entities
    ET_INFO_LOCATION,
};

#endif // __SHARED_ENTITYTYPE_H__

