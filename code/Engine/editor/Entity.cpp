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
//  File name:   Entity.cpp
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

//
int g_entityId = 1;

char* ValueForKey( epair_s*& e, const char* key )
{
	epair_s* ep;
	for ( ep = e ; ep ; ep = ep->next )
	{
		if ( !strcmp( ep->key, key ) )
		{
			return ep->value;
		}
	}
	return "";
}


char* ValueForKey( entity_s* ent, const char* key )
{
	if ( ent == 0 )
		return "";
	return ValueForKey( ent->epairs, key );
}

void TrackMD3Angles( entity_s* e, const char* key, const char* value )
{
	if ( strcmpi( key, "angle" ) != 0 )
	{
		return;
	}
	
	if ( e->eclass->fixedsize && e->eclass->nShowFlags & ECLASS_MISCMODEL )
	{
		float a = FloatForKey( e, "angle" );
		float b = atof( value );
		if ( a != b )
		{
			vec3_t vAngle;
			vAngle[0] = vAngle[1] = 0;
			vAngle[2] = -a;
			Brush_Rotate( e->brushes.onext, vAngle, e->origin, true );
			vAngle[2] = b;
			Brush_Rotate( e->brushes.onext, vAngle, e->origin, true );
		}
	}
}

void    SetKeyValue( epair_s*& e, const char* key, const char* value )
{
	epair_s*    ep;
	for ( ep = e ; ep ; ep = ep->next )
	{
		if ( !strcmp( ep->key, key ) )
		{
			free( ep->value );
			ep->value = ( char* )qmalloc( strlen( value ) + 1 );
			strcpy( ep->value, value );
			return;
		}
	}
	ep = ( epair_s* )qmalloc( sizeof( *ep ) );
	ep->next = e;
	e = ep;
	ep->key = ( char* )qmalloc( strlen( key ) + 1 );
	strcpy( ep->key, key );
	ep->value = ( char* )qmalloc( strlen( value ) + 1 );
	strcpy( ep->value, value );
	
}


void    SetKeyValue( entity_s* ent, const char* key, const char* value )
{

	if ( ent == NULL )
		return;
		
	if ( !key || !key[0] )
		return;
		
	TrackMD3Angles( ent, key, value );
	
	SetKeyValue( ent->epairs, key, value );
	
}

void    DeleteKey( epair_s*& e, const char* key )
{
	epair_s**   ep, *next;
	
	ep = &e;
	while ( *ep )
	{
		next = *ep;
		if ( !strcmp( next->key, key ) )
		{
			*ep = next->next;
			free( next->key );
			free( next->value );
			free( next );
			return;
		}
		ep = &next->next;
	}
}


void    DeleteKey( entity_s* ent, const char* key )
{
	DeleteKey( ent->epairs, key );
}




float   FloatForKey( entity_s* ent, const char* key )
{
	char*   k;
	
	k = ValueForKey( ent, key );
	return atof( k );
}

int IntForKey( entity_s* ent, const char* key )
{
	char*   k;
	
	k = ValueForKey( ent, key );
	return atoi( k );
}

void    GetVectorForKey( entity_s* ent, const char* key, vec3_t vec )
{
	char*   k;
	
	k = ValueForKey( ent, key );
	sscanf( k, "%f %f %f", &vec[0], &vec[1], &vec[2] );
}

/*
===============
Entity_FreeEpairs

Frees the entity epairs.
===============
*/
void Entity_FreeEpairs( entity_s* e )
{
	epair_s*    ep, *next;
	
	for ( ep = e->epairs; ep; ep = next )
	{
		next = ep->next;
		free( ep->key );
		free( ep->value );
		free( ep );
	}
	e->epairs = NULL;
}

/*
===========
Entity_AddToList
===========
*/
void Entity_AddToList( entity_s* e, entity_s* list )
{
	if ( e->next || e->prev )
		Error( "Entity_AddToList: allready linked" );
	e->next = list->next;
	list->next->prev = e;
	list->next = e;
	e->prev = list;
}

/*
===========
Entity_RemoveFromList
===========
*/
void Entity_RemoveFromList( entity_s* e )
{
	if ( !e->next || !e->prev )
		Error( "Entity_RemoveFromList: not linked" );
	e->next->prev = e->prev;
	e->prev->next = e->next;
	e->next = e->prev = NULL;
}



/*
===============
Entity_Free

Frees the entity and any brushes is has.
The entity is removed from the global entities list.
===============
*/
void Entity_Free( entity_s* e )
{
	while ( e->brushes.onext != &e->brushes )
		Brush_Free( e->brushes.onext );
		
	if ( e->next )
	{
		e->next->prev = e->prev;
		e->prev->next = e->next;
	}
	
	Entity_FreeEpairs( e );
	
	free( e );
}

/*
=================
Entity_MemorySize
=================
*/
int Entity_MemorySize( entity_s* e )
{
	epair_s*    ep;
	int size = 0;
	
	for ( ep = e->epairs; ep; ep = ep->next )
	{
		size += _msize( ep->key );
		size += _msize( ep->value );
		size += _msize( ep );
	}
	size += _msize( e );
	return size;
}

/*
=================
ParseEpair
=================
*/
epair_s* ParseEpair( void )
{
	epair_s*    e;
	
	e = ( epair_s* )qmalloc( sizeof( *e ) );
	
	e->key = ( char* )qmalloc( strlen( token ) + 1 );
	strcpy( e->key, token );
	
	GetToken( false );
	e->value = ( char* )qmalloc( strlen( token ) + 1 );
	strcpy( e->value, token );
	
	return e;
}

/*
================
Entity_Parse

If onlypairs is set, the classname info will not
be looked up, and the entity will not be added
to the global list.  Used for parsing the project.
================
*/
entity_s*   Entity_Parse( bool onlypairs, brush_s* pList )
{
	entity_s*   ent;
	eclass_s*   e;
	brush_s*        b;
	edVec3_c        mins, maxs;
	epair_s*        ep;
	bool    has_brushes;
	
	if ( !GetToken( true ) )
		return NULL;
		
	if ( strcmp( token, "{" ) )
		Error( "ParseEntity: { not found" );
		
	ent = ( entity_s* )qmalloc( sizeof( *ent ) );
	ent->entityId = g_entityId++;
	ent->brushes.onext = ent->brushes.oprev = &ent->brushes;
	
	int n = 0;
	do
	{
		if ( !GetToken( true ) )
		{
			Warning( "ParseEntity: EOF without closing brace" );
			return NULL;
		}
		if ( !strcmp( token, "}" ) )
			break;
		if ( !strcmp( token, "{" ) )
		{
			b = Brush_Parse();
			if ( b != NULL )
			{
				b->owner = ent;
				// add to the end of the entity chain
				b->onext = &ent->brushes;
				b->oprev = ent->brushes.oprev;
				ent->brushes.oprev->onext = b;
				ent->brushes.oprev = b;
			}
			else
			{
				break;
			}
		}
		else
		{
			ep = ParseEpair();
			ep->next = ent->epairs;
			ent->epairs = ep;
		}
	}
	while ( 1 );
	
	// group info entity?
	if ( strcmp( ValueForKey( ent, "classname" ), "group_info" ) == 0 )
		return ent;
		
	if ( onlypairs )
		return ent;
		
	if ( ent->brushes.onext == &ent->brushes )
		has_brushes = false;
	else
		has_brushes = true;
		
	GetVectorForKey( ent, "origin", ent->origin );
	
	e = Eclass_ForName( ValueForKey( ent, "classname" ), has_brushes );
	ent->eclass = e;
	
	if ( e->fixedsize )
	{
		// fixed size entity
		if ( ent->brushes.onext != &ent->brushes )
		{
			printf( "Warning: Fixed size entity with brushes\n" );
#if 0
			while ( ent->brushes.onext != &ent->brushes )
			{
				// FIXME: this will free the entity and crash!
				Brush_Free( b );
			}
#endif
			ent->brushes.next = ent->brushes.prev = &ent->brushes;
		}
		
		// create a custom brush
		mins = e->mins + ent->origin;
		maxs = e->maxs + ent->origin;
		
		float a = 0;
		if ( e->nShowFlags & ECLASS_MISCMODEL )
		{
			char* p = ValueForKey( ent, "model" );
			if ( p != NULL && strlen( p ) > 0 )
			{
				edVec3_c vMin, vMax;
				a = FloatForKey( ent, "angle" );
				//if (GetCachedModel(ent, p, vMin, vMax))
				//{
				//  // create a custom brush
				//  mins = ent->md3Class->mins + ent->origin;
				//  maxs = ent->md3Class->maxs + ent->origin;
				//}
			}
		}
		
		b = Brush_Create( mins, maxs, &e->texdef );
		
		if ( a )
		{
			vec3_t vAngle;
			vAngle[0] = vAngle[1] = 0;
			vAngle[2] = a;
			Brush_Rotate( b, vAngle, ent->origin, false );
		}
		
		
		b->owner = ent;
		
		b->onext = ent->brushes.onext;
		b->oprev = &ent->brushes;
		ent->brushes.onext->oprev = b;
		ent->brushes.onext = b;
	}
	else
	{
		// brush entity
		if ( ent->brushes.next == &ent->brushes )
			printf( "Warning: Brush entity with no brushes\n" );
	}
	
	// add all the brushes to the main list
	if ( pList )
	{
		for ( b = ent->brushes.onext ; b != &ent->brushes ; b = b->onext )
		{
			b->next = pList->next;
			pList->next->prev = b;
			b->prev = pList;
			pList->next = b;
		}
	}
	
	return ent;
}

/*
============
Entity_Write
============
*/
void Entity_Write( entity_s* e, FILE* f, bool use_region )
{
	epair_s*        ep;
	brush_s*        b;
	edVec3_c        origin;
	char        text[128];
	int         count;
	
	// if none of the entities brushes are in the region,
	// don't write the entity at all
	if ( use_region )
	{
		// in region mode, save the camera position as playerstart
		if ( !strcmp( ValueForKey( e, "classname" ), "info_player_start" ) )
		{
			fprintf( f, "{\n" );
			fprintf( f, "\"classname\" \"info_player_start\"\n" );
			fprintf( f, "\"origin\" \"%i %i %i\"\n", ( int )g_pParentWnd->GetCamera()->Camera().origin[0],
					 ( int )g_pParentWnd->GetCamera()->Camera().origin[1], ( int )g_pParentWnd->GetCamera()->Camera().origin[2] );
			fprintf( f, "\"angle\" \"%i\"\n", ( int )g_pParentWnd->GetCamera()->Camera().angles[YAW] );
			fprintf( f, "}\n" );
			return;
		}
		
		for ( b = e->brushes.onext ; b != &e->brushes ; b = b->onext )
			if ( !Map_IsBrushFiltered( b ) )
				break;  // got one
				
		if ( b == &e->brushes )
			return;     // nothing visible
	}
	
	if ( e->eclass->nShowFlags & ECLASS_PLUGINENTITY )
	{
		// NOTE: the whole brush placement / origin stuff is a mess
		origin = e->origin;
		sprintf( text, "%i %i %i", ( int )origin[0], ( int )origin[1], ( int )origin[2] );
		SetKeyValue( e, "origin", text );
	}
	// if fixedsize, calculate a new origin based on the current
	// brush position
	else if ( e->eclass->fixedsize )
	{
		//if (e->eclass->nShowFlags & ECLASS_MISCMODEL && e->md3Class != NULL)
		//{
		//  origin = e->origin;
		//  //VectorSubtract (e->brushes.onext->mins, e->md3Class->mins, origin);
		//}
		//else
		{
			origin = e->brushes.onext->getMins() - e->eclass->mins;
		}
		sprintf( text, "%i %i %i", ( int )origin[0], ( int )origin[1], ( int )origin[2] );
		SetKeyValue( e, "origin", text );
	}
	
	fprintf( f, "{\n" );
	for ( ep = e->epairs ; ep ; ep = ep->next )
		fprintf( f, "\"%s\" \"%s\"\n", ep->key, ep->value );
		
	if ( !e->eclass->fixedsize )
	{
		count = 0;
		for ( b = e->brushes.onext ; b != &e->brushes ; b = b->onext )
		{
			if ( !use_region || !Map_IsBrushFiltered( b ) )
			{
				fprintf( f, "// brush %i\n", count );
				count++;
				Brush_Write( b, f );
			}
		}
	}
	fprintf( f, "}\n" );
}



bool IsBrushSelected( brush_s* bSel )
{
	for ( brush_s* b = selected_brushes.next ; b != NULL && b != &selected_brushes; b = b->next )
	{
		if ( b == bSel )
			return true;
	}
	return false;
}

//
//============
//Entity_WriteSelected
//============
//
void Entity_WriteSelected( entity_s* e, FILE* f )
{
	epair_s*        ep;
	brush_s*        b;
	edVec3_c        origin;
	char        text[128];
	int         count;
	
	for ( b = e->brushes.onext ; b != &e->brushes ; b = b->onext )
		if ( IsBrushSelected( b ) )
			break;  // got one
			
	if ( b == &e->brushes )
		return;     // nothing selected
		
	// if fixedsize, calculate a new origin based on the current
	// brush position
	if ( e->eclass->fixedsize )
	{
		//if (e->eclass->nShowFlags & ECLASS_MISCMODEL && e->md3Class != NULL)
		//{
		//  origin = e->origin;
		////VectorSubtract (e->brushes.onext->mins, e->md3Class->mins, origin);
		//}
		//else
		{
			origin = e->brushes.onext->getMins() - e->eclass->mins;
		}
		sprintf( text, "%i %i %i", ( int )origin[0], ( int )origin[1], ( int )origin[2] );
		SetKeyValue( e, "origin", text );
	}
	
	fprintf( f, "{\n" );
	for ( ep = e->epairs ; ep ; ep = ep->next )
		fprintf( f, "\"%s\" \"%s\"\n", ep->key, ep->value );
		
	if ( !e->eclass->fixedsize )
	{
		count = 0;
		for ( b = e->brushes.onext ; b != &e->brushes ; b = b->onext )
		{
			if ( IsBrushSelected( b ) )
			{
				fprintf( f, "// brush %i\n", count );
				count++;
				Brush_Write( b, f );
			}
		}
	}
	fprintf( f, "}\n" );
}


//
//============
//Entity_WriteSelected to a CMemFile
//============
//
void Entity_WriteSelected( entity_s* e, CMemFile* pMemFile )
{
	epair_s*        ep;
	brush_s*        b;
	edVec3_c        origin;
	char        text[128];
	int         count;
	
	for ( b = e->brushes.onext ; b != &e->brushes ; b = b->onext )
		if ( IsBrushSelected( b ) )
			break;  // got one
			
	if ( b == &e->brushes )
		return;     // nothing selected
		
	// if fixedsize, calculate a new origin based on the current
	// brush position
	if ( e->eclass->fixedsize )
	{
		//if (e->eclass->nShowFlags & ECLASS_MISCMODEL && e->md3Class != NULL)
		//{
		////VectorSubtract (e->brushes.onext->mins, e->md3Class->mins, origin);
		//  origin = e->origin;
		//}
		//else
		{
			origin =  e->brushes.onext->getMins() - e->eclass->mins;
		}
		sprintf( text, "%i %i %i", ( int )origin[0], ( int )origin[1], ( int )origin[2] );
		SetKeyValue( e, "origin", text );
	}
	
	MemFile_fprintf( pMemFile, "{\n" );
	for ( ep = e->epairs ; ep ; ep = ep->next )
		MemFile_fprintf( pMemFile, "\"%s\" \"%s\"\n", ep->key, ep->value );
		
	if ( !e->eclass->fixedsize )
	{
		count = 0;
		for ( b = e->brushes.onext ; b != &e->brushes ; b = b->onext )
		{
			if ( IsBrushSelected( b ) )
			{
				MemFile_fprintf( pMemFile, "// brush %i\n", count );
				count++;
				Brush_Write( b, pMemFile );
			}
		}
	}
	MemFile_fprintf( pMemFile, "}\n" );
}




/*
============
Entity_Create

Creates a new entity out of the selected_brushes list.
If the entity class is fixed size, the brushes are only
used to find a midpoint.  Otherwise, the brushes have
their ownership transfered to the new entity.
============
*/
entity_s*   Entity_Create( eclass_s* c )
{
	entity_s*   e;
	brush_s*        b;
	edVec3_c    mins, maxs;
	int         i;
	
	// check to make sure the brushes are ok
	
	for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
	{
		if ( b->owner != world_entity )
		{
			Sys_Printf( "Entity NOT created, brushes not all from world\n" );
			Sys_Beep();
			return NULL;
		}
	}
	
	// create it
	
	e = ( entity_s* )qmalloc( sizeof( *e ) );
	e->entityId = g_entityId++;
	e->brushes.onext = e->brushes.oprev = &e->brushes;
	e->eclass = c;
	SetKeyValue( e, "classname", c->name );
	
	// add the entity to the entity list
	Entity_AddToList( e, &entities );
	
	
	if ( c->fixedsize )
	{
		//
		// just use the selection for positioning
		//
		b = selected_brushes.next;
		for ( i = 0 ; i < 3 ; i++ )
			e->origin[i] = b->getMins()[i] - c->mins[i];
			
		// create a custom brush
		mins = c->mins + e->origin;
		maxs = c->maxs + e->origin;
		
		b = Brush_Create( mins, maxs, &c->texdef );
		
		Entity_LinkBrush( e, b );
		
		// delete the current selection
		Select_Delete();
		
		// select the new brush
		b->next = b->prev = &selected_brushes;
		selected_brushes.next = selected_brushes.prev = b;
		
		Brush_Build( b );
	}
	else
	{
		//
		// change the selected brushes over to the new entity
		//
		for ( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
		{
			Entity_UnlinkBrush( b );
			Entity_LinkBrush( e, b );
			Brush_Build( b );   // so the key brush gets a name
		}
	}
	
	Sys_UpdateWindows( W_ALL );
	return e;
}


/*
===========
Entity_LinkBrush
===========
*/
void Entity_LinkBrush( entity_s* e, brush_s* b )
{
	if ( b->oprev || b->onext )
		Error( "Entity_LinkBrush: Allready linked" );
	b->owner = e;
	
	b->onext = e->brushes.onext;
	b->oprev = &e->brushes;
	e->brushes.onext->oprev = b;
	e->brushes.onext = b;
}

/*
===========
Entity_UnlinkBrush
===========
*/
void Entity_UnlinkBrush( brush_s* b )
{
	//if (!b->owner || !b->onext || !b->oprev)
	if ( !b->onext || !b->oprev )
		Error( "Entity_UnlinkBrush: Not currently linked" );
	b->onext->oprev = b->oprev;
	b->oprev->onext = b->onext;
	b->onext = b->oprev = NULL;
	b->owner = NULL;
}


/*
===========
Entity_Clone
===========
*/
entity_s*   Entity_Clone( entity_s* e )
{
	entity_s*   n;
	epair_s*        ep, *np;
	
	n = ( entity_s* )qmalloc( sizeof( *n ) );
	n->entityId = g_entityId++;
	n->brushes.onext = n->brushes.oprev = &n->brushes;
	n->eclass = e->eclass;
	
	// add the entity to the entity list
	Entity_AddToList( n, &entities );
	
	for ( ep = e->epairs ; ep ; ep = ep->next )
	{
		np = ( epair_s* )qmalloc( sizeof( *np ) );
		np->key = copystring( ep->key );
		np->value = copystring( ep->value );
		np->next = n->epairs;
		n->epairs = np;
	}
	return n;
}

int GetUniqueTargetId( int iHint )
{
	int iMin, iMax, i;
	BOOL fFound;
	entity_s* pe;
	
	fFound = FALSE;
	pe = entities.next;
	iMin = 0;
	iMax = 0;
	
	for ( ; pe != NULL && pe != &entities ; pe = pe->next )
	{
		i = IntForKey( pe, "target" );
		if ( i )
		{
			iMin = min( i, iMin );
			iMax = max( i, iMax );
			if ( i == iHint )
				fFound = TRUE;
		}
	}
	
	if ( fFound )
		return iMax + 1;
	else
		return iHint;
}

entity_s* FindEntity( char* pszKey, char* pszValue )
{
	entity_s* pe;
	
	pe = entities.next;
	
	for ( ; pe != NULL && pe != &entities ; pe = pe->next )
	{
		if ( !strcmp( ValueForKey( pe, pszKey ), pszValue ) )
			return pe;
	}
	
	return NULL;
}

entity_s* FindEntityInt( char* pszKey, int iValue )
{
	entity_s* pe;
	
	pe = entities.next;
	
	for ( ; pe != NULL && pe != &entities ; pe = pe->next )
	{
		if ( IntForKey( pe, pszKey ) == iValue )
			return pe;
	}
	
	return NULL;
}
