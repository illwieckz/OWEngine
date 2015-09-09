////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 1999-2005 Id Software, Inc.
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
//  File name:   ShaderInfo.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Implementation of the CShaderInfo class.
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Radiant.h"
#include "ShaderInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


CShaderInfo::CShaderInfo()
{
    m_fTransValue = 1.0;
    m_nFlags = 0;
    m_pQTexture = NULL;
}

CShaderInfo::~CShaderInfo()
{

}

void CShaderInfo::setName( char* pName )
{
    m_strName = pName;
    m_strName.MakeLower();
    if( m_strName.Find( "textures" ) == 0 )
    {
        CString s = m_strName.Right( m_strName.GetLength() - strlen( "textures" ) - 1 );
        m_strName = s;
    }
}
