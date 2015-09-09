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
//  File name:   EditWnd.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITWND_H__279AAE22_78C5_11D1_B53C_00AA00A410FC__INCLUDED_)
#define AFX_EDITWND_H__279AAE22_78C5_11D1_B53C_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CEditWnd window

class CEditWnd : public CEdit
{
		DECLARE_DYNCREATE( CEditWnd );
		// Construction
	public:
		CEditWnd();
		
		// Attributes
	public:
	
		// Operations
	public:
	
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CEditWnd)
	protected:
		virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
		//}}AFX_VIRTUAL
		
		// Implementation
	public:
		virtual ~CEditWnd();
		
		// Generated message map functions
	protected:
		//{{AFX_MSG(CEditWnd)
		//}}AFX_MSG
		
		DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITWND_H__279AAE22_78C5_11D1_B53C_00AA00A410FC__INCLUDED_)
