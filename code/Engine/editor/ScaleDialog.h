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
//  File name:   ScaleDialog.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEDIALOG_H__8A9B33B2_9922_11D1_B568_00AA00A410FC__INCLUDED_)
#define AFX_SCALEDIALOG_H__8A9B33B2_9922_11D1_B568_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CScaleDialog dialog

class CScaleDialog : public CDialog
{
		// Construction
	public:
		CScaleDialog( CWnd* pParent = NULL ); // standard constructor
		
		// Dialog Data
		//{{AFX_DATA(CScaleDialog)
		enum { IDD = IDD_DIALOG_SCALE };
		float   m_fZ;
		float   m_fX;
		float   m_fY;
		//}}AFX_DATA
		
		
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CScaleDialog)
	protected:
		virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV support
		//}}AFX_VIRTUAL
		
		// Implementation
	protected:
	
		// Generated message map functions
		//{{AFX_MSG(CScaleDialog)
		// NOTE: the ClassWizard will add member functions here
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALEDIALOG_H__8A9B33B2_9922_11D1_B568_00AA00A410FC__INCLUDED_)
