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
//  File name:   rf_2d.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: 2D graphics batching and drawing
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __RF_2D_H__
#define __RF_2D_H__

#include <math/vec2.h>
#include <shared/array.h>
#include <shared/r2dVert.h>

// tess2d_c is used to batching 2D graphics.
// Multiple images using the same color and material
// (eg. font characters) can be merged into a single
// draw call.
class tess2d_c
{
		arraySTD_c<r2dVert_s> verts;
		arraySTD_c<u16> indices;
		u32 numVerts; // currently used
		u32 numIndexes; // currently used
		class mtrAPI_i* material;
		float curColor[4];
	public:
		tess2d_c();
		void finishDrawing();
		void set2DColor( const float* rgba );   // NULL = 1,1,1,1
		void ensureAlloced( u32 numNeedIndexes, u32 numNeedVerts );
		void drawStretchPic( float x, float y, float w, float h,
							 float s1, float t1, float s2, float t2, class mtrAPI_i* material ); // NULL = white
};

class r2dCommandsQueue_c
{
		arraySTD_c<byte> data;
		u32 at;
		void ensureAlloced( u32 neededSize );
	public:
		r2dCommandsQueue_c();
		void addSetColorCmd( const float* rgba );   // NULL = 1,1,1,1
		void addDrawStretchPic( float x, float y, float w, float h,
								float s1, float t1, float s2, float t2, class mtrAPI_i* material ); // NULL = white
		void executeCommands();
		void clear();
};

extern r2dCommandsQueue_c r_2dCmds;

#endif // __RF_2D_H__
