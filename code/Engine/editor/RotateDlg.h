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
//  File name:   RotateDlg.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROTATEDLG_H__D4B79152_7A7E_11D1_B541_00AA00A410FC__INCLUDED_)
#define AFX_ROTATEDLG_H__D4B79152_7A7E_11D1_B541_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CRotateDlg dialog

class CRotateDlg : public CDialog
{
// Construction
public:
    CRotateDlg( CWnd* pParent = NULL ); // standard constructor
    
// Dialog Data
    //{{AFX_DATA(CRotateDlg)
    enum { IDD = IDD_ROTATE };
    CSpinButtonCtrl	m_wndSpin3;
    CSpinButtonCtrl	m_wndSpin2;
    CSpinButtonCtrl	m_wndSpin1;
    CString	m_strX;
    CString	m_strY;
    CString	m_strZ;
    //}}AFX_DATA
    
    
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRotateDlg)
protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV support
    //}}AFX_VIRTUAL
    
// Implementation
protected:
    void ApplyNoPaint();
    
    // Generated message map functions
    //{{AFX_MSG(CRotateDlg)
    virtual void OnOK();
    afx_msg void OnApply();
    virtual BOOL OnInitDialog();
    afx_msg void OnDeltaposSpin1( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnDeltaposSpin2( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnDeltaposSpin3( NMHDR* pNMHDR, LRESULT* pResult );
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATEDLG_H__D4B79152_7A7E_11D1_B541_00AA00A410FC__INCLUDED_)
