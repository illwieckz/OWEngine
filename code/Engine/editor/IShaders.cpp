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
//  File name:   IShaders.cpp
//  Version:     v1.1.2.2
//  Created:
//  Compilers:   Visual Studio
//  Description: Implementation of the shaders / textures interface
// -------------------------------------------------------------------------
//  History:
//
//  Revision 1.1.2.1  2000/02/11 03:52:30  ttimo
//  Revision 1.1.2.2  2000/02/24 22:24:45  ttimo
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

// this is a modified version of Texture_ForName
qtexture_t* WINAPI QERApp_TryTextureForName( const char* name )
{
	qtexture_t* q;
	char filename[1024];
	for ( q = g_qeglobals.d_qtextures ; q ; q = q->next )
	{
		if ( !strcmp( name,  q->filename ) )
			return q;
	}
	// try loading from file .. this is a copy of the worst part of Texture_ForName
	char cWork[1024];
	sprintf( filename, "%s/%s.tga", ValueForKey( g_qeglobals.d_project_entity, "texturepath" ), name );
	QE_ConvertDOSToUnixName( cWork, filename );
	strcpy( filename, cWork );
	unsigned char* pPixels = NULL;
	int nWidth;
	int nHeight;
	LoadImage( filename, &pPixels, &nWidth, &nHeight );
	if ( pPixels == NULL )
	{
		// try jpg
		// blatant assumption of .tga should be fine since we sprintf'd it above
		int nLen = strlen( filename );
		filename[nLen - 3] = 'j';
		filename[nLen - 2] = 'p';
		filename[nLen - 1] = 'g';
		LoadImage( filename, &pPixels, &nWidth, &nHeight );;
		if ( pPixels == NULL )
		{
			// try png
			// blatant assumption of .tga should be fine since we sprintf'd it above
			int nLen = strlen( filename );
			filename[nLen - 3] = 'p';
			filename[nLen - 2] = 'n';
			filename[nLen - 1] = 'g';
			LoadImage( filename, &pPixels, &nWidth, &nHeight );
		}
	}
	if ( pPixels )
	{
		q = Texture_LoadTGATexture( pPixels, nWidth, nHeight, NULL, 0, 0, 0 );
		//++timo storing the filename .. will be removed by shader code cleanup
		// this is dirty, and we sure miss some places were we should fill the filename info
		strcpy( q->filename, name );
		SetNameShaderInfo( q, filename, name );
		Sys_Printf( "done.\n", name );
		free( pPixels );
		return q;
	}
	return NULL;
}
