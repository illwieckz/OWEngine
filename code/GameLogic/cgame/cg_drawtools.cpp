////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 1999-2005 Id Software, Inc.
//  Copyright (C) 2014 V.
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
//  File name:   cg_drawtools.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Helper functions called by cg_draw, cg_scoreboard,
//               cg_info, etc
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "cg_local.h"
#include <api/rAPI.h>
#include <shared/colorTable.h>

/*
================
CG_AdjustFrom640

Adjusted for resolution and screen aspect ratio
================
*/
void CG_AdjustFrom640( float* x, float* y, float* w, float* h )
{
#if 0
	// adjust for wide screens
	if ( cgs.glconfig.vidWidth * 480 > cgs.glconfig.vidHeight * 640 )
	{
		*x += 0.5 * ( cgs.glconfig.vidWidth - ( cgs.glconfig.vidHeight * 640 / 480 ) );
	}
#endif
	// scale for screen sizes
	*x *= cgs.screenXScale;
	*y *= cgs.screenYScale;
	*w *= cgs.screenXScale;
	*h *= cgs.screenYScale;
}

/*
================
CG_DrawPic

Coordinates are 640*480 virtual values
=================
*/
void CG_DrawPic( float x, float y, float width, float height, class mtrAPI_i* hShader )
{
	CG_AdjustFrom640( &x, &y, &width, &height );
	rf->drawStretchPic( x, y, width, height, 0, 0, 1, 1, hShader );
}



/*
===============
CG_DrawChar

Coordinates and size in 640*480 virtual screen size
===============
*/
void CG_DrawChar( int x, int y, int width, int height, int ch )
{
	int row, col;
	float frow, fcol;
	float size;
	float   ax, ay, aw, ah;
	
	ch &= 255;
	
	if ( ch == ' ' )
	{
		return;
	}
	
	ax = x;
	ay = y;
	aw = width;
	ah = height;
	CG_AdjustFrom640( &ax, &ay, &aw, &ah );
	
	row = ch >> 4;
	col = ch & 15;
	
	frow = row * 0.0625;
	fcol = col * 0.0625;
	size = 0.0625;
	
	rf->drawStretchPic( ax, ay, aw, ah,
						fcol, frow,
						fcol + size, frow + size,
						cgs.media.charsetShader );
}


/*
==================
CG_DrawStringExt

Draws a multi-colored string with a drop shadow, optionally forcing
to a fixed color.

Coordinates are at 640 by 480 virtual resolution
==================
*/
void CG_DrawStringExt( int x, int y, const char* string, const float* setColor,
					   bool forceColor, bool shadow, int charWidth, int charHeight, int maxChars )
{
	vec4_t      color;
	const char* s;
	int         xx;
	int         cnt;
	
	if ( maxChars <= 0 )
		maxChars = 32767; // do them all!
		
	// draw the drop shadow
	if ( shadow )
	{
		color[0] = color[1] = color[2] = 0;
		color[3] = setColor[3];
		rf->set2DColor( color );
		s = string;
		xx = x;
		cnt = 0;
		while ( *s && cnt < maxChars )
		{
			if ( Q_IsColorString( s ) )
			{
				s += 2;
				continue;
			}
			CG_DrawChar( xx + 2, y + 2, charWidth, charHeight, *s );
			cnt++;
			xx += charWidth;
			s++;
		}
	}
	
	// draw the colored text
	s = string;
	xx = x;
	cnt = 0;
	rf->set2DColor( setColor );
	while ( *s && cnt < maxChars )
	{
		if ( Q_IsColorString( s ) )
		{
			if ( !forceColor )
			{
				memcpy( color, g_color_table[ColorIndex( *( s + 1 ) )], sizeof( color ) );
				color[3] = setColor[3];
				rf->set2DColor( color );
			}
			s += 2;
			continue;
		}
		CG_DrawChar( xx, y, charWidth, charHeight, *s );
		xx += charWidth;
		cnt++;
		s++;
	}
	rf->set2DColor( NULL );
}

void CG_DrawBigString( int x, int y, const char* s, float alpha )
{
	float   color[4];
	
	color[0] = color[1] = color[2] = 1.0;
	color[3] = alpha;
	CG_DrawStringExt( x, y, s, color, false, true, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0 );
}

void CG_DrawSmallString( int x, int y, const char* s, float alpha )
{
	float color[4];
	color[0] = color[1] = color[2] = 1.0;
	color[3] = alpha;
	CG_DrawStringExt( x, y, s, color, false, false, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0 );
}

void CG_DrawSmallStringColor( int x, int y, const char* s, float color[4] )
{
	CG_DrawStringExt( x, y, s, color, false, false, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0 );
}

/*
=================
CG_DrawStrlen

Returns character count, skiping color escape codes
=================
*/
int CG_DrawStrlen( const char* str )
{
	const char* s = str;
	int count = 0;
	
	while ( *s )
	{
		if ( Q_IsColorString( s ) )
		{
			s += 2;
		}
		else
		{
			count++;
			s++;
		}
	}
	
	return count;
}

