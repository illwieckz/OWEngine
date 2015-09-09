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
//  File name:   WaveOpen.cpp
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
#include "WaveOpen.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( CWaveOpen, CFileDialog )

CWaveOpen::CWaveOpen( BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
                      DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd ) :
    CFileDialog( bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd )
{
    m_ofn.Flags |= ( OFN_EXPLORER | OFN_ENABLETEMPLATE );
    m_ofn.lpTemplateName = MAKEINTRESOURCE( IDD_PLAYWAVE );
}


BEGIN_MESSAGE_MAP( CWaveOpen, CFileDialog )
    //{{AFX_MSG_MAP(CWaveOpen)
    ON_BN_CLICKED( IDC_BTN_PLAY, OnBtnPlay )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CWaveOpen::OnFileNameChange()
{
    CString str = GetPathName();
    str.MakeLower();
    CWnd* pWnd = GetDlgItem( IDC_BTN_PLAY );
    if( pWnd == NULL )
    {
        return;
    }
    if( str.Find( ".wav" ) >= 0 )
    {
        pWnd->EnableWindow( TRUE );
    }
    else
    {
        pWnd->EnableWindow( FALSE );
    }
}

void CWaveOpen::OnBtnPlay()
{
//  sndPlaySound(NULL, NULL);
    //CString str = GetPathName();
    //if (str.GetLength() > 0)
    //{
    //  sndPlaySound(str, SND_FILENAME | SND_ASYNC);
    //}
}

BOOL CWaveOpen::OnInitDialog()
{
    CFileDialog::OnInitDialog();
    
    CWnd* pWnd = GetDlgItem( IDC_BTN_PLAY );
    if( pWnd != NULL )
    {
        pWnd->EnableWindow( FALSE );
    }
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
