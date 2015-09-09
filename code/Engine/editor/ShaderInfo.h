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
//  File name:   ShaderInfo.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Interface for the CShaderInfo class.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADERINFO_H__93B64600_A208_11D2_803D_0020AFEB881A__INCLUDED_)
#define AFX_SHADERINFO_H__93B64600_A208_11D2_803D_0020AFEB881A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShaderInfo
{
	public:
		CString m_strName;
		CString m_strShaderName;
		CString m_strTextureName;
		CStringList m_strEditorParams;
		CStringList m_lstSurfaceParams;
		float m_fTransValue;
		int m_nFlags;
		qtexture_t* m_pQTexture;
		
		
		CShaderInfo();
		virtual ~CShaderInfo();
		
		void setName( char* pName );
};

#endif // !defined(AFX_SHADERINFO_H__93B64600_A208_11D2_803D_0020AFEB881A__INCLUDED_)
