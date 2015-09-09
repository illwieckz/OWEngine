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
//  File name:   cm_modelLoaderWrapper.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: simplewrapper for modelLoaderDLL
//               NOTE: so it can be used for generating coallision models
//                     from render models
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include <api/coreAPI.h>
#include <api/modelLoaderDLLAPI.h>
#include <shared/cmSurface.h>

bool CM_LoadRenderModelToSingleSurface( const char* rModelName, class cmSurface_c& out )
{
    if( g_modelLoader->loadStaticModelFile( rModelName, &out ) )
    {
        return true; // error
    }
    return false; // OK
}
