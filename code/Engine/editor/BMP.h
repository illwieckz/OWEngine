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
//  File name:   BMP.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef _BMP_H
#define _BMP_H

#define xBI_NONE  0
#define xBI_RGB   0
#define xBI_RLE4  2
#define xBI_RLE8  1

#define BMP_SIGNATURE_WORD  0x4d42

#pragma pack(1)

typedef struct
{
	unsigned short    bfType;       // signature - 'BM'
	unsigned long     bfSize;       // file size in bytes
	unsigned short    bfReserved1;  // 0
	unsigned short    bfReserved2;  // 0
	unsigned long     bfOffBits;    // offset to bitmap
} bmphd_t;



typedef struct
{
	unsigned long     biSize;       // size of this struct
	long              biWidth;      // bmap width in pixels
	long              biHeight;     // bmap height in pixels
	unsigned short    biPlanes;     // num planes - always 1
	unsigned short    biBitCount;   // bits perpixel
	unsigned long     biCompression; // compression flag
	unsigned long     biSizeImage;   // image size in bytes
	long              biXPelsPerMeter; // horz resolution
	long              biYPelsPerMeter; // vert resolution
	unsigned long     biClrUsed;       // 0 -> color table size
	unsigned long     biClrImportant;  // important color count
} binfo_t;


typedef struct
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;
} drgb_t;


// quake expects its palette to be bgr
// this is totally backwards but what can you do
typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb_t;


typedef struct
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
} bgr_t;


typedef struct
{
	int            bpp;        // bits per pixel
	int            width;
	int            height;
	unsigned char* data;
	rgb_t*         palette;
} bitmap_t;


void  LoadBMP( char* filename, bitmap_t* bit );
void FreeBMP( bitmap_t* bitmap );
void WriteBMP( char* filename, bitmap_t* bit );
void NewBMP( int width, int height, int bpp, bitmap_t* bit );



#endif
