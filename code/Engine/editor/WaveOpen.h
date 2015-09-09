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
//  File name:   WaveOpen.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEOPEN_H__0FB9DA11_EB02_11D2_A50A_0020AFEB881A__INCLUDED_)
#define AFX_WAVEOPEN_H__0FB9DA11_EB02_11D2_A50A_0020AFEB881A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWaveOpen dialog

class CWaveOpen : public CFileDialog
{
		DECLARE_DYNAMIC( CWaveOpen )
		
	public:
		CWaveOpen( BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
				   LPCTSTR lpszDefExt = NULL,
				   LPCTSTR lpszFileName = NULL,
				   DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				   LPCTSTR lpszFilter = NULL,
				   CWnd* pParentWnd = NULL );
				   
		virtual void OnFileNameChange( );
	protected:
		//{{AFX_MSG(CWaveOpen)
		afx_msg void OnBtnPlay();
		virtual BOOL OnInitDialog();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEOPEN_H__0FB9DA11_EB02_11D2_A50A_0020AFEB881A__INCLUDED_)
