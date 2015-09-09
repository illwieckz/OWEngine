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
//  File name:   inputSystemAPI.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __INPUTSYSTEMAPI_API_H__
#define __INPUTSYSTEMAPI_API_H__

#include <shared/typedefs.h>
#include "iFaceBase.h"

#define INPUT_SYSTEM_API_IDENTSTR "InputSystemAPI0001"

class inputSystemAPI_i : public iFaceBase_i
{
	public:
		void ( *IN_Shutdown )();
		// IN_Init must be called after SDL_Init(SDL_INIT_VIDEO), otherwise we'd get a Com_Error
		void ( *IN_Init )();
		void ( *IN_Restart )();
};

extern inputSystemAPI_i* g_inputSystem;

#endif // __INPUTSYSTEMAPI_API_H__
