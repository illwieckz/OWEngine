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
//  File name:   CSG.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "qe3.h"
#include "winding.h"

/*
=============
CSG_MakeHollow
=============
*/
void Brush_Scale( brush_s* b )
{
	for ( face_s* f = b->brush_faces ; f ; f = f->next )
	{
		for ( int i = 0 ; i < 3 ; i++ )
		{
			f->planepts[i] *= g_qeglobals.d_gridsize;
		}
	}
}

void CSG_MakeHollow( void )
{
	brush_s*        b, *front, *back, *next;
	face_s*     f;
	face_s      split;
	edVec3_c        move;
	int         i;
	
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = next )
	{
		next = b->next;
		
		if ( b->owner->eclass->fixedsize || b->patchBrush || b->terrainBrush || b->hiddenBrush )
			continue;
			
		for ( f = b->brush_faces ; f ; f = f->next )
		{
			split = *f;
			move = f->plane.normal * g_qeglobals.d_gridsize;
			for ( i = 0 ; i < 3 ; i++ )
			{
				split.planepts[i] -= move;
			}
			
			Brush_SplitBrushByFace( b, &split, &front, &back );
			if ( back )
				Brush_Free( back );
			if ( front )
				Brush_AddToList( front, &selected_brushes );
		}
		Brush_Free( b );
	}
	Sys_UpdateWindows( W_ALL );
}

/*
=============
Brush_Merge

 Returns a new brush that is created by merging brush1 and brush2.
 May return NULL if brush1 and brush2 do not create a convex brush when merged.
 The input brushes brush1 and brush2 stay intact.

 if onlyshape is true then the merge is allowed based on the shape only
 otherwise the texture/shader references of faces in the same plane have to
 be the same as well.
=============
*/
brush_s* Brush_Merge( brush_s* brush1, brush_s* brush2, int onlyshape )
{
	int i, shared;
	brush_s* newbrush;
	face_s* face1, *face2, *newface, *f;
	
	// check for bounding box overlapp
	for ( i = 0; i < 3; i++ )
	{
		if ( brush1->getMins()[i] > brush2->getMaxs()[i] + ON_EPSILON
				|| brush1->getMaxs()[i] < brush2->getMins()[i] - ON_EPSILON )
		{
			// never merge if the brushes overlap
			return NULL;
		}
	}
	//
	shared = 0;
	// check if the new brush would be convex... flipped planes make a brush non-convex
	for ( face1 = brush1->brush_faces; face1; face1 = face1->next )
	{
		// don't check the faces of brush 1 and 2 touching each other
		for ( face2 = brush2->brush_faces; face2; face2 = face2->next )
		{
			if ( face1->plane.isPlaneEqual( face2->plane, true ) )
			{
				shared++;
				// there may only be ONE shared side
				if ( shared > 1 )
					return NULL;
				break;
			}
		}
		// if this face plane is shared
		if ( face2 ) continue;
		//
		for ( face2 = brush2->brush_faces; face2; face2 = face2->next )
		{
			// don't check the faces of brush 1 and 2 touching each other
			for ( f = brush1->brush_faces; f; f = f->next )
			{
				if ( face2->plane.isPlaneEqual( f->plane, true ) ) break;
			}
			if ( f )
				continue;
			//
			if ( face1->plane.isPlaneEqual( face2->plane, false ) )
			{
				//if the texture/shader references should be the same but are not
				if ( !onlyshape && stricmp( face1->texdef.name, face2->texdef.name ) != 0 ) return NULL;
				continue;
			}
			//
			if ( Winding_PlanesConcave( face1->face_winding, face2->face_winding,
										face1->plane.normal, face2->plane.normal,
										face1->plane.dist, face2->plane.dist ) )
			{
				return NULL;
			} //end if
		} //end for
	} //end for
	//
	newbrush = Brush_Alloc();
	//
	for ( face1 = brush1->brush_faces; face1; face1 = face1->next )
	{
		// don't add the faces of brush 1 and 2 touching each other
		for ( face2 = brush2->brush_faces; face2; face2 = face2->next )
		{
			if ( face1->plane.isPlaneEqual( face2->plane, true ) )
				break;
		}
		if ( face2 )
			continue;
		// don't add faces with the same plane twice
		for ( f = newbrush->brush_faces; f; f = f->next )
		{
			if ( face1->plane.isPlaneEqual( f->plane, false ) )
				break;
			if ( face1->plane.isPlaneEqual( f->plane, true ) )
				break;
		}
		if ( f )
			continue;
		//
		newface = Face_Alloc();
		newface->texdef = face1->texdef;
		newface->planepts[0] = face1->planepts[0];
		newface->planepts[1] = face1->planepts[1];
		newface->planepts[2] = face1->planepts[2];
		newface->plane = face1->plane;
		newface->next = newbrush->brush_faces;
		newbrush->brush_faces = newface;
	}
	//
	for ( face2 = brush2->brush_faces; face2; face2 = face2->next )
	{
		// don't add the faces of brush 1 and 2 touching each other
		for ( face1 = brush1->brush_faces; face1; face1 = face1->next )
		{
			if ( face2->plane.isPlaneEqual( face1->plane, true ) )
				break;
		}
		if ( face1 )
			continue;
		// don't add faces with the same plane twice
		for ( f = newbrush->brush_faces; f; f = f->next )
		{
			if ( face2->plane.isPlaneEqual( f->plane, false ) )
				break;
			if ( face2->plane.isPlaneEqual( f->plane, true ) )
				break;
		}
		if ( f )
			continue;
		//
		newface = Face_Alloc();
		newface->texdef = face2->texdef;
		newface->planepts[0] = face2->planepts[0];
		newface->planepts[1] = face2->planepts[1];
		newface->planepts[2] = face2->planepts[2];
		newface->plane = face2->plane;
		newface->next = newbrush->brush_faces;
		newbrush->brush_faces = newface;
	}
	// link the new brush to an entity
	Entity_LinkBrush( brush1->owner, newbrush );
	// build windings for the faces
	Brush_BuildWindings( newbrush, false );
	return newbrush;
}

/*
=============
Brush_MergeListPairs

  Returns a list with merged brushes.
  Tries to merge brushes pair wise.
  The input list is destroyed.
  Input and output should be a single linked list using .next
=============
*/
brush_s* Brush_MergeListPairs( brush_s* brushlist, int onlyshape )
{
	int nummerges, merged;
	brush_s* b1, *b2, *tail, *newbrush, *newbrushlist;
	brush_s* lastb2;
	
	if ( !brushlist ) return NULL;
	
	nummerges = 0;
	do
	{
		for ( tail = brushlist; tail; tail = tail->next )
		{
			if ( !tail->next ) break;
		}
		merged = 0;
		newbrushlist = NULL;
		for ( b1 = brushlist; b1; b1 = brushlist )
		{
			lastb2 = b1;
			for ( b2 = b1->next; b2; b2 = b2->next )
			{
				newbrush = Brush_Merge( b1, b2, onlyshape );
				if ( newbrush )
				{
					tail->next = newbrush;
					lastb2->next = b2->next;
					brushlist = brushlist->next;
					b1->next = b1->prev = NULL;
					b2->next = b2->prev = NULL;
					Brush_Free( b1 );
					Brush_Free( b2 );
					for ( tail = brushlist; tail; tail = tail->next )
					{
						if ( !tail->next ) break;
					} //end for
					merged++;
					nummerges++;
					break;
				}
				lastb2 = b2;
			}
			//if b1 can't be merged with any of the other brushes
			if ( !b2 )
			{
				brushlist = brushlist->next;
				//keep b1
				b1->next = newbrushlist;
				newbrushlist = b1;
			}
		}
		brushlist = newbrushlist;
	}
	while ( merged );
	return newbrushlist;
}

/*
=============
Brush_MergeList

 Tries to merge all brushes in the list into one new brush.
 The input brush list stays intact.
 Returns NULL if no merged brush can be created.
 To create a new brush the brushes in the list may not overlap and
 the outer faces of the brushes together should make a new convex brush.

 if onlyshape is true then the merge is allowed based on the shape only
 otherwise the texture/shader references of faces in the same plane have to
 be the same as well.
=============
*/
brush_s* Brush_MergeList( brush_s* brushlist, int onlyshape )
{
	brush_s* brush1, *brush2, *brush3, *newbrush;
	face_s* face1, *face2, *face3, *newface, *f;
	
	if ( !brushlist ) return NULL;
	for ( brush1 = brushlist; brush1; brush1 = brush1->next )
	{
		// check if the new brush would be convex... flipped planes make a brush concave
		for ( face1 = brush1->brush_faces; face1; face1 = face1->next )
		{
			// don't check face1 if it touches another brush
			for ( brush2 = brushlist; brush2; brush2 = brush2->next )
			{
				if ( brush2 == brush1 ) continue;
				for ( face2 = brush2->brush_faces; face2; face2 = face2->next )
				{
					if ( face1->plane.isPlaneEqual( face2->plane, true ) )
					{
						break;
					}
				}
				if ( face2 ) break;
			}
			// if face1 touches another brush
			if ( brush2 ) continue;
			//
			for ( brush2 = brush1->next; brush2; brush2 = brush2->next )
			{
				// don't check the faces of brush 2 touching another brush
				for ( face2 = brush2->brush_faces; face2; face2 = face2->next )
				{
					for ( brush3 = brushlist; brush3; brush3 = brush3->next )
					{
						if ( brush3 == brush2 ) continue;
						for ( face3 = brush3->brush_faces; face3; face3 = face3->next )
						{
							if ( face2->plane.isPlaneEqual( face3->plane, true ) ) break;
						}
						if ( face3 ) break;
					}
					// if face2 touches another brush
					if ( brush3 ) continue;
					//
					if ( face1->plane.isPlaneEqual( face2->plane, false ) )
					{
						//if the texture/shader references should be the same but are not
						if ( !onlyshape && stricmp( face1->texdef.name, face2->texdef.name ) != 0 ) return NULL;
						continue;
					}
					//
					if ( Winding_PlanesConcave( face1->face_winding, face2->face_winding,
												face1->plane.normal, face2->plane.normal,
												face1->plane.dist, face2->plane.dist ) )
					{
						return NULL;
					}
				}
			}
		}
	}
	//
	newbrush = Brush_Alloc();
	//
	for ( brush1 = brushlist; brush1; brush1 = brush1->next )
	{
		for ( face1 = brush1->brush_faces; face1; face1 = face1->next )
		{
			// don't add face1 to the new brush if it touches another brush
			for ( brush2 = brushlist; brush2; brush2 = brush2->next )
			{
				if ( brush2 == brush1 ) continue;
				for ( face2 = brush2->brush_faces; face2; face2 = face2->next )
				{
					if ( face1->plane.isPlaneEqual( face2->plane, true ) )
					{
						break;
					}
				}
				if ( face2 ) break;
			}
			if ( brush2 ) continue;
			// don't add faces with the same plane twice
			for ( f = newbrush->brush_faces; f; f = f->next )
			{
				if ( face1->plane.isPlaneEqual( f->plane, false ) )
					break;
				if ( face1->plane.isPlaneEqual( f->plane, true ) )
					break;
			}
			if ( f )
				continue;
			//
			newface = Face_Alloc();
			newface->texdef = face1->texdef;
			newface->planepts[0] = face1->planepts[0];
			newface->planepts[1] = face1->planepts[1];
			newface->planepts[2] = face1->planepts[2];
			newface->plane = face1->plane;
			newface->next = newbrush->brush_faces;
			newbrush->brush_faces = newface;
		}
	}
	// link the new brush to an entity
	Entity_LinkBrush( brushlist->owner, newbrush );
	// build windings for the faces
	Brush_BuildWindings( newbrush, false );
	return newbrush;
}

/*
=============
Brush_Subtract

 Returns a list of brushes that remain after B is subtracted from A.
 May by empty if A is contained inside B.
 The originals are undisturbed.
=============
*/
brush_s* Brush_Subtract( brush_s* a, brush_s* b )
{
	// a - b = out (list)
	brush_s* front, *back;
	brush_s* in, *out, *next;
	face_s* f;
	
	in = a;
	out = NULL;
	for ( f = b->brush_faces; f && in; f = f->next )
	{
		Brush_SplitBrushByFace( in, f, &front, &back );
		if ( in != a ) Brush_Free( in );
		if ( front )
		{
			// add to list
			front->next = out;
			out = front;
		}
		in = back;
	}
	//NOTE: in != a just in case brush b has no faces
	if ( in && in != a )
	{
		Brush_Free( in );
	}
	else
	{
		//didn't really intersect
		for ( b = out; b; b = next )
		{
			next = b->next;
			b->next = b->prev = NULL;
			Brush_Free( b );
		}
		return a;
	}
	return out;
}

/*
=============
CSG_Subtract
=============
*/
void CSG_Subtract( void )
{
	brush_s*        b, *s, *fragments, *nextfragment, *frag, *next, *snext;
	brush_s     fragmentlist;
	int         i, numfragments, numbrushes;
	
	Sys_Printf( "Subtracting...\n" );
	
	if ( selected_brushes.next == &selected_brushes )
	{
		Sys_Printf( "No brushes selected.\n" );
		return;
	}
	
	fragmentlist.next = &fragmentlist;
	fragmentlist.prev = &fragmentlist;
	
	numfragments = 0;
	numbrushes = 0;
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = next )
	{
		next = b->next;
		
		if ( b->owner->eclass->fixedsize )
			continue;   // can't use texture from a fixed entity, so don't subtract
			
		// chop all fragments further up
		for ( s = fragmentlist.next; s != &fragmentlist; s = snext )
		{
			snext = s->next;
			
			for ( i = 0 ; i < 3 ; i++ )
				if ( b->getMins()[i] >= s->getMaxs()[i] - ON_EPSILON
						|| b->getMaxs()[i] <= s->getMins()[i] + ON_EPSILON )
					break;
			if ( i != 3 )
				continue;   // definately don't touch
			fragments = Brush_Subtract( s, b );
			// if the brushes did not really intersect
			if ( fragments == s )
				continue;
			// try to merge fragments
			fragments = Brush_MergeListPairs( fragments, true );
			// add the fragments to the list
			for ( frag = fragments; frag; frag = nextfragment )
			{
				nextfragment = frag->next;
				frag->next = NULL;
				frag->owner = s->owner;
				Brush_AddToList( frag, &fragmentlist );
			}
			// free the original brush
			Brush_Free( s );
		}
		
		// chop any active brushes up
		for ( s = active_brushes.next; s != &active_brushes; s = snext )
		{
			snext = s->next;
			
			if ( s->owner->eclass->fixedsize || s->patchBrush || s->terrainBrush || s->hiddenBrush )
				continue;
				
			//face_s *pFace = s->brush_faces;
			if ( s->brush_faces->d_texture->bFromShader && ( s->brush_faces->d_texture->nShaderFlags & QER_NOCARVE ) )
			{
				continue;
			}
			
			for ( i = 0 ; i < 3 ; i++ )
				if ( b->getMins()[i] >= s->getMaxs()[i] - ON_EPSILON
						|| b->getMaxs()[i] <= s->getMins()[i] + ON_EPSILON )
					break;
			if ( i != 3 )
				continue;   // definately don't touch
				
			fragments = Brush_Subtract( s, b );
			// if the brushes did not really intersect
			if ( fragments == s )
				continue;
			//
			Undo_AddBrush( s );
			// one extra brush chopped up
			numbrushes++;
			// try to merge fragments
			fragments = Brush_MergeListPairs( fragments, true );
			// add the fragments to the list
			for ( frag = fragments; frag; frag = nextfragment )
			{
				nextfragment = frag->next;
				frag->next = NULL;
				frag->owner = s->owner;
				Brush_AddToList( frag, &fragmentlist );
			}
			// free the original brush
			Brush_Free( s );
		}
	}
	
	// move all fragments to the active brush list
	for ( frag = fragmentlist.next; frag != &fragmentlist; frag = nextfragment )
	{
		nextfragment = frag->next;
		numfragments++;
		Brush_RemoveFromList( frag );
		Brush_AddToList( frag, &active_brushes );
		Undo_EndBrush( frag );
	}
	
	if ( numfragments == 0 )
	{
		Sys_Printf( "Selected brush%s did not intersect with any other brushes.\n",
					( selected_brushes.next->next == &selected_brushes ) ? "" : "es" );
		return;
	}
	Sys_Printf( "done. (created %d fragment%s out of %d brush%s)\n", numfragments, ( numfragments == 1 ) ? "" : "s",
				numbrushes, ( numbrushes == 1 ) ? "" : "es" );
	Sys_UpdateWindows( W_ALL );
}

/*
=============
CSG_Merge
=============
*/
void CSG_Merge( void )
{
	brush_s* b, *next, *newlist, *newbrush;
	struct entity_s*    owner;
	
	Sys_Printf( "Merging...\n" );
	
	if ( selected_brushes.next == &selected_brushes )
	{
		Sys_Printf( "No brushes selected.\n" );
		return;
	}
	
	if ( selected_brushes.next->next == &selected_brushes )
	{
		Sys_Printf( "At least two brushes have to be selected.\n" );
		return;
	}
	
	owner = selected_brushes.next->owner;
	
	for ( b = selected_brushes.next; b != &selected_brushes; b = next )
	{
		next = b->next;
		
		if ( b->owner->eclass->fixedsize )
		{
			// can't use texture from a fixed entity, so don't subtract
			Sys_Printf( "Cannot add fixed size entities.\n" );
			return;
		}
		
		if ( b->patchBrush )
		{
			Sys_Printf( "Cannot add patches.\n" );
			return;
		}
		if ( b->terrainBrush )
		{
			Sys_Printf( "Cannot add terrains.\n" );
			return;
		}
		
		if ( b->brush_faces->d_texture->bFromShader && ( b->brush_faces->d_texture->nShaderFlags & QER_NOCARVE ) )
		{
			Sys_Printf( "Cannot add brushes using shaders that don't allows CSG operations.\n" );
			return;
		}
		
		if ( b->owner != owner )
		{
			Sys_Printf( "Cannot add brushes from different entities.\n" );
			return;
		}
		
	}
	
	newlist = NULL;
	for ( b = selected_brushes.next; b != &selected_brushes; b = next )
	{
		next = b->next;
		
		Brush_RemoveFromList( b );
		b->next = newlist;
		b->prev = NULL;
		newlist = b;
	}
	
	newbrush = Brush_MergeList( newlist, true );
	// if the new brush would not be convex
	if ( !newbrush )
	{
		// add the brushes back into the selection
		for ( b = newlist; b; b = next )
		{
			next = b->next;
			b->next = NULL;
			b->prev = NULL;
			Brush_AddToList( b, &selected_brushes );
		}
		Sys_Printf( "Cannot add a set of brushes with a concave hull.\n" );
		return;
	}
	// free the original brushes
	for ( b = newlist; b; b = next )
	{
		next = b->next;
		b->next = NULL;
		b->prev = NULL;
		Brush_Free( b );
	}
	Brush_AddToList( newbrush, &selected_brushes );
	
	Sys_Printf( "done.\n" );
	Sys_UpdateWindows( W_ALL );
}
