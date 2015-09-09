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
//  File name:   DlgEvent.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DlgEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEvent dialog


CDlgEvent::CDlgEvent( CWnd* pParent /*=NULL*/ )
    : CDialog( CDlgEvent::IDD, pParent )
{
    //{{AFX_DATA_INIT(CDlgEvent)
    m_strParm = _T( "" );
    m_event = 0;
    //}}AFX_DATA_INIT
}


void CDlgEvent::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CDlgEvent)
    DDX_Text( pDX, IDC_EDIT_PARAM, m_strParm );
    DDX_Radio( pDX, IDC_RADIO_EVENT, m_event );
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CDlgEvent, CDialog )
    //{{AFX_MSG_MAP(CDlgEvent)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEvent message handlers
