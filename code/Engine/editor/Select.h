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
//  File name:   Select.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

typedef enum
{
	sel_brush,
	// sel_sticky_brush,
	// sel_face,
	sel_vertex,
	sel_edge,
	sel_singlevertex,
	sel_curvepoint,
	sel_area,
	
	
} select_t;

typedef struct
{
	brush_s*        brush;
	face_s*     face;
	float       dist;
	bool    selected;
} trace_t;


#define SF_SELECTED_ONLY     0x01
#define SF_ENTITIES_FIRST    0x02
#define SF_SINGLEFACE          0x04
#define SF_IGNORECURVES    0x08
#define SF_IGNOREGROUPS    0x10
#define SF_CYCLE           0x20
#define SF_CYCLEKEEP       0x40


trace_t Test_Ray( vec3_t origin, vec3_t dir, int flags );

void Select_GetBounds( edVec3_c& mins, edVec3_c& maxs );
void Select_Brush( brush_s* b, bool bComplete = true, bool bStatus = true );
void Select_Ray( vec3_t origin, vec3_t dir, int flags );
void Select_Delete( void );
void Select_Deselect( bool bDeselectFaces = true );
void Select_Invert( void );
void Select_Clone( void );
void Select_Move( vec3_t delta, bool bSnap = true );
void WINAPI Select_SetTexture( texdef_t* texdef, brushprimit_texdef_s* brushprimit_texdef, bool bFitScale = false, void* pPlugTexdef = NULL );
void Select_FlipAxis( int axis );
void Select_RotateAxis( int axis, float deg, bool bPaint = true, bool bMouse = false );
void Select_CompleteTall( void );
void Select_PartialTall( void );
void Select_Touching( void );
void Select_Inside( void );
void Select_MakeStructural( void );
void Select_MakeDetail( void );
void Select_AllOfType();
void Select_Reselect();
void Select_FitTexture( int nHeight = 1, int nWidth = 1 );

// absolute texture coordinates
// TTimo NOTE: this is stuff for old brushes format and rotation texture lock .. sort of in-between with bush primitives
void ComputeAbsolute( face_s* f, edVec3_c& p1, edVec3_c& p2, edVec3_c& p3 );
void AbsoluteToLocal( const class edPlane_c& normal2, face_s* f, edVec3_c& p1, edVec3_c& p2, edVec3_c& p3 );
void Select_Hide();
void Select_ShowAllHidden();
void clearSelection();
