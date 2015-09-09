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
//  File name:   cl_scrn.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: master forrefresh, status bar, console, chat, etc...
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "client.h"
#include <api/rAPI.h>
#include <api/loadingScreenMgrAPI.h>
#include <shared/str.h>
#include <shared/colorTable.h>

bool	scr_initialized;		// ready to draw

cvar_s*		cl_timegraph;
cvar_s*		cl_debuggraph;
cvar_s*		cl_graphheight;
cvar_s*		cl_graphscale;
cvar_s*		cl_graphshift;

/*
================
SCR_DrawNamedPic

Coordinates are 640*480 virtual values
=================
*/
void SCR_DrawNamedPic( float x, float y, float width, float height, const char* picname )
{
    mtrAPI_i* hShader;
    
    assert( width != 0 );
    
    hShader = rf->registerMaterial( picname );
    SCR_AdjustFrom640( &x, &y, &width, &height );
    rf->drawStretchPic( x, y, width, height, 0, 0, 1, 1, hShader );
}


/*
================
SCR_AdjustFrom640

Adjusted for resolution and screen aspect ratio
================
*/
void SCR_AdjustFrom640( float* x, float* y, float* w, float* h )
{
    float	xscale;
    float	yscale;
    
#if 0
    // adjust for wide screens
    if( rf->getWinWidth() * 480 > rf->getWinHeight() * 640 )
    {
        *x += 0.5 * ( rf->getWinWidth() - ( rf->getWinHeight() * 640 / 480 ) );
    }
#endif
    
    // scale for screen sizes
    xscale = rf->getWinWidth() / 640.0;
    yscale = rf->getWinHeight() / 480.0;
    if( x )
    {
        *x *= xscale;
    }
    if( y )
    {
        *y *= yscale;
    }
    if( w )
    {
        *w *= xscale;
    }
    if( h )
    {
        *h *= yscale;
    }
}

/*
================
SCR_FillRect

Coordinates are 640*480 virtual values
=================
*/
void SCR_FillRect( float x, float y, float width, float height, const float* color )
{
    rf->set2DColor( color );
    
    SCR_AdjustFrom640( &x, &y, &width, &height );
    rf->drawStretchPic( x, y, width, height, 0, 0, 0, 0, cls.whiteShader );
    
    rf->set2DColor( NULL );
}


/*
================
SCR_DrawPic

Coordinates are 640*480 virtual values
=================
*/
void SCR_DrawPic( float x, float y, float width, float height, class mtrAPI_i* hShader )
{
    SCR_AdjustFrom640( &x, &y, &width, &height );
    rf->drawStretchPic( x, y, width, height, 0, 0, 1, 1, hShader );
}



/*
** SCR_DrawChar
** chars are drawn at 640*480 virtual screen size
*/
static void SCR_DrawChar( int x, int y, float size, int ch )
{
    int row, col;
    float frow, fcol;
    float	ax, ay, aw, ah;
    
    ch &= 255;
    
    if( ch == ' ' )
    {
        return;
    }
    
    if( y < -size )
    {
        return;
    }
    
    ax = x;
    ay = y;
    aw = size;
    ah = size;
    SCR_AdjustFrom640( &ax, &ay, &aw, &ah );
    
    row = ch >> 4;
    col = ch & 15;
    
    frow = row * 0.0625;
    fcol = col * 0.0625;
    size = 0.0625;
    
    rf->drawStretchPic( ax, ay, aw, ah,
                        fcol, frow,
                        fcol + size, frow + size,
                        cls.charSetShader );
}

/*
** SCR_DrawSmallChar
** small chars are drawn at native screen resolution
*/
void SCR_DrawSmallChar( int x, int y, int ch )
{
    int row, col;
    float frow, fcol;
    float size;
    
    ch &= 255;
    
    if( ch == ' ' )
    {
        return;
    }
    
    if( y < -SMALLCHAR_HEIGHT )
    {
        return;
    }
    
    row = ch >> 4;
    col = ch & 15;
    
    frow = row * 0.0625;
    fcol = col * 0.0625;
    size = 0.0625;
    
    rf->drawStretchPic( x, y, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT,
                        fcol, frow,
                        fcol + size, frow + size,
                        cls.charSetShader );
}


/*
==================
SCR_DrawBigString[Color]

Draws a multi-colored string with a drop shadow, optionally forcing
to a fixed color.

Coordinates are at 640 by 480 virtual resolution
==================
*/
void SCR_DrawStringExt( int x, int y, float size, const char* string, float* setColor, bool forceColor,
                        bool noColorEscape )
{
    vec4_t		color;
    const char*	s;
    int			xx;
    
    // draw the drop shadow
    color[0] = color[1] = color[2] = 0;
    color[3] = setColor[3];
    rf->set2DColor( color );
    s = string;
    xx = x;
    while( *s )
    {
        if( !noColorEscape && Q_IsColorString( s ) )
        {
            s += 2;
            continue;
        }
        SCR_DrawChar( xx + 2, y + 2, size, *s );
        xx += size;
        s++;
    }
    
    
    // draw the colored text
    s = string;
    xx = x;
    rf->set2DColor( setColor );
    while( *s )
    {
        if( !noColorEscape && Q_IsColorString( s ) )
        {
            if( !forceColor )
            {
                memcpy( color, g_color_table[ColorIndex( *( s + 1 ) )], sizeof( color ) );
                color[3] = setColor[3];
                rf->set2DColor( color );
            }
            s += 2;
            continue;
        }
        SCR_DrawChar( xx, y, size, *s );
        xx += size;
        s++;
    }
    rf->set2DColor( NULL );
}


void SCR_DrawBigString( int x, int y, const char* s, float alpha, bool noColorEscape )
{
    float	color[4];
    
    color[0] = color[1] = color[2] = 1.0;
    color[3] = alpha;
    SCR_DrawStringExt( x, y, BIGCHAR_WIDTH, s, color, false, noColorEscape );
}

void SCR_DrawBigStringColor( int x, int y, const char* s, vec4_t color, bool noColorEscape )
{
    SCR_DrawStringExt( x, y, BIGCHAR_WIDTH, s, color, true, noColorEscape );
}


/*
==================
SCR_DrawSmallString[Color]

Draws a multi-colored string with a drop shadow, optionally forcing
to a fixed color.
==================
*/
void SCR_DrawSmallStringExt( int x, int y, const char* string, float* setColor, bool forceColor,
                             bool noColorEscape )
{
    vec4_t		color;
    const char*	s;
    int			xx;
    
    // draw the colored text
    s = string;
    xx = x;
    rf->set2DColor( setColor );
    while( *s )
    {
        if( Q_IsColorString( s ) )
        {
            if( !forceColor )
            {
                memcpy( color, g_color_table[ColorIndex( *( s + 1 ) )], sizeof( color ) );
                color[3] = setColor[3];
                rf->set2DColor( color );
            }
            if( !noColorEscape )
            {
                s += 2;
                continue;
            }
        }
        SCR_DrawSmallChar( xx, y, *s );
        xx += SMALLCHAR_WIDTH;
        s++;
    }
    rf->set2DColor( NULL );
}



/*
** SCR_Strlen -- skips color escape codes
*/
static int SCR_Strlen( const char* str )
{
    const char* s = str;
    int count = 0;
    
    while( *s )
    {
        if( Q_IsColorString( s ) )
        {
            s += 2;
        }
        else
        {
            count++;
            s++;
        }
    }
    
    return count;
}

/*
** SCR_GetBigStringWidth
*/
int	SCR_GetBigStringWidth( const char* str )
{
    return SCR_Strlen( str ) * BIGCHAR_WIDTH;
}


//===============================================================================

/*
=================
SCR_DrawDemoRecording
=================
*/
void SCR_DrawDemoRecording( void )
{
    char	string[1024];
    int		pos;
    
    if( !clc.demorecording )
    {
        return;
    }
    if( clc.spDemoRecording )
    {
        return;
    }
    
    pos = FS_FTell( clc.demofile );
    sprintf( string, "RECORDING %s: %ik", clc.demoName, pos / 1024 );
    
    SCR_DrawStringExt( 320 - strlen( string ) * 4, 20, 8, string, g_color_table[7], true, false );
}


#ifdef USE_VOIP
/*
=================
SCR_DrawVoipMeter
=================
*/
void SCR_DrawVoipMeter( void )
{
    char	buffer[16];
    char	string[256];
    int limit, i;
    
    if( !cl_voipShowMeter->integer )
        return;  // player doesn't want to show meter at all.
    else if( !cl_voipSend->integer )
        return;  // not recording at the moment.
    else if( clc.state != CA_ACTIVE )
        return;  // not connected to a server.
    else if( !clc.voipEnabled )
        return;  // server doesn't support VoIP.
    else if( clc.demoplaying )
        return;  // playing back a demo.
    else if( !cl_voip->integer )
        return;  // client has VoIP support disabled.
        
    limit = ( int )( clc.voipPower * 10.0f );
    if( limit > 10 )
        limit = 10;
        
    for( i = 0; i < limit; i++ )
        buffer[i] = '*';
    while( i < 10 )
        buffer[i++] = ' ';
    buffer[i] = '\0';
    
    sprintf( string, "VoIP: [%s]", buffer );
    SCR_DrawStringExt( 320 - strlen( string ) * 4, 10, 8, string, g_color_table[7], true, false );
}
#endif




/*
===============================================================================

DEBUG GRAPH

===============================================================================
*/

static	int			current;
static	float		values[1024];

/*
==============
SCR_DebugGraph
==============
*/
void SCR_DebugGraph( float value )
{
    values[current] = value;
    current = ( current + 1 ) % ARRAY_LEN( values );
}

/*
==============
SCR_DrawDebugGraph
==============
*/
void SCR_DrawDebugGraph( void )
{
    int		a, x, y, w, i, h;
    float	v;
    
    //
    // draw the graph
    //
    w = rf->getWinWidth();
    x = 0;
    y = rf->getWinHeight();
    rf->set2DColor( g_color_table[0] );
    rf->drawStretchPic( x, y - cl_graphheight->integer,
                        w, cl_graphheight->integer, 0, 0, 0, 0, cls.whiteShader );
    rf->set2DColor( NULL );
    
    for( a = 0 ; a < w ; a++ )
    {
        i = ( ARRAY_LEN( values ) + current - 1 - ( a % ARRAY_LEN( values ) ) ) % ARRAY_LEN( values );
        v = values[i];
        v = v * cl_graphscale->integer + cl_graphshift->integer;
        
        if( v < 0 )
            v += cl_graphheight->integer * ( 1 + ( int )( -v / cl_graphheight->integer ) );
        h = ( int )v % cl_graphheight->integer;
        rf->drawStretchPic( x + w - 1 - a, y - h, 1, h, 0, 0, 0, 0, cls.whiteShader );
    }
}

//=============================================================================

/*
==================
SCR_Init
==================
*/
void SCR_Init( void )
{
    cl_timegraph = Cvar_Get( "timegraph", "0", CVAR_CHEAT );
    cl_debuggraph = Cvar_Get( "debuggraph", "0", CVAR_CHEAT );
    cl_graphheight = Cvar_Get( "graphheight", "32", CVAR_CHEAT );
    cl_graphscale = Cvar_Get( "graphscale", "1", CVAR_CHEAT );
    cl_graphshift = Cvar_Get( "graphshift", "0", CVAR_CHEAT );
    
    scr_initialized = true;
}


//=======================================================

/*
==================
SCR_DrawScreenField

This will be called twice if rendering in stereo mode
==================
*/
void SCR_DrawScreenField()
{
    bool uiFullscreen;
    
    rf->beginFrame();
    
    // let the loadingScreenMGR override old drawing routines
    if( g_loadingScreen && g_loadingScreen->isEmpty() == false )
    {
        g_loadingScreen->addDrawCalls();
        return;
    }
    
    uiFullscreen = 0;// (uivm && VM_Call( uivm, UI_IS_FULLSCREEN ));
    
    // wide aspect ratio screens need to have the sides cleared
    // unless they are displaying game renderings
    if( uiFullscreen || ( clc.state != CA_ACTIVE && clc.state != CA_CINEMATIC ) )
    {
        if( rf->getWinWidth() * 480 > rf->getWinHeight() * 640 )
        {
            rf->set2DColor( g_color_table[0] );
            rf->drawStretchPic( 0, 0, rf->getWinWidth(), rf->getWinHeight(), 0, 0, 0, 0, cls.whiteShader );
            rf->set2DColor( NULL );
        }
    }
    
    // if the menu is going to cover the entire screen, we
    // don't need to render anything under it
    if( 1 && !uiFullscreen )
    {
        switch( clc.state )
        {
            default:
                Com_Error( ERR_FATAL, "SCR_DrawScreenField: bad clc.state" );
                break;
            case CA_CINEMATIC:
                SCR_DrawCinematic();
                break;
            case CA_DISCONNECTED:
                // force menu up
                //		S_StopAllSounds();
//			VM_Call( uivm, UI_SET_ACTIVE_MENU, UIMENU_MAIN );
                break;
            case CA_CONNECTING:
            case CA_CHALLENGING:
            case CA_CONNECTED:
                // connecting clients will only show the connection dialog
                // refresh to update the time
//			VM_Call( uivm, UI_REFRESH, cls.realtime );
//			VM_Call( uivm, UI_DRAW_CONNECT_SCREEN, false );
                break;
            case CA_LOADING:
            case CA_PRIMED:
                // draw the game information screen and loading progress
                CL_CGameRendering();
                
                // also draw the connection information, so it doesn't
                // flash away too briefly on local or lan games
                // refresh to update the time
//			VM_Call( uivm, UI_REFRESH, cls.realtime );
//			VM_Call( uivm, UI_DRAW_CONNECT_SCREEN, true );
                break;
            case CA_ACTIVE:
                // always supply STEREO_CENTER as vieworg offset is now done by the engine.
                CL_CGameRendering();
                SCR_DrawDemoRecording();
#ifdef USE_VOIP
                SCR_DrawVoipMeter();
#endif
                break;
        }
    }
    
    // the menu draws next
//	if ( Key_GetCatcher( ) & KEYCATCH_UI && uivm ) {
//		VM_Call( uivm, UI_REFRESH, cls.realtime );
//	}

    // console draws next
    Con_DrawConsole();
    
    if( 1 && clc.downloadName[0] )
    {
        str txt = va( "Downloading %s....\n", clc.downloadName );
        SCR_DrawBigString( 30, 30, txt, 1, false );
        txt = va( "Block %i, count %i\n", clc.downloadBlock, clc.downloadCount );
        SCR_DrawBigString( 30, 60, txt, 1, false );
    }
    
    // debug graph can be drawn on top of anything
    if( cl_debuggraph->integer || cl_timegraph->integer || cl_debugMove->integer )
    {
        SCR_DrawDebugGraph();
    }
}

/*
==================
SCR_UpdateScreen

This is called every frame, and can also be called explicitly to flush
text to the screen.
==================
*/
void SCR_UpdateScreen( void )
{
    static int	recursive;
    
    if( !scr_initialized )
    {
        return;				// not initialized yet
    }
    
    if( ++recursive > 2 )
    {
        Com_Error( ERR_FATAL, "SCR_UpdateScreen: recursively called" );
    }
    recursive = 1;
    
    // If there is no VM, there are also no rendering commands issued. Stop the renderer in
    // that case.
    if( 1 || com_dedicated->integer )
    {
    
        //// XXX
        //int in_anaglyphMode = Cvar_VariableIntegerValue("r_anaglyphMode");
        //// if running in stereo, we need to draw the frame twice
        //if ( cls.glconfig.stereoEnabled || in_anaglyphMode) {
        //	SCR_DrawScreenField( STEREO_LEFT );
        //	SCR_DrawScreenField( STEREO_RIGHT );
        //} else {
        SCR_DrawScreenField();
        //}
        
        
        //if ( com_speeds->integer ) {
        //	re.EndFrame( &time_frontend, &time_backend );
        //} else {
        rf->endFrame();
        //}
    }
    
    recursive = 0;
}

