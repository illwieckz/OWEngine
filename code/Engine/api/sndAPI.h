////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
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
//  File name:   sndAPI.h
//  Version:     v1.00
//  Created:     09-16-2016
//  Compilers:   Visual Studio
//  Description: sound system DLL interface
// -------------------------------------------------------------------------
//  History:
//  09-16-2015: Added basic support for sound module
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SND_API_H__
#define __SND_API_H__

#include "iFaceBase.h"

#define SND_API_IDENTSTR "SndEngineAPI0001"

// these are only temporary function pointers, TODO: rework them?
struct sndAPI_s : public iFaceBase_i
{

};

extern sndAPI_s* g_sndAPI;

#endif // __SNDAPI_H__

