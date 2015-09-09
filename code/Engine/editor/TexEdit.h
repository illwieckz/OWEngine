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
//  File name:   TexEdit.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXEDIT_H__913F6812_99CD_11D1_B568_00AA00A410FC__INCLUDED_)
#define AFX_TEXEDIT_H__913F6812_99CD_11D1_B568_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CTexEdit window

class CTexWnd;

class CTexEdit : public CEdit
{
// Construction
public:
    CTexEdit();
    
// Attributes
public:

protected:
    CTexWnd* m_pTexWnd;
    CFont m_Font;
// Operations
public:
    void SetTexWnd( CTexWnd* pTex )
    {
        m_pTexWnd = pTex;
    };
    
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTexEdit)
    //}}AFX_VIRTUAL
    
// Implementation
public:
    virtual ~CTexEdit();
    
    // Generated message map functions
protected:
    //{{AFX_MSG(CTexEdit)
    afx_msg HBRUSH CtlColor( CDC* pDC, UINT nCtlColor );
    afx_msg void OnChange();
    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
    //}}AFX_MSG
    
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXEDIT_H__913F6812_99CD_11D1_B568_00AA00A410FC__INCLUDED_)
