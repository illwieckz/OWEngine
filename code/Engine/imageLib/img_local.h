////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2012 V.
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
//  File name:   img_local.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IMG_LOCAL_H__
#define __IMG_LOCAL_H__

#include <shared/typedefs.h>

// img_main.cpp
void IMG_GetDefaultImage( byte** outData, u32* outW, u32* outH );

// img_devil.cpp
void IMG_InitDevil();
const char* IMG_LoadImageInternal( const char* fname, byte** imageData, u32* width, u32* height );

// img_convert.cpp
void IMG_Convert8BitImageToRGBA32( byte** converted, u32* outWidth, u32* outHeight, const byte* pixels, u32 width, u32 height, const byte* palette );

// img_utils.cpp
void IMG_HorizontalFlip( byte* data, u32 width, u32 height );
void IMG_VerticalFlip( byte* data, u32 width, u32 height ) ;
void IMG_RotatePic( byte* data, u32 width );

// img_write.cpp
bool IMG_WriteTGA( const char* fname, byte* pic, u32 width, u32 height, u32 bpp );

#endif // __IMG_LOCAL_H__

