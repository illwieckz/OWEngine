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
//  File name:   QE3.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __QE3_H__
#define __QE3_H__

// disable data conversion warnings for gl
#pragma warning(disable : 4244)     // MIPS
#pragma warning(disable : 4136)     // X86
#pragma warning(disable : 4051)     // ALPHA

#include <windows.h>

#include <gl/gl.h>

#include <math.h>
#include <stdlib.h>
#include <malloc.h>

// this define to use HTREEITEM and MFC stuff in the headers
#define QERTYPES_USE_MFC
#include "qertypes.h"
#include "cmdlib.h"
#include "mathlib.h"
#include "parse.h"
#include "lbmlib.h"

#include <commctrl.h>
#include "afxres.h"
#include "resource.h"

#include "qedefs.h"

#include "qfiles.h"

#include "textures.h"
#include "brush.h"
#include "entity.h"
#include "map.h"
#include "select.h"

#include "camera.h"
#include "xy.h"
#include "z.h"
#include "mru.h"

#include "undo.h"

// the dec offsetof macro doesn't work very well...
#define myoffsetof(type,identifier) ((size_t)&((type *)0)->identifier)


// set these before calling CheckParm
extern int myargc;
extern char** myargv;

double I_FloatTime( void );

void	Error( char* error, ... );
void	Warning( char* error, ... );
int		CheckParm( char* check );
void ParseCommandLine( char* lpCmdLine );


int 	ParseNum( char* str );


char* COM_Parse( char* data );

extern	char		com_token[1024];
extern	bool	com_eof;

#define	MAX_NUM_ARGVS	32
extern	int		argc;
extern	char*	argv[MAX_NUM_ARGVS];

typedef int fileHandle_t;

typedef struct
{
    int		p1, p2;
    face_s*	f1, *f2;
} pedge_t;

typedef struct
{
    int		  iSize;
    int		  iTexMenu;		// nearest, linear, etc
    float	  fGamma;			// gamma for textures
    char	  szProject[256];	// last project loaded
    vec3_t	colors[COLOR_LAST];
    bool  show_names;
    bool  show_coordinates;
    int       exclude;
    int     m_nTextureTweak;
} SavedInfo_t;

//
// system functions
//
// TTimo NOTE: WINAPI funcs can be accessed by plugins
void    Sys_UpdateStatusBar( void );
void    WINAPI Sys_UpdateWindows( int bits );
void    Sys_Beep( void );
void    Sys_ClearPrintf( void );
void    Sys_Printf( char* text, ... );
double	Sys_DoubleTime( void );
void    Sys_GetCursorPos( int* x, int* y );
void    Sys_SetCursorPos( int x, int y );
void    Sys_SetTitle( char* text );
void    Sys_BeginWait( void );
void    Sys_EndWait( void );
void    Sys_Status( const char* psz, int part );

/*
** most of the QE globals are stored in this structure
*/
typedef struct
{
    bool d_showgrid;
    int      d_gridsize;
    
    int      d_num_entities;
    
    entity_s* d_project_entity;
    
    float     d_new_brush_bottom_z,
              d_new_brush_top_z;
              
    HINSTANCE d_hInstance;
    
    HGLRC     d_hglrcBase;
    HDC       d_hdcBase;
    
    HWND      d_hwndMain;
    HWND      d_hwndCamera;
    HWND      d_hwndEdit;
    HWND      d_hwndEntity;
    HWND      d_hwndTexture;
    HWND      d_hwndXY;
    HWND      d_hwndZ;
    HWND      d_hwndStatus;
    HWND      d_hwndMedia;
    
    edVec3_c  d_points[MAX_POINTS];
    int       d_numpoints;
    pedge_t   d_edges[MAX_EDGES];
    int       d_numedges;
    
    int       d_num_move_points;
    float*    d_move_points[4096];
    
    qtexture_t*	d_qtextures;
    
    texturewin_t d_texturewin;
    
    int	         d_pointfile_display_list;
    
    xy_t         d_xyOld;
    
    LPMRUMENU    d_lpMruMenu;
    
    SavedInfo_t  d_savedinfo;
    
    int          d_workcount;
    
    // connect entities uses the last two brushes selected
    int			 d_select_count;
    brush_s*		d_select_order[2];
    edVec3_c       d_select_translate;    // for dragging w/o making new display lists
    select_t     d_select_mode;
    
    int		     d_font_list;
    
    int          d_parsed_brushes;
    
    bool	show_blocks;
    
    // Timo
    // tells if we are internally using brush primitive (texture coordinates and map format)
    // this is a shortcut for IntForKey( g_qeglobals.d_project_entity, "brush_primit" )
    // NOTE: must keep the two ones in sync
    BOOL m_bBrushPrimitMode;
    
    // used while importing brush data from file or memory buffer
    // tells if conversion between map format and internal preferences ( m_bBrushPrimitMode ) is needed
    bool	bNeedConvert;
    bool	bOldBrushes;
    bool	bPrimitBrushes;
    
    edVec3_c  d_vAreaTL;
    edVec3_c  d_vAreaBR;
    
    // tells if we are using .INI files for prefs instead of registry
    bool	use_ini;
    // even in .INI mode we use the registry for all void* prefs
    char		use_ini_registry[64];
    
    bool dontDrawSelectedOutlines;
    
} QEGlobals_t;

//void *qmalloc (size_t size);
char* copystring( char* s );
char* ExpandReletivePath( char* p );

void Pointfile_Delete( void );
void WINAPI Pointfile_Check( void );
void Pointfile_Next( void );
void Pointfile_Prev( void );
void Pointfile_Clear( void );
void Pointfile_Draw( void );
void Pointfile_Load( void );

//
// drag.c
//
void Drag_Begin( int x, int y, int buttons,
                 vec3_t xaxis, vec3_t yaxis,
                 vec3_t origin, vec3_t dir );
void Drag_MouseMoved( int x, int y, int buttons );
void Drag_MouseUp( int nButtons = 0 );

//
// csg.c
//
void CSG_MakeHollow( void );
void CSG_Subtract( void );
void CSG_Merge( void );

//
// vertsel.c
//

void SetupVertexSelection( void );
void SelectEdgeByRay( const edVec3_c& org, const edVec3_c& dir );
void SelectVertexByRay( const edVec3_c& org, const edVec3_c& dir );

void ConnectEntities( void );

extern	int	update_bits;

extern	int	screen_width;
extern	int	screen_height;

extern	HANDLE	bsp_process;
extern HANDLE g_hBSPOutput;
extern HANDLE g_hBSPInput;


char*	TranslateString( char* buf );

void ProjectDialog( void );

void FillTextureMenu( CStringArray* pArray = NULL );
void FillBSPMenu( void );

BOOL CALLBACK Win_Dialog(
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
);


//
// win_cam.c
//
void WCam_Create( HINSTANCE hInstance );


//
// win_xy.c
//
void WXY_Create( HINSTANCE hInstance );

//
// win_z.c
//
void WZ_Create( HINSTANCE hInstance );

//
// win_ent.c
//


//
// win_main.c
//
void Main_Create( HINSTANCE hInstance );
extern BOOL SaveWindowState( HWND hWnd, const char* pszName );
extern BOOL LoadWindowState( HWND hWnd, const char* pszName );

extern BOOL SaveRegistryInfo( const char* pszName, void* pvBuf, long lSize );
extern BOOL LoadRegistryInfo( const char* pszName, void* pvBuf, long* plSize );

//
// entityw.c
//
BOOL CreateEntityWindow( HINSTANCE hInstance );
void FillClassList( void );
BOOL UpdateEntitySel( eclass_s* pec );
void SetInspectorMode( int iType );
int DrawTexControls( HWND hWnd );
void SetSpawnFlags( void );
void GetSpawnFlags( void );
void SetKeyValuePairs( bool bClearMD3 = false );
extern void BuildGammaTable( float g );
BOOL GetSelectAllCriteria( CString& strKey, CString& strVal );


// win_dlg.c

void DoGamma( void );
void DoFind( void );
void DoRotate( void );
void DoSides( bool bCone = false, bool bSphere = false, bool bTorus = false );
void DoAbout( void );
void DoSurface();

/*
** QE function declarations
*/
void     QE_CheckAutoSave( void );
void     WINAPI QE_ConvertDOSToUnixName( char* dst, const char* src );
void     QE_CountBrushesAndUpdateStatusBar( void );
void     WINAPI QE_CheckOpenGLForErrors( void );
void     QE_ExpandBspString( char* bspaction, char* out, char* mapname, bool useTemps );
void     QE_Init( void );
bool QE_KeyDown( int key, int nFlags = 0 );
bool QE_LoadProject( char* projectfile );
bool QE_SingleBrush( bool bQuiet = false );


// sys stuff
void Sys_MarkMapModified( void );

/*
** QE Win32 function declarations
*/
int  WINAPI QEW_SetupPixelFormat( HDC hDC, bool zbuffer );
void QEW_StopGL( HWND hWnd, HGLRC hGLRC, HDC hDC );

/*
** extern declarations
*/
extern QEGlobals_t   g_qeglobals;

//++timo clean (moved into qertypes.h)
//enum VIEWTYPE {YZ, XZ, XY};
bool IsBrushSelected( brush_s* bSel );

// curve brushes

void Curve_MakeCurvedBrush( bool negative, bool top, bool bottom,
                            bool s1, bool s2, bool s3, bool s4 );

void Curve_Invert( void );

void Curve_AddFakePlanes( brush_s* B );
void Curve_StripFakePlanes( brush_s* B );
void Curve_BuildPoints( brush_s* b );
void Curve_XYDraw( brush_s* b );
void Curve_CameraDraw( brush_s* b );

void Curve_WriteFile( char* name );


// patch stuff

extern bool g_bSameView;
extern int  g_nPatchClickedView;
bool within( vec3_t vTest, vec3_t vTL, vec3_t vBR );

void Brush_RebuildBrush( brush_s* b, vec3_t vMins, vec3_t vMaxs );
patchMesh_c* MakeNewPatch();
brush_s* AddBrushForPatch( patchMesh_c* pm, bool bLinkToWorld = true );
brush_s* Patch_GenericMesh( int nWidth, int nHeight, int nOrientation = 2, bool bDeleteSource = true, bool bOverride = false );
void Patch_ReadFile( char* name );
void Patch_WriteFile( char* name );
void Patch_BuildPoints( brush_s* b );
//void Patch_Move(patchMesh_c *p, const vec3_t vMove, bool bRebuild = false);
void Patch_ApplyMatrix( patchMesh_c* p, const class edVec3_c& vOrigin, const edVec3_c vMatrix[3], bool bSnap = false );
void Patch_EditPatch();
void Patch_Deselect();
void Patch_Deselect( patchMesh_c* p );
void Patch_Delete( patchMesh_c* p );
int  Patch_MemorySize( patchMesh_c* p );
void Patch_Select( patchMesh_c* p );
void Patch_Scale( patchMesh_c* p, const vec3_t vOrigin, const vec3_t vAmt, bool bRebuilt = true );
void Patch_Cleanup();
void Patch_SetView( int n );
void Patch_SetTexture( patchMesh_c* p, texdef_t* tex_def, IPluginTexdef* pPlugTexdef = NULL );
void Patch_BrushToMesh( bool bCone = false, bool bBevel = false, bool bEndcap = false, bool bSquare = false, int nHeight = 3 );
bool Patch_DragScale( patchMesh_c* p, const edVec3_c& vAmt, const edVec3_c& vMove );
void Patch_ReadBuffer( char* pBuff, bool bSelect = false );
void Patch_WriteFile( CMemFile* pMemFile );
void Patch_UpdateSelected( vec3_t vMove );
void Patch_AddRow( patchMesh_c* p );
brush_s* Patch_Parse( bool bOld );
void Patch_Write( patchMesh_c* p, FILE* f );
void Patch_Write( patchMesh_c* p, CMemFile* file );
//void Patch_AdjustColumns(patchMesh_c *p, int nCols);
//void Patch_AdjustRows(patchMesh_c *p, int nRows);
void Patch_AdjustSelected( bool bInsert, bool bColumn, bool bFlag );
patchMesh_c* Patch_Duplicate( patchMesh_c* pFrom );
void Patch_RotateTexture( patchMesh_c* p, float fAngle );
void Patch_ScaleTexture( patchMesh_c* p, float fx, float fy, bool bFixup = true );
void Patch_ShiftTexture( patchMesh_c* p, float fx, float fy );
//void Patch_DrawCam(patchMesh_c *p);
//void Patch_InsertColumn(patchMesh_c *p, bool bAdd);
//void Patch_InsertRow(patchMesh_c *p, bool bAdd);
//void Patch_RemoveRow(patchMesh_c *p, bool bFirst);
void Patch_ToggleInverted();
void Patch_Restore( patchMesh_c* p );
void Patch_Save( patchMesh_c* p );
void Patch_SetTextureInfo( texdef_t* pt );
void Patch_NaturalTexturing();
void Patch_ResetTexturing( float fx, float fy );
void Patch_FitTexturing();
void Patch_BendToggle();
void Patch_StartInsDel();
void Patch_BendHandleTAB();
void Patch_BendHandleENTER();
void Patch_SelectBendNormal();
void Patch_SelectBendAxis();
bool WINAPI OnlyPatchesSelected();
bool WINAPI AnyPatchesSelected();
patchMesh_c* SinglePatchSelected();
void Patch_CapCurrent( bool bInvertedBevel = false, bool bInvertedEndcap = false );
void Patch_DisperseRows();
void Patch_DisperseColumns();
void Patch_NaturalizeSelected( bool bCap = false, bool bCycleCap = false );
void Patch_SelectAreaPoints();
void Patch_InvertTexture( bool bY );
void Patch_InsDelToggle();
void Patch_InsDelHandleTAB();
void Patch_InsDelHandleENTER();
void Patch_SetOverlays();
void Patch_ClearOverlays();
void Patch_Thicken( int nAmount, bool bSeam );
void Patch_Transpose();
void Patch_Freeze();
void Patch_UnFreeze( bool bAll );
const char* Patch_GetTextureName();
void Patch_FindReplaceTexture( brush_s* pb, const char* pFind, const char* pReplace, bool bForce );
void Patch_ReplaceQTexture( brush_s* pb, qtexture_t* pOld, qtexture_t* pNew );
void Select_SnapToGrid();
extern bool g_bPatchShowBounds;
extern bool g_bPatchWireFrame;
extern bool g_bPatchWeld;
extern bool g_bPatchDrillDown;
extern bool g_bPatchInsertMode;
extern bool g_bPatchBendMode;
extern edVec3_c g_vBendOrigin;
void Patch_FromTriangle( vec5_t vx, vec5_t vy, vec5_t vz );

// Timo
// new brush primitive stuff
void ComputeAxisBase( edVec3_c& normal, edVec3_c& texS, edVec3_c& texT );
void FaceToBrushPrimitFace( face_s* f );
void EmitBrushPrimitTextureCoordinates( face_s*, winding_t* );
// EmitTextureCoordinates, is old code used for brush to brush primitive conversion
void EmitTextureCoordinates( class texturedVertex_c& out, qtexture_t* q, face_s* f );
void BrushPrimit_Parse( brush_s* );
// compute a fake shift scale rot representation from the texture matrix
void TexMatToFakeTexCoords( vec_t texMat[2][3], float shift[2], float* rot, float scale[2] );
void FakeTexCoordsToTexMat( float shift[2], float rot, float scale[2], vec_t texMat[2][3] );
void ConvertTexMatWithQTexture( brushprimit_texdef_s* texMat1, qtexture_t* qtex1, brushprimit_texdef_s* texMat2, qtexture_t* qtex2 );
// texture locking
void Face_MoveTexture_BrushPrimit( face_s* f, const edVec3_c& delta );
void Select_ShiftTexture_BrushPrimit( face_s* f, int x, int y );
void RotateFaceTexture_BrushPrimit( face_s* f, int nAxis, float fDeg, vec3_t vOrigin );
// used in CCamWnd::ShiftTexture_BrushPrimit
void ComputeBest2DVector( const edVec3_c& v, const edVec3_c& X, const edVec3_c& Y, int& x, int& y );


//
// eclass.cpp
//
extern bool parsing_single;
extern bool eclass_found;
extern eclass_s* eclass_e;
void Eclass_ScanFile( char* filename );

//
// ShaderInfo.cpp
//
#include "ShaderInfo.h"

//
// TexWnd.cpp
//
CShaderInfo* hasShader( const char* pName );

extern CStringArray g_BSPFrontendCommands;
//
// IShaders interface
CShaderInfo* SetNameShaderInfo( qtexture_t* q, const char* pPath, const char* pName );
qtexture_t* Texture_LoadTGATexture( unsigned char* pPixels, int nWidth, int nHeight, char* pPath, int nFlags, int nContents, int nValue );
qtexture_t* WINAPI QERApp_TryTextureForName( const char* name );



#endif
