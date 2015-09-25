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
//  File name:   ZWnd.h
//  Version:     v1.01
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//  09-26-2015 : Added zClip for OWEditor
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZWND_H__44B4BA02_781B_11D1_B53C_00AA00A410FC__INCLUDED_)
#define AFX_ZWND_H__44B4BA02_781B_11D1_B53C_00AA00A410FC__INCLUDED_

#include "zclip.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CZWnd window

class CZWnd : public CWnd
{
		DECLARE_DYNCREATE( CZWnd );
		// Construction
	public:
		CZWnd();
		
		// Attributes
	public:
	
		// Operations
	public:
	
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CZWnd)
	protected:
		virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
		//}}AFX_VIRTUAL
		
		// Implementation
	public:
		virtual ~CZWnd();
		
		CZClip* m_pZClip;
		
		// Generated message map functions
	protected:
		HDC m_dcZ;
		HGLRC m_hglrcZ;
		//{{AFX_MSG(CZWnd)
		afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
		afx_msg void OnDestroy();
		afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
		afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
		afx_msg void OnMButtonDown( UINT nFlags, CPoint point );
		afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
		afx_msg void OnPaint();
		afx_msg void OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI );
		afx_msg void OnMouseMove( UINT nFlags, CPoint point );
		afx_msg void OnSize( UINT nType, int cx, int cy );
		afx_msg void OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp );
		afx_msg void OnKillFocus( CWnd* pNewWnd );
		afx_msg void OnSetFocus( CWnd* pOldWnd );
		afx_msg void OnClose();
		afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
		afx_msg void OnMButtonUp( UINT nFlags, CPoint point );
		afx_msg void OnRButtonUp( UINT nFlags, CPoint point );
		afx_msg void OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZWND_H__44B4BA02_781B_11D1_B53C_00AA00A410FC__INCLUDED_)
