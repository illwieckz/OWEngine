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
//  File name:   Select.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

// select.c
#include "stdafx.h"
#include "qe3.h"


// externs
CPtrArray g_SelectedFaces;
CPtrArray g_SelectedFaceBrushes;
CPtrArray& g_ptrSelectedFaces = g_SelectedFaces;
CPtrArray& g_ptrSelectedFaceBrushes = g_SelectedFaceBrushes;


void clearSelection()
{
	g_qeglobals.d_select_mode = sel_brush;
}

/*
===========
Test_Ray
===========
*/
#define DIST_START  999999
trace_t Test_Ray( vec3_t origin, vec3_t dir, int flags )
{
	brush_s*    brush;
	face_s* face;
	float   dist;
	trace_t t;
	
	memset( &t, 0, sizeof( t ) );
	t.dist = DIST_START;
	
	if ( flags & SF_CYCLE )
	{
		CPtrArray array;
		brush_s* pToSelect = ( selected_brushes.next != &selected_brushes ) ? selected_brushes.next : NULL;
		Select_Deselect();
		
		// go through active brushes and accumulate all "hit" brushes
		for ( brush = active_brushes.next ; brush != &active_brushes ; brush = brush->next )
		{
			//if ( (flags & SF_ENTITIES_FIRST) && brush->owner == world_entity)
			//  continue;
			
			if ( FilterBrush( brush ) )
				continue;
				
			if ( !g_PrefsDlg.m_bSelectCurves && brush->patchBrush )
				continue;
				
			if ( !g_PrefsDlg.m_bSelectTerrain && brush->terrainBrush )
				continue;
				
			//if (!g_bShowPatchBounds && brush->patchBrush)
			//  continue;
			
			face = Brush_Ray( origin, dir, brush, &dist );
			
			if ( face )
			{
				array.Add( brush );
			}
		}
		
		int nSize = array.GetSize();
		if ( nSize > 0 )
		{
			bool bFound = false;
			for ( int i = 0; i < nSize; i++ )
			{
				brush_s* b = reinterpret_cast<brush_s*>( array.GetAt( i ) );
				// did we hit the last one selected yet ?
				if ( b == pToSelect )
				{
					// yes we want to select the next one in the list
					int n = ( i > 0 ) ? i - 1 : nSize - 1;
					pToSelect = reinterpret_cast<brush_s*>( array.GetAt( n ) );
					bFound = true;
					break;
				}
			}
			if ( !bFound )
				pToSelect = reinterpret_cast<brush_s*>( array.GetAt( 0 ) );
		}
		if ( pToSelect )
		{
			face = Brush_Ray( origin, dir, pToSelect, &dist );
			
			t.dist = dist;
			t.brush = pToSelect;
			t.face = face;
			t.selected = false;
			return t;
		}
	}
	
	if ( !( flags & SF_SELECTED_ONLY ) )
	{
		for ( brush = active_brushes.next ; brush != &active_brushes ; brush = brush->next )
		{
			if ( ( flags & SF_ENTITIES_FIRST ) && brush->owner == world_entity )
				continue;
				
			if ( FilterBrush( brush ) )
				continue;
				
			if ( !g_PrefsDlg.m_bSelectCurves && brush->patchBrush )
				continue;
				
			if ( !g_PrefsDlg.m_bSelectTerrain && brush->terrainBrush )
				continue;
				
			//if (!g_bShowPatchBounds && brush->patchBrush)
			//  continue;
			
			face = Brush_Ray( origin, dir, brush, &dist );
			if ( dist > 0 && dist < t.dist )
			{
				t.dist = dist;
				t.brush = brush;
				t.face = face;
				t.selected = false;
			}
		}
	}
	
	
	for ( brush = selected_brushes.next ; brush != 0 && brush != &selected_brushes ; brush = brush->next )
	{
		if ( ( flags & SF_ENTITIES_FIRST ) && brush->owner == world_entity )
			continue;
			
		if ( FilterBrush( brush ) )
			continue;
			
		if ( !g_PrefsDlg.m_bSelectCurves && brush->patchBrush )
			continue;
			
		if ( !g_PrefsDlg.m_bSelectTerrain && brush->terrainBrush )
			continue;
			
		face = Brush_Ray( origin, dir, brush, &dist );
		if ( dist > 0 && dist < t.dist )
		{
			t.dist = dist;
			t.brush = brush;
			t.face = face;
			t.selected = true;
		}
	}
	
	// if entites first, but didn't find any, check regular
	
	if ( ( flags & SF_ENTITIES_FIRST ) && t.brush == NULL )
		return Test_Ray( origin, dir, flags - SF_ENTITIES_FIRST );
		
	return t;
	
}


/*
============
Select_Brush

============
*/
void Select_Brush( brush_s* brush, bool bComplete, bool bStatus )
{
	brush_s*    b;
	entity_s*   e;
	
	g_ptrSelectedFaces.RemoveAll();
	g_ptrSelectedFaceBrushes.RemoveAll();
	//selected_face = NULL;
	if ( g_qeglobals.d_select_count < 2 )
		g_qeglobals.d_select_order[g_qeglobals.d_select_count] = brush;
	g_qeglobals.d_select_count++;
	
	//if (brush->patchBrush)
	//  Patch_Select(brush->nPatchID);
	
	e = brush->owner;
	if ( e )
	{
		// select complete entity on first click
		if ( e != world_entity && bComplete == true )
		{
			for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
				if ( b->owner == e )
					goto singleselect;
			for ( b = e->brushes.onext ; b != &e->brushes ; b = b->onext )
			{
				Brush_RemoveFromList( b );
				Brush_AddToList( b, &selected_brushes );
			}
		}
		else
		{
singleselect:
			Brush_RemoveFromList( brush );
			Brush_AddToList( brush, &selected_brushes );
			UpdateSurfaceDialog();
			UpdatePatchInspector();
		}
		
		if ( e->eclass )
		{
			UpdateEntitySel( brush->owner->eclass );
		}
	}
	if ( bStatus )
	{
		edVec3_c vMin, vMax, vSize;
		Select_GetBounds( vMin, vMax );
		vSize = vMax - vMin;
		CString strStatus;
		strStatus.Format( "Selection X:: %.1f  Y:: %.1f  Z:: %.1f", vSize[0], vSize[1], vSize[2] );
		g_pParentWnd->SetStatusText( 2, strStatus );
	}
}


/*
============
Select_Ray

If the origin is inside a brush, that brush will be ignored.
============
*/
void Select_Ray( vec3_t origin, vec3_t dir, int flags )
{
	trace_t t;
	
	t = Test_Ray( origin, dir, flags );
	if ( !t.brush )
		return;
		
	if ( flags == SF_SINGLEFACE )
	{
		int nCount = g_SelectedFaces.GetSize();
		bool bOk = true;
		for ( int i = 0; i < nCount; i++ )
		{
			if ( t.face == reinterpret_cast<face_s*>( g_SelectedFaces.GetAt( i ) ) )
			{
				bOk = false;
				// need to move remove i'th entry
				g_SelectedFaces.RemoveAt( i, 1 );
				g_SelectedFaceBrushes.RemoveAt( i, 1 );
			}
		}
		if ( bOk )
		{
			g_SelectedFaces.Add( t.face );
			g_SelectedFaceBrushes.Add( t.brush );
		}
		//selected_face = t.face;
		//selected_face_brush = t.brush;
		Sys_UpdateWindows( W_ALL );
		clearSelection();
		// Texture_SetTexture requires a brushprimit_texdef fitted to the default width=2 height=2 texture
		brushprimit_texdef_s brushprimit_texdef;
		ConvertTexMatWithQTexture( &t.face->brushprimit_texdef, t.face->d_texture, &brushprimit_texdef, NULL );
		Texture_SetTexture( &t.face->texdef, &brushprimit_texdef, false, false );
		UpdateSurfaceDialog();
		return;
	}
	
	// move the brush to the other list
	
	clearSelection();
	
	if ( t.selected )
	{
		Brush_RemoveFromList( t.brush );
		Brush_AddToList( t.brush, &active_brushes );
		UpdatePatchInspector();
	}
	else
	{
		Select_Brush( t.brush, !( GetKeyState( VK_MENU ) & 0x8000 ) );
	}
	
	Sys_UpdateWindows( W_ALL );
}


void Select_Delete( void )
{
	brush_s*    brush;
	
	g_ptrSelectedFaces.RemoveAll();
	g_ptrSelectedFaceBrushes.RemoveAll();
	//selected_face = NULL;
	
	clearSelection();
	
	g_qeglobals.d_select_count = 0;
	g_qeglobals.d_num_move_points = 0;
	while ( selected_brushes.next != &selected_brushes )
	{
		brush = selected_brushes.next;
		if ( brush->patchBrush )
		{
			//Patch_Delete(brush->nPatchID);
			Patch_Delete( brush->pPatch );
		}
		
		Brush_Free( brush );
	}
	
	// FIXME: remove any entities with no brushes
	
	Sys_UpdateWindows( W_ALL );
}

void Select_Deselect( bool bDeselectFaces )
{
	brush_s*    b;
	
	Patch_Deselect();
	
	g_pParentWnd->ActiveXY()->UndoClear();
	
	g_qeglobals.d_workcount++;
	g_qeglobals.d_select_count = 0;
	g_qeglobals.d_num_move_points = 0;
	b = selected_brushes.next;
	
	if ( b == &selected_brushes || b == 0 )
	{
		if ( bDeselectFaces )
		{
			g_ptrSelectedFaces.RemoveAll();
			g_ptrSelectedFaceBrushes.RemoveAll();
			//selected_face = NULL;
		}
		Sys_UpdateWindows( W_ALL );
		return;
	}
	
	if ( bDeselectFaces )
	{
		g_ptrSelectedFaces.RemoveAll();
		g_ptrSelectedFaceBrushes.RemoveAll();
		//selected_face = NULL;
	}
	
	clearSelection();
	
	// grab top / bottom height for new brushes
	if ( b->getMins()[2] < b->getMaxs()[2] )
	{
		g_qeglobals.d_new_brush_bottom_z = b->getMins()[2];
		g_qeglobals.d_new_brush_top_z = b->getMaxs()[2];
	}
	
	selected_brushes.next->prev = &active_brushes;
	selected_brushes.prev->next = active_brushes.next;
	active_brushes.next->prev = selected_brushes.prev;
	active_brushes.next = selected_brushes.next;
	selected_brushes.prev = selected_brushes.next = &selected_brushes;
	
	Sys_UpdateWindows( W_ALL );
}

/*
============
Select_Move
============
*/
void Select_Move( vec3_t delta, bool bSnap )
{
	brush_s*    b;
	
	
	// actually move the selected brushes
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
		Brush_Move( b, delta, bSnap );
		
	edVec3_c vMin, vMax;
	Select_GetBounds( vMin, vMax );
	CString strStatus;
	strStatus.Format( "Origin X:: %.1f  Y:: %.1f  Z:: %.1f", vMin[0], vMax[1], vMax[2] );
	g_pParentWnd->SetStatusText( 2, strStatus );
	
	//  Sys_UpdateWindows (W_ALL);
}

/*
============
Select_Clone

Creates an exact duplicate of the selection in place, then moves
the selected brushes off of their old positions
============
*/
void Select_Clone( void )
{
#if 1
	ASSERT( g_pParentWnd->ActiveXY() );
	g_bScreenUpdates = false;
	g_pParentWnd->ActiveXY()->Copy();
	g_pParentWnd->ActiveXY()->Paste();
	g_pParentWnd->NudgeSelection( 2, g_qeglobals.d_gridsize );
	g_pParentWnd->NudgeSelection( 3, g_qeglobals.d_gridsize );
	g_bScreenUpdates = true;
	Sys_UpdateWindows( W_ALL );
#else
	
	brush_s*        b, *b2, *n, *next, *next2;
	vec3_t      delta;
	entity_s*   e;
	
	g_qeglobals.d_workcount++;
	clearSelection();
	
	delta[0] = g_qeglobals.d_gridsize;
	delta[1] = g_qeglobals.d_gridsize;
	delta[2] = 0;
	
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = next )
	{
		next = b->next;
		// if the brush is a world brush, handle simply
		if ( b->owner == world_entity )
		{
			n = Brush_Clone( b );
			Brush_AddToList( n, &active_brushes );
			Entity_LinkBrush( world_entity, n );
			Brush_Build( n );
			Brush_Move( b, delta );
			continue;
		}
	
		e = Entity_Clone( b->owner );
		// clear the target / targetname
		DeleteKey( e, "target" );
		DeleteKey( e, "targetname" );
	
		// if the brush is a fixed size entity, create a new entity
		if ( b->owner->eclass->fixedsize )
		{
			n = Brush_Clone( b );
			Brush_AddToList( n, &active_brushes );
			Entity_LinkBrush( e, n );
			Brush_Build( n );
			Brush_Move( b, delta );
			continue;
		}
	
		// brush is a complex entity, grab all the other ones now
	
		next = &selected_brushes;
	
		for ( b2 = b ; b2 != &selected_brushes ; b2 = next2 )
		{
			next2 = b2->next;
			if ( b2->owner != b->owner )
			{
				if ( next == &selected_brushes )
					next = b2;
				continue;
			}
	
			// move b2 to the start of selected_brushes,
			// so it won't be hit again
			Brush_RemoveFromList( b2 );
			Brush_AddToList( b2, &selected_brushes );
	
			n = Brush_Clone( b2 );
			Brush_AddToList( n, &active_brushes );
			Entity_LinkBrush( e, n );
			Brush_Build( n );
			Brush_Move( b2, delta, true );
		}
	
	}
	Sys_UpdateWindows( W_ALL );
#endif
}



/*
============
Select_SetTexture
Timo : bFitScale to compute scale on the plane and counteract plane / axial plane snapping
Timo :  brush primitive texturing
        the brushprimit_texdef given must be understood as a qtexture_t width=2 height=2 ( HiRes )
Timo :  texture plugin, added an IPluginTexdef* parameter
        must be casted to an IPluginTexdef!
        if not NULL, get ->Copy() of it into each face or brush ( and remember to hook )
        if NULL, means we have no information, ask for a default
============
*/
void WINAPI Select_SetTexture( texdef_t* texdef, brushprimit_texdef_s* brushprimit_texdef, bool bFitScale, void* pPlugTexdef )
{
	brush_s*    b;
	int nCount = g_ptrSelectedFaces.GetSize();
	if ( nCount > 0 )
	{
		Undo_Start( "set face textures" );
		ASSERT( g_ptrSelectedFaces.GetSize() == g_ptrSelectedFaceBrushes.GetSize() );
		for ( int i = 0; i < nCount; i++ )
		{
			face_s* selFace = reinterpret_cast<face_s*>( g_ptrSelectedFaces.GetAt( i ) );
			brush_s* selBrush = reinterpret_cast<brush_s*>( g_ptrSelectedFaceBrushes.GetAt( i ) );
			Undo_AddBrush( selBrush );
			SetFaceTexdef( selBrush, selFace, texdef, brushprimit_texdef, bFitScale, static_cast<IPluginTexdef*>( pPlugTexdef ) );
			Brush_Build( selBrush, bFitScale );
			Undo_EndBrush( selBrush );
		}
		Undo_End();
	}
	else if ( selected_brushes.next != &selected_brushes )
	{
		Undo_Start( "set brush textures" );
		for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
			if ( !b->owner->eclass->fixedsize )
			{
				Undo_AddBrush( b );
				Brush_SetTexture( b, texdef, brushprimit_texdef, bFitScale, static_cast<IPluginTexdef*>( pPlugTexdef ) );
				Undo_EndBrush( b );
			}
		Undo_End();
	}
	Sys_UpdateWindows( W_ALL );
}


/*
================================================================

  TRANSFORMATIONS

================================================================
*/

void Select_GetBounds( edVec3_c& mins, edVec3_c& maxs )
{
	brush_s*    b;
	int     i;
	
	for ( i = 0 ; i < 3 ; i++ )
	{
		mins[i] = 99999;
		maxs[i] = -99999;
	}
	
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
		for ( i = 0 ; i < 3 ; i++ )
		{
			if ( b->getMins()[i] < mins[i] )
				mins[i] = b->getMins()[i];
			if ( b->getMaxs()[i] > maxs[i] )
				maxs[i] = b->getMaxs()[i];
		}
}


void Select_GetTrueMid( vec3_t mid )
{
	edVec3_c    mins, maxs;
	Select_GetBounds( mins, maxs );
	
	for ( int i = 0 ; i < 3 ; i++ )
		mid[i] = ( mins[i] + ( ( maxs[i] - mins[i] ) / 2 ) );
}


void Select_GetMid( vec3_t mid )
{
	edVec3_c    mins, maxs;
	int     i;
	
	if ( g_PrefsDlg.m_bNoClamp )
	{
		Select_GetTrueMid( mid );
		return;
	}
	
	Select_GetBounds( mins, maxs );
	
	for ( i = 0 ; i < 3 ; i++ )
		mid[i] = g_qeglobals.d_gridsize * floor( ( ( mins[i] + maxs[i] ) * 0.5 ) / g_qeglobals.d_gridsize );
		
}

edVec3_c    select_origin;
edVec3_c    select_matrix[3];
bool    select_fliporder;

void Select_ApplyMatrix( bool bSnap, bool bRotation, int nAxis, float fDeg )
{
	brush_s*    b;
	face_s* f;
	int     i, j;
	edVec3_c    temp;
	
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
	{
		for ( f = b->brush_faces ; f ; f = f->next )
		{
			for ( i = 0 ; i < 3 ; i++ )
			{
				temp = f->planepts[i] - select_origin;
				for ( j = 0 ; j < 3 ; j++ )
					f->planepts[i][j] = temp.dotProduct( select_matrix[j] ) + select_origin[j];
			}
			if ( select_fliporder )
			{
				// swap the points
				temp = f->planepts[0];
				f->planepts[0] = f->planepts[2];
				f->planepts[2] = temp;
			}
		}
		
		//if(b->owner->eclass->fixedsize)
		//{
		//  if (bRotation && b->owner->md3Class)
		//  {
		//      b->owner->vRotation[nAxis] += fDeg;
		//  }
		//}
		
		Brush_Build( b, bSnap );
		
		if ( b->patchBrush )
		{
			//Patch_ApplyMatrix(b->nPatchID, select_origin, select_matrix);
			Patch_ApplyMatrix( b->pPatch, select_origin, select_matrix, bSnap );
		}
		
		//if (b->terrainBrush)
		//{
		//  Terrain_ApplyMatrix(b->pTerrain, select_origin, select_matrix, bSnap);
		//}
		
	}
}

void Back( const edVec3_c& dir, edVec3_c& p )
{
	if ( fabs( dir[0] ) == 1 )
		p[0] = 0;
	else if ( fabs( dir[1] ) == 1 )
		p[1] = 0;
	else
		p[2] = 0;
}



// using scale[0] and scale[1]
void ComputeScale( const edVec3_c& rex, const edVec3_c& rey, edVec3_c& p, const face_s* f )
{
	float px = rex.dotProduct( p );
	float py = rey.dotProduct( p );
	px *= f->texdef.scale[0];
	py *= f->texdef.scale[1];
	edVec3_c aux = rex;
	aux *= px;
	p = aux;
	aux = rey;
	aux *= py;
	p += aux;
}

void ComputeAbsolute( face_s* f, edVec3_c& p1, edVec3_c& p2, edVec3_c& p3 )
{
	edVec3_c ex, ey, ez;          // local axis base
	
#ifdef _DEBUG
	if ( g_qeglobals.m_bBrushPrimitMode )
		Sys_Printf( "Warning : illegal call of ComputeAbsolute in brush primitive mode\n" );
#endif
		
	// compute first local axis base
	TextureAxisFromPlane( f->plane, ex, ey );
	ez.crossProduct( ex, ey );
	
	edVec3_c aux = ex * -f->texdef.shift[0];
	p1 = aux;
	aux = ey * -f->texdef.shift[1];
	p1 += aux;
	p2 = p1;
	p2 += ex;
	p3 = p1;
	p3 += ey;
	aux = ez;
	aux *= -f->texdef.rotate;
	VectorRotate( p1, aux, p1 );
	VectorRotate( p2, aux, p2 );
	VectorRotate( p3, aux, p3 );
	// computing rotated local axis base
	edVec3_c rex = ex;
	VectorRotate( rex, aux, rex );
	edVec3_c rey = ey;
	VectorRotate( rey, aux, rey );
	
	ComputeScale( rex, rey, p1, f );
	ComputeScale( rex, rey, p2, f );
	ComputeScale( rex, rey, p3, f );
	
	// project on normal plane
	// along ez
	// assumes plane normal is normalized
	f->plane.projectOnPlane( ez, p1 );
	f->plane.projectOnPlane( ez, p2 );
	f->plane.projectOnPlane( ez, p3 );
};


void AbsoluteToLocal( const class edPlane_c& normal2, face_s* f, edVec3_c& p1, edVec3_c& p2, edVec3_c& p3 )
{
	edVec3_c ex, ey, ez;
	
#ifdef _DEBUG
	if ( g_qeglobals.m_bBrushPrimitMode )
		Sys_Printf( "Warning : illegal call of AbsoluteToLocal in brush primitive mode\n" );
#endif
		
	// computing new local axis base
	TextureAxisFromPlane( normal2, ex, ey );
	ez.crossProduct( ex, ey );
	
	// projecting back on (ex,ey)
	Back( ez, p1 );
	Back( ez, p2 );
	Back( ez, p3 );
	
	edVec3_c aux = p2 - p1;
	
	float x = aux.dotProduct( ex );
	float y = aux.dotProduct( ey );
	f->texdef.rotate = RAD2DEG( atan2( y, x ) );
	
	// computing rotated local axis base
	aux = ez;
	aux *= f->texdef.rotate;
	edVec3_c rex = ex;
	VectorRotate( rex, aux, rex );
	edVec3_c rey = ey;
	VectorRotate( rey, aux, rey );
	
	// scale
	aux = p2 - p1;
	f->texdef.scale[0] = aux.dotProduct( rex );
	aux = p3 - p1;
	f->texdef.scale[1] = aux.dotProduct( rey );
	
	// shift
	// only using p1
	x = rex.dotProduct( p1 );
	y = rey.dotProduct( p1 );
	x /= f->texdef.scale[0];
	y /= f->texdef.scale[1];
	
	p1 = rex * x;
	aux = rey * y;
	aux *= y;
	p1 += aux;
	aux = ez * -f->texdef.rotate;
	VectorRotate( p1, aux, p1 );
	f->texdef.shift[0] = -p1.dotProduct( ex );
	f->texdef.shift[1] = -p1.dotProduct( ey );
	
	// stored rot is good considering local axis base
	// change it if necessary
	f->texdef.rotate = -f->texdef.rotate;
	
	Clamp( f->texdef.shift[0], f->d_texture->width );
	Clamp( f->texdef.shift[1], f->d_texture->height );
	Clamp( f->texdef.rotate, 360 );
	
}

void RotateFaceTexture( face_s* f, int nAxis, float fDeg )
{
	edVec3_c p1, p2, p3, rota;
	p1.clear();
	p2.clear();
	p3.clear();
	rota.clear();
	ComputeAbsolute( f, p1, p2, p3 );
	
	rota[nAxis] = fDeg;
	VectorRotate( p1, rota, select_origin, p1 );
	VectorRotate( p2, rota, select_origin, p2 );
	VectorRotate( p3, rota, select_origin, p3 );
	
	edPlane_c normal2;
	edVec3_c vNormal;
	vNormal[0] = f->plane.normal[0];
	vNormal[1] = f->plane.normal[1];
	vNormal[2] = f->plane.normal[2];
	VectorRotate( vNormal, rota, vNormal );
	normal2.normal[0] = vNormal[0];
	normal2.normal[1] = vNormal[1];
	normal2.normal[2] = vNormal[2];
	AbsoluteToLocal( normal2, f, p1, p2 , p3 );
	
}

void RotateTextures( int nAxis, float fDeg, vec3_t vOrigin )
{
	for ( brush_s* b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
	{
		for ( face_s* f = b->brush_faces ; f ; f = f->next )
		{
			if ( g_qeglobals.m_bBrushPrimitMode )
				RotateFaceTexture_BrushPrimit( f, nAxis, fDeg, vOrigin );
			else
				RotateFaceTexture( f, nAxis, fDeg );
			//++timo removed that call .. works fine .. ???????
			//          Brush_Build(b, false);
		}
		Brush_Build( b, false );
	}
}


void Select_FlipAxis( int axis )
{
	int     i;
	
	Select_GetMid( select_origin );
	for ( i = 0 ; i < 3 ; i++ )
	{
		select_matrix[i].clear();
		select_matrix[i][i] = 1;
	}
	select_matrix[axis][axis] = -1;
	
	select_fliporder = true;
	Select_ApplyMatrix( true, false, 0, 0 );
	Sys_UpdateWindows( W_ALL );
}


void Select_Scale( float x, float y, float z )
{
	Select_GetMid( select_origin );
	for ( brush_s* b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
	{
		for ( face_s* f = b->brush_faces ; f ; f = f->next )
		{
			for ( int i = 0 ; i < 3 ; i++ )
			{
				f->planepts[i][0] -= select_origin[0];
				f->planepts[i][1] -= select_origin[1];
				f->planepts[i][2] -= select_origin[2];
				f->planepts[i][0] *= x;
				//f->planepts[i][0] = floor(f->planepts[i][0] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;
				
				f->planepts[i][1] *= y;
				//f->planepts[i][1] = floor(f->planepts[i][1] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;
				
				f->planepts[i][2] *= z;
				//f->planepts[i][2] = floor(f->planepts[i][2] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;
				
				f->planepts[i][0] += select_origin[0];
				f->planepts[i][1] += select_origin[1];
				f->planepts[i][2] += select_origin[2];
			}
		}
		Brush_Build( b, false );
		if ( b->patchBrush )
		{
			vec3_t v;
			v[0] = x;
			v[1] = y;
			v[2] = z;
			//Patch_Scale(b->nPatchID, select_origin, v);
			Patch_Scale( b->pPatch, select_origin, v );
		}
	}
}

void Select_RotateAxis( int axis, float deg, bool bPaint, bool bMouse )
{
	edVec3_c    temp;
	int     i, j;
	vec_t   c, s;
	
	if ( deg == 0 )
	{
		//Sys_Printf("0 deg\n");
		return;
	}
	
	if ( bMouse )
	{
		select_origin = g_pParentWnd->ActiveXY()->RotateOrigin();
	}
	else
	{
		Select_GetMid( select_origin );
	}
	
	select_fliporder = false;
	
	if ( deg == 90 )
	{
		for ( i = 0 ; i < 3 ; i++ )
		{
			select_matrix[i].clear();
			select_matrix[i][i] = 1;
		}
		i = ( axis + 1 ) % 3;
		j = ( axis + 2 ) % 3;
		temp = select_matrix[i];
		select_matrix[i] = select_matrix[j];
		select_matrix[j] = -temp;
	}
	else
	{
		deg = -deg;
		if ( deg == -180.0 )
		{
			c = -1;
			s = 0;
		}
		else if ( deg == -270.0 )
		{
			c = 0;
			s = -1;
		}
		else
		{
			c = cos( DEG2RAD( deg ) );
			s = sin( DEG2RAD( deg ) );
		}
		
		for ( i = 0 ; i < 3 ; i++ )
		{
			select_matrix[i].clear();
			select_matrix[i][i] = 1;
		}
		
		switch ( axis )
		{
			case 0:
				select_matrix[1][1] = c;
				select_matrix[1][2] = -s;
				select_matrix[2][1] = s;
				select_matrix[2][2] = c;
				break;
			case 1:
				select_matrix[0][0] = c;
				select_matrix[0][2] = s;
				select_matrix[2][0] = -s;
				select_matrix[2][2] = c;
				break;
			case 2:
				select_matrix[0][0] = c;
				select_matrix[0][1] = -s;
				select_matrix[1][0] = s;
				select_matrix[1][1] = c;
				break;
		}
	}
	
	if ( g_PrefsDlg.m_bRotateLock )
		RotateTextures( axis, deg, select_origin );
	Select_ApplyMatrix( !bMouse, true, axis, deg );
	
	if ( bPaint )
		Sys_UpdateWindows( W_ALL );
}

/*
================================================================

GROUP SELECTIONS

================================================================
*/

void Select_CompleteTall( void )
{
	brush_s*    b, *next;
	//int       i;
	edVec3_c    mins, maxs;
	
	if ( !QE_SingleBrush() )
		return;
		
	clearSelection();
	
	mins = selected_brushes.next->getMins();
	maxs = selected_brushes.next->getMaxs();
	Select_Delete();
	
	int nDim1 = ( g_pParentWnd->ActiveXY()->GetViewType() == YZ ) ? 1 : 0;
	int nDim2 = ( g_pParentWnd->ActiveXY()->GetViewType() == XY ) ? 1 : 2;
	
	for ( b = active_brushes.next ; b != &active_brushes ; b = next )
	{
		next = b->next;
		
		if ( ( b->getMaxs()[nDim1] > maxs[nDim1] || b->getMins()[nDim1] < mins[nDim1] )
				|| ( b->getMaxs()[nDim2] > maxs[nDim2] || b->getMins()[nDim2] < mins[nDim2] ) )
			continue;
			
		if ( FilterBrush( b ) )
			continue;
			
		Brush_RemoveFromList( b );
		Brush_AddToList( b, &selected_brushes );
#if 0
		// old stuff
		for ( i = 0 ; i < 2 ; i++ )
			if ( b->maxs[i] > maxs[i] || b->mins[i] < mins[i] )
				break;
		if ( i == 2 )
		{
			Brush_RemoveFromList( b );
			Brush_AddToList( b, &selected_brushes );
		}
#endif
	}
	Sys_UpdateWindows( W_ALL );
}

void Select_PartialTall( void )
{
	brush_s*    b, *next;
	//int       i;
	edVec3_c    mins, maxs;
	
	if ( !QE_SingleBrush() )
		return;
		
	clearSelection();
	
	mins = selected_brushes.next->getMins();
	maxs = selected_brushes.next->getMaxs();
	Select_Delete();
	
	int nDim1 = ( g_pParentWnd->ActiveXY()->GetViewType() == YZ ) ? 1 : 0;
	int nDim2 = ( g_pParentWnd->ActiveXY()->GetViewType() == XY ) ? 1 : 2;
	
	for ( b = active_brushes.next ; b != &active_brushes ; b = next )
	{
		next = b->next;
		
		if ( ( b->getMins()[nDim1] > maxs[nDim1] || b->getMaxs()[nDim1] < mins[nDim1] )
				|| ( b->getMins()[nDim2] > maxs[nDim2] || b->getMaxs()[nDim2] < mins[nDim2] ) )
			continue;
			
		if ( FilterBrush( b ) )
			continue;
			
		Brush_RemoveFromList( b );
		Brush_AddToList( b, &selected_brushes );
		
		
#if 0
		// old stuff
		for ( i = 0 ; i < 2 ; i++ )
			if ( b->mins[i] > maxs[i] || b->maxs[i] < mins[i] )
				break;
		if ( i == 2 )
		{
			Brush_RemoveFromList( b );
			Brush_AddToList( b, &selected_brushes );
		}
#endif
	}
	Sys_UpdateWindows( W_ALL );
}

void Select_Touching( void )
{
	brush_s*    b, *next;
	int     i;
	edVec3_c    mins, maxs;
	
	if ( !QE_SingleBrush() )
		return;
		
	clearSelection();
	
	mins = selected_brushes.next->getMins();
	maxs = selected_brushes.next->getMaxs();
	
	for ( b = active_brushes.next ; b != &active_brushes ; b = next )
	{
		next = b->next;
		
		if ( FilterBrush( b ) )
			continue;
			
		for ( i = 0 ; i < 3 ; i++ )
			if ( b->getMins()[i] > maxs[i] + 1 || b->getMaxs()[i] < mins[i] - 1 )
				break;
				
		if ( i == 3 )
		{
			Brush_RemoveFromList( b );
			Brush_AddToList( b, &selected_brushes );
		}
	}
	Sys_UpdateWindows( W_ALL );
}

void Select_Inside( void )
{
	brush_s*    b, *next;
	int     i;
	edVec3_c    mins, maxs;
	
	if ( !QE_SingleBrush() )
		return;
		
	clearSelection();
	
	mins = selected_brushes.next->getMins();
	maxs = selected_brushes.next->getMaxs();
	Select_Delete();
	
	for ( b = active_brushes.next ; b != &active_brushes ; b = next )
	{
		next = b->next;
		
		if ( FilterBrush( b ) )
			continue;
			
		for ( i = 0 ; i < 3 ; i++ )
			if ( b->getMaxs()[i] > maxs[i] || b->getMins()[i] < mins[i] )
				break;
		if ( i == 3 )
		{
			Brush_RemoveFromList( b );
			Brush_AddToList( b, &selected_brushes );
		}
	}
	Sys_UpdateWindows( W_ALL );
}

/*
=============
Select_Ungroup

Turn the currently selected entity back into normal brushes
=============
*/
void Select_Ungroup( void )
{
	int numselectedgroups;
	entity_s*   e;
	brush_s*        b, *sb;
	
	numselectedgroups = 0;
	for ( sb = selected_brushes.next; sb != &selected_brushes; sb = sb->next )
	{
		e = sb->owner;
		
		if ( !e || e == world_entity || e->eclass->fixedsize )
		{
			continue;
		}
		
		for ( b = e->brushes.onext; b != &e->brushes; b = e->brushes.onext )
		{
			//Brush_RemoveFromList (b);
			//Brush_AddToList (b, &active_brushes);
			Entity_UnlinkBrush( b );
			Entity_LinkBrush( world_entity, b );
			Brush_Build( b );
			b->owner = world_entity;
		}
		Entity_Free( e );
		numselectedgroups++;
	}
	
	if ( numselectedgroups <= 0 )
	{
		Sys_Printf( "No grouped entities selected.\n" );
		return;
	}
	Sys_Printf( "Ungrouped %d entit%s.\n", numselectedgroups, ( numselectedgroups == 1 ) ? "y" : "ies" );
	Sys_UpdateWindows( W_ALL );
}


/*
====================
Select_MakeStructural
====================
*/
void Select_MakeStructural( void )
{
	brush_s*    b;
	face_s* f;
	
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
		for ( f = b->brush_faces ; f ; f = f->next )
			f->texdef.contents &= ~CONTENTS_DETAIL;
	Select_Deselect();
	Sys_UpdateWindows( W_ALL );
}

void Select_MakeDetail( void )
{
	brush_s*    b;
	face_s* f;
	
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
		for ( f = b->brush_faces ; f ; f = f->next )
			f->texdef.contents |= CONTENTS_DETAIL;
	Select_Deselect();
	Sys_UpdateWindows( W_ALL );
}

void Select_ShiftTexture( int x, int y )
{
	brush_s*        b;
	face_s*     f;
	
	int nFaceCount = g_ptrSelectedFaces.GetSize();
	
	if ( selected_brushes.next == &selected_brushes && nFaceCount == 0 )
		return;
		
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
	{
		for ( f = b->brush_faces ; f ; f = f->next )
		{
			if ( g_qeglobals.m_bBrushPrimitMode )
			{
				// use face normal to compute a true translation
				Select_ShiftTexture_BrushPrimit( f, x, y );
			}
			else
			{
				f->texdef.shift[0] += x;
				f->texdef.shift[1] += y;
			}
		}
		Brush_Build( b );
		if ( b->patchBrush )
		{
			//Patch_ShiftTexture(b->nPatchID, x, y);
			Patch_ShiftTexture( b->pPatch, x, y );
		}
	}
	
	if ( nFaceCount > 0 )
	{
		for ( int i = 0; i < nFaceCount; i++ )
		{
			face_s* selFace = reinterpret_cast<face_s*>( g_ptrSelectedFaces.GetAt( i ) );
			brush_s* selBrush = reinterpret_cast<brush_s*>( g_ptrSelectedFaceBrushes.GetAt( i ) );
			if ( g_qeglobals.m_bBrushPrimitMode )
			{
			
				// use face normal to compute a true translation
				// Select_ShiftTexture_BrushPrimit( selected_face, x, y );
				// use camera view to compute texture shift
				g_pParentWnd->GetCamera()->ShiftTexture_BrushPrimit( selFace, x, y );
			}
			else
			{
				selFace->texdef.shift[0] += x;
				selFace->texdef.shift[1] += y;
			}
			Brush_Build( selBrush );
		}
	}
	
	Sys_UpdateWindows( W_CAMERA );
}

void Select_ScaleTexture( int x, int y )
{
	brush_s*        b;
	face_s*     f;
	
	int nFaceCount = g_ptrSelectedFaces.GetSize();
	
	if ( selected_brushes.next == &selected_brushes && nFaceCount == 0 )
	{
		return;
	}
	
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
	{
		for ( f = b->brush_faces ; f ; f = f->next )
		{
			if ( g_qeglobals.m_bBrushPrimitMode )
			{
				// apply same scale as the spinner button of the surface inspector
				float   shift[2];
				float   rotate;
				float   scale[2];
				brushprimit_texdef_s bp;
				// compute normalized texture matrix
				ConvertTexMatWithQTexture( &f->brushprimit_texdef, f->d_texture, &bp, NULL );
				// compute fake shift scale rot
				TexMatToFakeTexCoords( bp.coords, shift, &rotate, scale );
				// update
				scale[0] += static_cast<float>( x ) * 0.1;
				scale[1] += static_cast<float>( y ) * 0.1;
				// compute new normalized texture matrix
				FakeTexCoordsToTexMat( shift, rotate, scale, bp.coords );
				// apply to face texture matrix
				ConvertTexMatWithQTexture( &bp, NULL, &f->brushprimit_texdef, f->d_texture );
			}
			else
			{
				f->texdef.scale[0] += x;
				f->texdef.scale[1] += y;
			}
		}
		Brush_Build( b );
		if ( b->patchBrush )
		{
			Patch_ScaleTexture( b->pPatch, x, y );
		}
	}
	
	if ( nFaceCount > 0 )
	{
		for ( int i = 0; i < nFaceCount; i++ )
		{
			face_s* selFace = reinterpret_cast<face_s*>( g_ptrSelectedFaces.GetAt( i ) );
			brush_s* selBrush = reinterpret_cast<brush_s*>( g_ptrSelectedFaceBrushes.GetAt( i ) );
			if ( g_qeglobals.m_bBrushPrimitMode )
			{
				float   shift[2];
				float   rotate;
				float   scale[2];
				brushprimit_texdef_s bp;
				ConvertTexMatWithQTexture( &selFace->brushprimit_texdef, selFace->d_texture, &bp, NULL );
				TexMatToFakeTexCoords( bp.coords, shift, &rotate, scale );
				scale[0] += static_cast<float>( x ) * 0.1;
				scale[1] += static_cast<float>( y ) * 0.1;
				FakeTexCoordsToTexMat( shift, rotate, scale, bp.coords );
				ConvertTexMatWithQTexture( &bp, NULL, &selFace->brushprimit_texdef, selFace->d_texture );
			}
			else
			{
				selFace->texdef.scale[0] += x;
				selFace->texdef.scale[1] += y;
			}
			Brush_Build( selBrush );
		}
	}
	
	Sys_UpdateWindows( W_CAMERA );
}

void Select_RotateTexture( int amt )
{
	brush_s*        b;
	face_s*     f;
	
	int nFaceCount = g_ptrSelectedFaces.GetSize();
	
	if ( selected_brushes.next == &selected_brushes && nFaceCount == 0 )
	{
		return;
	}
	
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
	{
		for ( f = b->brush_faces ; f ; f = f->next )
		{
			if ( g_qeglobals.m_bBrushPrimitMode )
			{
				// apply same scale as the spinner button of the surface inspector
				float   shift[2];
				float   rotate;
				float   scale[2];
				brushprimit_texdef_s bp;
				// compute normalized texture matrix
				ConvertTexMatWithQTexture( &f->brushprimit_texdef, f->d_texture, &bp, NULL );
				// compute fake shift scale rot
				TexMatToFakeTexCoords( bp.coords, shift, &rotate, scale );
				// update
				rotate += amt;
				// compute new normalized texture matrix
				FakeTexCoordsToTexMat( shift, rotate, scale, bp.coords );
				// apply to face texture matrix
				ConvertTexMatWithQTexture( &bp, NULL, &f->brushprimit_texdef, f->d_texture );
			}
			else
			{
				f->texdef.rotate += amt;
				f->texdef.rotate = static_cast<int>( f->texdef.rotate ) % 360;
			}
		}
		Brush_Build( b );
		if ( b->patchBrush )
		{
			//Patch_RotateTexture(b->nPatchID, amt);
			Patch_RotateTexture( b->pPatch, amt );
		}
	}
	
	if ( nFaceCount > 0 )
	{
		for ( int i = 0; i < nFaceCount; i++ )
		{
			face_s* selFace = reinterpret_cast<face_s*>( g_ptrSelectedFaces.GetAt( i ) );
			brush_s* selBrush = reinterpret_cast<brush_s*>( g_ptrSelectedFaceBrushes.GetAt( i ) );
			if ( g_qeglobals.m_bBrushPrimitMode )
			{
				float   shift[2];
				float   rotate;
				float   scale[2];
				brushprimit_texdef_s bp;
				ConvertTexMatWithQTexture( &selFace->brushprimit_texdef, selFace->d_texture, &bp, NULL );
				TexMatToFakeTexCoords( bp.coords, shift, &rotate, scale );
				rotate += amt;
				FakeTexCoordsToTexMat( shift, rotate, scale, bp.coords );
				ConvertTexMatWithQTexture( &bp, NULL, &selFace->brushprimit_texdef, selFace->d_texture );
			}
			else
			{
				selFace->texdef.rotate += amt;
				selFace->texdef.rotate = static_cast<int>( selFace->texdef.rotate ) % 360;
			}
			Brush_Build( selBrush );
		}
	}
	
	Sys_UpdateWindows( W_CAMERA );
}


void FindReplaceTextures( const char* pFind, const char* pReplace, bool bSelected, bool bForce )
{
	brush_s* pList = ( bSelected ) ? &selected_brushes : &active_brushes;
	if ( !bSelected )
	{
		Select_Deselect();
	}
	
	for ( brush_s* pBrush = pList->next ; pBrush != pList; pBrush = pBrush->next )
	{
		if ( pBrush->patchBrush )
		{
			Patch_FindReplaceTexture( pBrush, pFind, pReplace, bForce );
		}
		
		for ( face_s* pFace = pBrush->brush_faces; pFace; pFace = pFace->next )
		{
			if ( bForce || strcmpi( pFace->texdef.name, pFind ) == 0 )
			{
				pFace->d_texture = Texture_ForName( pReplace );
				//strcpy(pFace->texdef.name, pReplace);
				pFace->texdef.SetName( pReplace );
			}
		}
		Brush_Build( pBrush );
	}
	Sys_UpdateWindows( W_CAMERA );
}


void Select_AllOfType()
{
	brush_s*    b, *next;
	entity_s*   e;
	if ( ( selected_brushes.next == &selected_brushes )
			|| ( selected_brushes.next->next != &selected_brushes ) )
	{
	
		CString strName;
		if ( g_ptrSelectedFaces.GetSize() == 0 )
		{
			strName = g_qeglobals.d_texturewin.texdef.name;
		}
		else
		{
			face_s* selFace = reinterpret_cast<face_s*>( g_ptrSelectedFaces.GetAt( 0 ) );
			strName = selFace->texdef.name;
		}
		
		Select_Deselect();
		for ( b = active_brushes.next ; b != &active_brushes ; b = next )
		{
			next = b->next;
			
			if ( FilterBrush( b ) )
				continue;
				
			if ( b->patchBrush )
			{
				if ( strcmpi( strName, b->pPatch->d_texture->name ) == 0 )
				{
					Brush_RemoveFromList( b );
					Brush_AddToList( b, &selected_brushes );
				}
			}
			else
			{
				for ( face_s* pFace = b->brush_faces; pFace; pFace = pFace->next )
				{
					if ( strcmpi( strName, pFace->texdef.name ) == 0 )
					{
						Brush_RemoveFromList( b );
						Brush_AddToList( b, &selected_brushes );
					}
				}
			}
		}
		Sys_UpdateWindows( W_ALL );
		return;
	}
	
	
	b = selected_brushes.next;
	e = b->owner;
	
	if ( e != NULL )
	{
		if ( e != world_entity )
		{
			CString strName = e->eclass->name;
			CString strKey, strVal;
			bool bCriteria = GetSelectAllCriteria( strKey, strVal );
			Sys_Printf( "Selecting all %s(s)\n", strName );
			Select_Deselect();
			
			for ( b = active_brushes.next ; b != &active_brushes ; b = next )
			{
				next = b->next;
				
				if ( FilterBrush( b ) )
					continue;
					
				e = b->owner;
				if ( e != NULL )
				{
					if ( strcmpi( e->eclass->name, strName ) == 0 )
					{
						bool doIt = true;
						if ( bCriteria )
						{
							CString str = ValueForKey( e, strKey );
							if ( str.CompareNoCase( strVal ) != 0 )
							{
								doIt = false;
							}
						}
						if ( doIt )
						{
							Brush_RemoveFromList( b );
							Brush_AddToList( b, &selected_brushes );
						}
					}
				}
			}
		}
	}
	Sys_UpdateWindows( W_ALL );
	
}

void Select_Reselect()
{
	brush_s* b;
	CPtrArray holdArray;
	for ( b = selected_brushes.next ; b && b != &selected_brushes ; b = b->next )
	{
		holdArray.Add( reinterpret_cast<void*>( b ) );
	}
	
	int n = holdArray.GetSize();
	while ( n-- > 0 )
	{
		b = reinterpret_cast<brush_s*>( holdArray.GetAt( n ) );
		Select_Brush( b );
	}
	Sys_UpdateWindows( W_ALL );
}


void Select_FitTexture( int nHeight, int nWidth )
{
	brush_s*        b;
	
	int nFaceCount = g_ptrSelectedFaces.GetSize();
	
	if ( selected_brushes.next == &selected_brushes && nFaceCount == 0 )
		return;
		
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
	{
		Brush_FitTexture( b, nHeight, nWidth );
		Brush_Build( b );
	}
	
	if ( nFaceCount > 0 )
	{
		for ( int i = 0; i < nFaceCount; i++ )
		{
			face_s* selFace = reinterpret_cast<face_s*>( g_ptrSelectedFaces.GetAt( i ) );
			brush_s* selBrush = reinterpret_cast<brush_s*>( g_ptrSelectedFaceBrushes.GetAt( i ) );
			Face_FitTexture( selFace, nHeight, nWidth );
			Brush_Build( selBrush );
		}
	}
	
	Sys_UpdateWindows( W_CAMERA );
}

void Select_AxialTexture()
{

}

void Select_Hide()
{
	for ( brush_s* b = selected_brushes.next ; b && b != &selected_brushes ; b = b->next )
	{
		b->hiddenBrush = true;
	}
	Sys_UpdateWindows( W_ALL );
}

void Select_ShowAllHidden()
{
	brush_s* b;
	for ( b = selected_brushes.next ; b && b != &selected_brushes ; b = b->next )
	{
		b->hiddenBrush = false;
	}
	for ( b = active_brushes.next ; b && b != &active_brushes ; b = b->next )
	{
		b->hiddenBrush = false;
	}
	Sys_UpdateWindows( W_ALL );
}


/*
============
Select_Invert
============
*/
void Select_Invert( void )
{
	brush_s* next, *prev;
	
	Sys_Printf( "inverting selection...\n" );
	
	next = active_brushes.next;
	prev = active_brushes.prev;
	if ( selected_brushes.next != &selected_brushes )
	{
		active_brushes.next = selected_brushes.next;
		active_brushes.prev = selected_brushes.prev;
		active_brushes.next->prev = &active_brushes;
		active_brushes.prev->next = &active_brushes;
	}
	else
	{
		active_brushes.next = &active_brushes;
		active_brushes.prev = &active_brushes;
	}
	if ( next != &active_brushes )
	{
		selected_brushes.next = next;
		selected_brushes.prev = prev;
		selected_brushes.next->prev = &selected_brushes;
		selected_brushes.prev->next = &selected_brushes;
	}
	else
	{
		selected_brushes.next = &selected_brushes;
		selected_brushes.prev = &selected_brushes;
	}
	
	Sys_UpdateWindows( W_ALL );
	
	Sys_Printf( "done.\n" );
}

