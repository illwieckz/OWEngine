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
//  File name:   Textures.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

// a texturename of the form (0 0 0) will
// create a solid color texture

void    Texture_Init( bool bHardInit = true );
void    Texture_FlushUnused();
void    Texture_Flush( bool bReload = false );
void    Texture_ClearInuse( void );
void    Texture_ShowInuse( void );
void    Texture_ShowDirectory( int menunum, bool bLinked = false );
void    Texture_ShowAll();
void Texture_Cleanup( CStringList* pList = NULL );

// TTimo: added bNoAlpha flag to ignore alpha channel when parsing a .TGA file, transparency is usually achieved through qer_trans keyword in shaders
// in some cases loading an empty alpha channel causes display bugs (brushes not seen)
qtexture_t* Texture_ForName( const char* name, bool bReplace = false, bool bShader = false, bool bNoAlpha = false, bool bReload = false, bool makeShader = true );

void    Texture_Init( void );
// Timo
// added an optional IPluginTexdef when one is available
// we need a forward declaration, this is crap
class IPluginTexdef;
void    Texture_SetTexture( texdef_t* texdef, brushprimit_texdef_s* brushprimit_texdef, bool bFitScale = false, bool bSetSelection = true );

void    Texture_SetMode( int iMenu );   // GL_TEXTURE_NEAREST, etc..
void Texture_ResetPosition();

void FreeShaders();
void LoadShaders();
void ReloadShaders();
int  WINAPI Texture_LoadSkin( char* pName, int* pnWidth, int* pnHeight );
void Texture_LoadFromPlugIn( LPVOID vp );
void Texture_StartPos( void );
qtexture_t* Texture_NextPos( int* x, int* y );
