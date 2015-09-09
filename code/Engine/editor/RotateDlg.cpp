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
//  File name:   RotateDlg.cpp
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
#include "RotateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotateDlg dialog


CRotateDlg::CRotateDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( CRotateDlg::IDD, pParent )
{
    //{{AFX_DATA_INIT(CRotateDlg)
    m_strX = _T( "" );
    m_strY = _T( "" );
    m_strZ = _T( "" );
    //}}AFX_DATA_INIT
}


void CRotateDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CRotateDlg)
    DDX_Control( pDX, IDC_SPIN3, m_wndSpin3 );
    DDX_Control( pDX, IDC_SPIN2, m_wndSpin2 );
    DDX_Control( pDX, IDC_SPIN1, m_wndSpin1 );
    DDX_Text( pDX, IDC_ROTX, m_strX );
    DDX_Text( pDX, IDC_ROTY, m_strY );
    DDX_Text( pDX, IDC_ROTZ, m_strZ );
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CRotateDlg, CDialog )
    //{{AFX_MSG_MAP(CRotateDlg)
    ON_BN_CLICKED( ID_APPLY, OnApply )
    ON_NOTIFY( UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1 )
    ON_NOTIFY( UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2 )
    ON_NOTIFY( UDN_DELTAPOS, IDC_SPIN3, OnDeltaposSpin3 )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotateDlg message handlers

void CRotateDlg::OnOK()
{
    OnApply();
    CDialog::OnOK();
}

void CRotateDlg::OnApply()
{
    UpdateData( TRUE );
    float f = atof( m_strX );
    if( f != 0.0 )
        Select_RotateAxis( 0, f );
    f = atof( m_strY );
    if( f != 0.0 )
        Select_RotateAxis( 1, f );
    f = atof( m_strZ );
    if( f != 0.0 )
        Select_RotateAxis( 2, f );
}

BOOL CRotateDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_wndSpin1.SetRange( 0, 359 );
    m_wndSpin2.SetRange( 0, 359 );
    m_wndSpin3.SetRange( 0, 359 );
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CRotateDlg::OnDeltaposSpin1( NMHDR* pNMHDR, LRESULT* pResult )
{
    NM_UPDOWN* pNMUpDown = ( NM_UPDOWN* )pNMHDR;
    Select_RotateAxis( 0, pNMUpDown->iDelta );
    *pResult = 0;
}

void CRotateDlg::OnDeltaposSpin2( NMHDR* pNMHDR, LRESULT* pResult )
{
    NM_UPDOWN* pNMUpDown = ( NM_UPDOWN* )pNMHDR;
    Select_RotateAxis( 1, pNMUpDown->iDelta );
    *pResult = 0;
}

void CRotateDlg::OnDeltaposSpin3( NMHDR* pNMHDR, LRESULT* pResult )
{
    NM_UPDOWN* pNMUpDown = ( NM_UPDOWN* )pNMHDR;
    Select_RotateAxis( 2, pNMUpDown->iDelta );
    *pResult = 0;
}

void CRotateDlg::ApplyNoPaint()
{

}
