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
//  File name:   iFaceMgrAPI.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: shared interface manager API
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IFACEMGRAPI_H__
#define __IFACEMGRAPI_H__

class iFaceBase_i;

enum qioModule_e
{
    QMD_CORE,
    QMD_GAME,
    QMD_CGAME,
    QMD_RENDERER,
    QMD_MATERIALSYSTEM,
    QMD_IMAGELIB,
    QMD_CM,
    QMD_MODELLOADER,
    QMD_REF_BACKEND_GL,
    QMD_REF_BACKEND_NULL,
    QMD_REF_BACKEND_DX9,
    QMD_DECL_MANAGER,
    QMD_REF_BACKEND_DX10,
    QMD_GPHYSICS,
    QMD_EDITOR,
};

inline bool QM_IsServerSide( qioModule_e m )
{
    if( m == QMD_GAME )
        return true;
    return false;
}

// this must be implemented in every module
qioModule_e IFM_GetCurModule();

class iFaceMgrAPI_i
{
public:
    //
    //	INTERFACE PROVIDERS
    //
    // this should be called for every interface of interface provider dll when its loaded
    virtual void registerInterface( iFaceBase_i* iFace, const char* iFaceName, qioModule_e module ) = 0;
    // this MUST be called before interface provider DLL is unloaded, so all of the external
    // interface pointers are properly NULLed
    virtual void unregisterModuleInterfaces( qioModule_e module ) = 0;
    //
    //	INTERFACE USERS
    //
    // this should be called once at module startup. iFaceMgr will automatically set the iFaceUser ptr
    // as soon as given API is avaible (registered trough registerInterface)
    virtual void registerIFaceUser( iFaceBase_i** iFaceUser, const char* iFaceName, qioModule_e module ) = 0;
    
    //
    // shortcuts so you dont have to type module name every time
    //
    inline void registerInterface( iFaceBase_i* iFace, const char* iFaceName )
    {
        qioModule_e curModule = IFM_GetCurModule();
        registerInterface( iFace, iFaceName, curModule );
    }
    inline void unregisterModuleInterfaces()
    {
        qioModule_e curModule = IFM_GetCurModule();
        unregisterModuleInterfaces( curModule );
    }
    inline void registerIFaceUser( void* iFaceUser, const char* iFaceName )
    {
        qioModule_e curModule = IFM_GetCurModule();
        registerIFaceUser( ( iFaceBase_i** )iFaceUser, iFaceName, curModule );
    }
};


extern iFaceMgrAPI_i* g_iFaceMan;


#endif // __IFACEMGRAPI_H__
