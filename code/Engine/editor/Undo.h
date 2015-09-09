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
//  File name:   Undo.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Multilevel Undo/Redo
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

//start operation
void Undo_Start( char* operation );
//end operation
void Undo_End( void );
//add brush to the undo
void Undo_AddBrush( brush_s* pBrush );
//add a list with brushes to the undo
void Undo_AddBrushList( brush_s* brushlist );
//end a brush after the operation is performed
void Undo_EndBrush( brush_s* pBrush );
//end a list with brushes after the operation is performed
void Undo_EndBrushList( brush_s* brushlist );
//add entity to undo
void Undo_AddEntity( entity_s* entity );
//end an entity after the operation is performed
void Undo_EndEntity( entity_s* entity );
//undo last operation
void Undo_Undo( void );
//redo last undone operation
void Undo_Redo( void );
//returns true if there is something to be undone available
int  Undo_UndoAvailable( void );
//returns true if there is something to redo available
int  Undo_RedoAvailable( void );
//clear the undo buffer
void Undo_Clear( void );
//set maximum undo size (default 64)
void Undo_SetMaxSize( int size );
//get maximum undo size
int  Undo_GetMaxSize( void );
//set maximum undo memory in bytes (default 2 MB)
void Undo_SetMaxMemorySize( int size );
//get maximum undo memory in bytes
int  Undo_GetMaxMemorySize( void );
//returns the amount of memory used by undo
int  Undo_MemorySize( void );

