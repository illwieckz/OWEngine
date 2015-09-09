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
//  File name:   cgameAPI.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: server game DLL interface
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __CGAMEAPI_H__
#define __CGAMEAPI_H__

#include "iFaceBase.h"

#define CGAME_API_IDENTSTR "ClientGameAPI0001"

// these are only temporary function pointers, TODO: rework them?
struct cgameAPI_s : public iFaceBase_i
{
	// called when the level loads or when the renderer is restarted
	// all media should be registered at this time
	// cgame will display loading status by calling SCR_Update, which
	// will call CG_DrawInformation during the loading process
	// reliableCommandSequence will be 0 on fresh loads, but higher for
	// demos, tourney restarts, or vid_restarts
	void ( *Init )( int serverMessageNum, int serverCommandSequence, int clientNum );
	// oportunity to flush and close any open files
	void ( *Shutdown )( void );
	// Generates and draws a game scene and status information at the given time.
	// If demoPlayback is set, local movement prediction will not be enabled
	void ( *DrawActiveFrame )( int serverTime, bool demoPlayback );
};

extern cgameAPI_s* g_cgame;

#endif // __GAMEAPI_H__
