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
//  File name:   sysEventCasterAPI.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SYSEVENTCASTERAPI_H__
#define __SYSEVENTCASTERAPI_H__

#include "iFaceBase.h"

#define SYSEVENTCASTER_API_IDENTSTR "SysEventCasterAPI0001"

class sysEventCasterAPI_c : public iFaceBase_i
{
public:
    virtual void postMouseMotionEvent( float deltaX, float deltaY )  = 0;
    virtual void postMouseRightButtonDownEvent()  = 0;
    virtual void postMouseRightButtonUpEvent()  = 0;
    virtual void postMouseLeftButtonDownEvent()  = 0;
    virtual void postMouseLeftButtonUpEvent() = 0;
};

extern sysEventCasterAPI_c* g_sysEventCaster;

#endif // __SYSEVENTCASTERAPI_H__
