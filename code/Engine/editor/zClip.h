////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
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
//  File name:   zClip.cpp
//  Version:     v1.00
//  Created:     09-25-2015
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef ZCLIP_H
#define ZCLIP_H

// I don't like doing macros without braces and with whitespace, but the compiler moans if I do these differently,
//	and since they're only for use within glColor3f() calls anyway then this is ok...  (that's my excuse anyway)
//
#define ZCLIP_COLOUR		1.0, 0.0, 1.0
#define ZCLIP_COLOUR_DIM	0.8, 0.0, 0.8

class CZClip
{
	public:
		CZClip();
		~CZClip();
		
		int		GetTop( void );
		int		GetBottom( void );
		void	SetTop( int iNewZ );
		void	SetBottom( int iNewZ );
		void	Reset( void );
		bool	IsEnabled( void );
		bool	Enable( bool bOnOff );
		void	Paint( void );
		
	protected:
		void	Legalise( void );
		
		bool	m_bEnabled;
		int		m_iZClipTop;
		int		m_iZClipBottom;
};


#endif	// #ifndef ZCLIP_H

