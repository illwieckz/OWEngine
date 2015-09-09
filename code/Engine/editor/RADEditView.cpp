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
//  File name:   RADEditView.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Radiant.h"
#include "RADEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRADEditView

IMPLEMENT_DYNCREATE( CRADEditView, CEditView )

CRADEditView::CRADEditView()
{
}

CRADEditView::~CRADEditView()
{
}


BEGIN_MESSAGE_MAP( CRADEditView, CEditView )
	//{{AFX_MSG_MAP(CRADEditView)
	ON_CONTROL_REFLECT( EN_CHANGE, OnChange )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRADEditView drawing

void CRADEditView::OnDraw( CDC* pDC )
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRADEditView diagnostics

#ifdef _DEBUG
void CRADEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CRADEditView::Dump( CDumpContext& dc ) const
{
	CEditView::Dump( dc );
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRADEditView message handlers

void CRADEditView::OnChange()
{
}
