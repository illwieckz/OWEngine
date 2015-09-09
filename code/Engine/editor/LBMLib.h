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
//  File name:   LBMLib.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

void LoadLBM( char* filename, byte** picture, byte** palette );
void WriteLBMfile( char* filename, byte* data, int width, int height
                   , byte* palette );
void LoadPCX( char* filename, byte** picture, byte** palette, int* width, int* height );
void WritePCXfile( char* filename, byte* data, int width, int height
                   , byte* palette );

// loads / saves either lbm or pcx, depending on extension
void Load256Image( char* name, byte** pixels, byte** palette,
                   int* width, int* height );
void Save256Image( char* name, byte* pixels, byte* palette,
                   int width, int height );


void LoadTGA( char* filename, byte** pixels, int* width, int* height );
void LoadImage( const char* name, byte** pic, int* width, int* height );
