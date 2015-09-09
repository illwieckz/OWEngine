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
//  File name:   Vertsel.cpp
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

#define NEWEDGESEL 1

int	FindPoint( vec3_t point )
{
    int		i, j;
    
    for( i = 0 ; i < g_qeglobals.d_numpoints ; i++ )
    {
        for( j = 0 ; j < 3 ; j++ )
            if( fabs( point[j] - g_qeglobals.d_points[i][j] ) > 0.1 )
                break;
        if( j == 3 )
            return i;
    }
    
    g_qeglobals.d_points[g_qeglobals.d_numpoints] = point;
    if( g_qeglobals.d_numpoints < MAX_POINTS - 1 )
    {
        g_qeglobals.d_numpoints++;
    }
    
    return g_qeglobals.d_numpoints - 1;
}

int FindEdge( int p1, int p2, face_s* f )
{
    int		i;
    
    for( i = 0 ; i < g_qeglobals.d_numedges ; i++ )
        if( g_qeglobals.d_edges[i].p1 == p2 && g_qeglobals.d_edges[i].p2 == p1 )
        {
            g_qeglobals.d_edges[i].f2 = f;
            return i;
        }
        
    g_qeglobals.d_edges[g_qeglobals.d_numedges].p1 = p1;
    g_qeglobals.d_edges[g_qeglobals.d_numedges].p2 = p2;
    g_qeglobals.d_edges[g_qeglobals.d_numedges].f1 = f;
    
    if( g_qeglobals.d_numedges < MAX_EDGES - 1 )
    {
        g_qeglobals.d_numedges++;
    }
    
    return g_qeglobals.d_numedges - 1;
}

#ifdef NEWEDGESEL
void MakeFace( brush_s* b, face_s* f )
#else
void MakeFace( face_s* f )
#endif
{
    winding_t*	w;
    int			i;
    int			pnum[128];
    
#ifdef NEWEDGESEL
    w = Brush_MakeFaceWinding( b, f );
#else
    w = Brush_MakeFaceWinding( selected_brushes.next, f );
#endif
    if( !w )
        return;
    for( i = 0 ; i < w->numpoints ; i++ )
        pnum[i] = FindPoint( w->points[i] );
    for( i = 0 ; i < w->numpoints ; i++ )
        FindEdge( pnum[i], pnum[( i + 1 ) % w->numpoints], f );
        
    free( w );
}

void SetupVertexSelection( void )
{
    face_s*	f;
    brush_s* b;
    
    g_qeglobals.d_numpoints = 0;
    g_qeglobals.d_numedges = 0;
    
#ifdef NEWEDGESEL
    for( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
    {
        for( f = b->brush_faces ; f ; f = f->next )
            MakeFace( b, f );
    }
#else
    if( !QE_SingleBrush() )
        return;
    
    b = selected_brushes.next;
    for( f = b->brush_faces ; f ; f = f->next )
        MakeFace( b, f );
#endif
    
}


#ifdef NEWEDGESEL
void SelectFaceEdge( brush_s* b, face_s* f, int p1, int p2 )
#else
void SelectFaceEdge( face_s* f, int p1, int p2 )
#endif
{
    winding_t*	w;
    int			i, j, k;
    int			pnum[128];
    
#ifdef NEWEDGESEL
    w = Brush_MakeFaceWinding( b, f );
#else
    w = Brush_MakeFaceWinding( selected_brushes.next, f );
#endif
    if( !w )
        return;
    for( i = 0 ; i < w->numpoints ; i++ )
        pnum[i] = FindPoint( w->points[i] );
        
    for( i = 0 ; i < w->numpoints ; i++ )
        if( pnum[i] == p1 && pnum[( i + 1 ) % w->numpoints] == p2 )
        {
            f->planepts[0] = g_qeglobals.d_points[pnum[i]];
            f->planepts[1] = g_qeglobals.d_points[pnum[( i + 1 ) % w->numpoints]];
            f->planepts[2] = g_qeglobals.d_points[pnum[( i + 2 ) % w->numpoints]];
            for( j = 0 ; j < 3 ; j++ )
            {
                for( k = 0 ; k < 3 ; k++ )
                {
                    f->planepts[j][k] = floor( f->planepts[j][k] / g_qeglobals.d_gridsize + 0.5 ) * g_qeglobals.d_gridsize;
                }
            }
            
            AddPlanept( f->planepts[0] );
            AddPlanept( f->planepts[1] );
            break;
        }
        
    if( i == w->numpoints )
        Sys_Printf( "SelectFaceEdge: failed\n" );
    free( w );
}


void SelectVertex( int p1 )
{
    brush_s*		b;
    winding_t*	w;
    int			i, j, k;
    face_s*		f;
    
#ifdef NEWEDGESEL
    for( b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
    {
        for( f = b->brush_faces ; f ; f = f->next )
        {
            w =  Brush_MakeFaceWinding( b, f );
            if( !w )
                continue;
            for( i = 0 ; i < w->numpoints ; i++ )
            {
                if( FindPoint( w->points[i] ) == p1 )
                {
                    f->planepts[0] = w->points[( i + w->numpoints - 1 ) % w->numpoints].getXYZ();
                    f->planepts[1] = w->points[i].getXYZ();
                    f->planepts[2] = w->points[( i + 1 ) % w->numpoints].getXYZ();
                    for( j = 0 ; j < 3 ; j++ )
                    {
                        for( k = 0 ; k < 3 ; k++ )
                        {
                            ;//f->planepts[j][k] = floor(f->planepts[j][k]/g_qeglobals.d_gridsize+0.5)*g_qeglobals.d_gridsize;
                        }
                    }
                    
                    AddPlanept( f->planepts[1] );
                    //MessageBeep(-1);
                    
                    break;
                }
            }
            free( w );
        }
    }
#else
    b = selected_brushes.next;
    for( f = b->brush_faces ; f ; f = f->next )
    {
        w =  Brush_MakeFaceWinding( b, f );
        if( !w )
            continue;
        for( i = 0 ; i < w->numpoints ; i++ )
        {
            if( FindPoint( w->points[i] ) == p1 )
            {
                VectorCopy( w->points[( i + w->numpoints - 1 ) % w->numpoints], f->planepts[0] );
                VectorCopy( w->points[i], f->planepts[1] );
                VectorCopy( w->points[( i + 1 ) % w->numpoints], f->planepts[2] );
                for( j = 0 ; j < 3 ; j++ )
                {
                    for( k = 0 ; k < 3 ; k++ )
                    {
                        ;//f->planepts[j][k] = floor(f->planepts[j][k]/g_qeglobals.d_gridsize+0.5)*g_qeglobals.d_gridsize;
                    }
                }
    
                AddPlanept( f->planepts[1] );
                //MessageBeep(-1);
    
                break;
            }
        }
        free( w );
    }
#endif
}

void SelectEdgeByRay( const edVec3_c& org, const edVec3_c& dir )
{
    int		i, j, besti;
    float	d, bestd;
    edVec3_c	mid, temp;
    pedge_t*	e;
    
    // find the edge closest to the ray
    besti = -1;
    bestd = 8;
    
    for( i = 0 ; i < g_qeglobals.d_numedges ; i++ )
    {
        for( j = 0 ; j < 3 ; j++ )
            mid[j] = 0.5 * ( g_qeglobals.d_points[g_qeglobals.d_edges[i].p1][j] + g_qeglobals.d_points[g_qeglobals.d_edges[i].p2][j] );
            
        temp = mid - org;
        d = temp.dotProduct( dir );
        temp.vectorMA( org, d, dir );
        temp = mid - temp;
        d = temp.vectorLength();
        if( d < bestd )
        {
            bestd = d;
            besti = i;
        }
    }
    
    if( besti == -1 )
    {
        Sys_Printf( "Click didn't hit an edge\n" );
        return;
    }
    Sys_Printf( "hit edge\n" );
    
    // make the two faces that border the edge use the two edge points
    // as primary drag points
    g_qeglobals.d_num_move_points = 0;
    e = &g_qeglobals.d_edges[besti];
#ifdef NEWEDGESEL
    for( brush_s* b = selected_brushes.next ; b != &selected_brushes ; b = b->next )
    {
        SelectFaceEdge( b, e->f1, e->p1, e->p2 );
        SelectFaceEdge( b, e->f2, e->p2, e->p1 );
    }
#else
    SelectFaceEdge( e->f1, e->p1, e->p2 );
    SelectFaceEdge( e->f2, e->p2, e->p1 );
#endif
    
    
}

void SelectVertexByRay( const edVec3_c& org, const edVec3_c& dir )
{
    int		i, besti;
    float	d, bestd;
    edVec3_c	temp;
    
    // find the point closest to the ray
    besti = -1;
    bestd = 8;
    
    for( i = 0 ; i < g_qeglobals.d_numpoints ; i++ )
    {
        temp = g_qeglobals.d_points[i] - org;
        d = temp.dotProduct( dir );
        temp.vectorMA( org, d, dir );
        temp = g_qeglobals.d_points[i] - temp;
        d = temp.vectorLength();
        if( d < bestd )
        {
            bestd = d;
            besti = i;
        }
    }
    
    if( besti == -1 )
    {
        Sys_Printf( "Click didn't hit a vertex\n" );
        return;
    }
    Sys_Printf( "hit vertex\n" );
    g_qeglobals.d_move_points[g_qeglobals.d_num_move_points++] = g_qeglobals.d_points[besti];
    //SelectVertex (besti);
}



extern void AddPatchMovePoint( const edVec3_c& v, bool bMulti, bool bFull );
void SelectCurvePointByRay( const edVec3_c& org, vec3_t dir, int buttons )
{
    int		i, besti;
    float	d, bestd;
    edVec3_c	temp;
    
    // find the point closest to the ray
    besti = -1;
    bestd = 8;
    
    for( i = 0 ; i < g_qeglobals.d_numpoints ; i++ )
    {
        temp = g_qeglobals.d_points[i] - org;
        d = temp.dotProduct( dir );
        temp.vectorMA( org, d, dir );
        temp = g_qeglobals.d_points[i] - temp;
        d = temp.vectorLength();
        if( d <= bestd )
        {
            bestd = d;
            besti = i;
        }
    }
    
    if( besti == -1 )
    {
        if( g_pParentWnd->ActiveXY()->AreaSelectOK() )
        {
            g_qeglobals.d_select_mode = sel_area;
            g_qeglobals.d_vAreaTL = org;
            g_qeglobals.d_vAreaBR = org;
        }
        return;
    }
    //Sys_Printf ("hit vertex\n");
    AddPatchMovePoint( g_qeglobals.d_points[besti], buttons & MK_CONTROL, buttons & MK_SHIFT );
}



