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
//  File name:   sys_win32.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "sys_local.h"

#include <windows.h>
#include <lmerr.h>
#include <lmcons.h>
#include <lmwksta.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <conio.h>
#include <wincrypt.h>
#include <shlobj.h>
#include <psapi.h>
#include <float.h>

// Used to determine where to store user-specific files
static char homePath[ MAX_OSPATH ] = { 0 };

#ifndef DEDICATED
static UINT timerResolution = 0;
#endif

/*
================
Sys_SetFPUCW
Set FPU control word to default value
================
*/

#ifndef _RC_CHOP
// mingw doesn't seem to have these defined :(

#define _MCW_EM 0x0008001fU
#define _MCW_RC 0x00000300U
#define _MCW_PC 0x00030000U
#define _RC_NEAR      0x00000000U
#define _PC_53  0x00010000U

unsigned int _controlfp( unsigned int new, unsigned int mask );
#endif

#define FPUCWMASK1 (_MCW_RC | _MCW_EM)
#define FPUCW (_RC_NEAR | _MCW_EM | _PC_53)

#if idx64
#define FPUCWMASK   (FPUCWMASK1)
#else
#define FPUCWMASK   (FPUCWMASK1 | _MCW_PC)
#endif

void Sys_SetFloatEnv( void )
{
	_controlfp( FPUCW, FPUCWMASK );
}

/*
================
Sys_DefaultHomePath
================
*/
char* Sys_DefaultHomePath( void )
{
#if 1
	return 0;
#else
	TCHAR szPath[MAX_PATH];
	FARPROC qSHGetFolderPath;
	HMODULE shfolder = LoadLibrary( "shfolder.dll" );
	
	if ( !*homePath && com_homepath )
	{
		if ( shfolder == NULL )
		{
			Com_Printf( "Unable to load SHFolder.dll\n" );
			return NULL;
		}
	
		qSHGetFolderPath = GetProcAddress( shfolder, "SHGetFolderPathA" );
		if ( qSHGetFolderPath == NULL )
		{
			Com_Printf( "Unable to find SHGetFolderPath in SHFolder.dll\n" );
			FreeLibrary( shfolder );
			return NULL;
		}
	
		if ( !SUCCEEDED( qSHGetFolderPath( NULL, CSIDL_APPDATA,
										   NULL, 0, szPath ) ) )
		{
			Com_Printf( "Unable to detect CSIDL_APPDATA\n" );
			FreeLibrary( shfolder );
			return NULL;
		}
	
		Com_sprintf( homePath, sizeof( homePath ), "%s%c", szPath, PATH_SEP );
	
		if ( com_homepath->string[0] )
			Q_strcat( homePath, sizeof( homePath ), com_homepath->string );
		else
			Q_strcat( homePath, sizeof( homePath ), HOMEPATH_NAME_WIN );
	
		FreeLibrary( shfolder );
	}
	
	return homePath;
#endif
}

/*
================
Sys_Milliseconds
================
*/
int sys_timeBase;
int Sys_Milliseconds( void )
{
	int             sys_curtime;
	static bool initialized = false;
	
	if ( !initialized )
	{
		sys_timeBase = timeGetTime();
		initialized = true;
	}
	sys_curtime = timeGetTime() - sys_timeBase;
	
	return sys_curtime;
}

/*
================
Sys_RandomBytes
================
*/
bool Sys_RandomBytes( byte* string, int len )
{
	HCRYPTPROV  prov;
	
	if ( !CryptAcquireContext( &prov, NULL, NULL,
							   PROV_RSA_FULL, CRYPT_VERIFYCONTEXT ) )
	{
	
		return false;
	}
	
	if ( !CryptGenRandom( prov, len, ( BYTE* )string ) )
	{
		CryptReleaseContext( prov, 0 );
		return false;
	}
	CryptReleaseContext( prov, 0 );
	return true;
}

/*
================
Sys_GetCurrentUser
================
*/
char* Sys_GetCurrentUser( void )
{
	static char s_userName[1024];
	unsigned long size = sizeof( s_userName );
	
	if ( !GetUserName( s_userName, &size ) )
		strcpy( s_userName, "player" );
		
	if ( !s_userName[0] )
	{
		strcpy( s_userName, "player" );
	}
	
	return s_userName;
}

/*
================
Sys_GetClipboardData
================
*/
char* Sys_GetClipboardData( void )
{
	char* data = NULL;
	char* cliptext;
	
	if ( OpenClipboard( NULL ) != 0 )
	{
		HANDLE hClipboardData;
		
		if ( ( hClipboardData = GetClipboardData( CF_TEXT ) ) != 0 )
		{
			if ( ( cliptext = ( char* )GlobalLock( hClipboardData ) ) != 0 )
			{
				data = ( char* )malloc( GlobalSize( hClipboardData ) + 1 );
				Q_strncpyz( data, cliptext, GlobalSize( hClipboardData ) );
				GlobalUnlock( hClipboardData );
				
				strtok( data, "\n\r\b" );
			}
		}
		CloseClipboard();
	}
	return data;
}

#define MEM_THRESHOLD 96*1024*1024

/*
==================
Sys_LowPhysicalMemory
==================
*/
bool Sys_LowPhysicalMemory( void )
{
	MEMORYSTATUS stat;
	GlobalMemoryStatus( &stat );
	return ( stat.dwTotalPhys <= MEM_THRESHOLD ) ? true : false;
}

/*
==============
Sys_Basename
==============
*/
const char* Sys_Basename( char* path )
{
	static char base[ MAX_OSPATH ] = { 0 };
	int length;
	
	length = strlen( path ) - 1;
	
	// Skip trailing slashes
	while ( length > 0 && path[ length ] == '\\' )
		length--;
		
	while ( length > 0 && path[ length - 1 ] != '\\' )
		length--;
		
	Q_strncpyz( base, &path[ length ], sizeof( base ) );
	
	length = strlen( base ) - 1;
	
	// Strip trailing slashes
	while ( length > 0 && base[ length ] == '\\' )
		base[ length-- ] = '\0';
		
	return base;
}

/*
==============
Sys_Dirname
==============
*/
const char* Sys_Dirname( char* path )
{
	static char dir[ MAX_OSPATH ] = { 0 };
	int length;
	
	Q_strncpyz( dir, path, sizeof( dir ) );
	length = strlen( dir ) - 1;
	
	while ( length > 0 && dir[ length ] != '\\' )
		length--;
		
	dir[ length ] = '\0';
	
	return dir;
}

/*
==============
Sys_Mkdir
==============
*/
bool Sys_Mkdir( const char* path )
{
	if ( !CreateDirectory( path, NULL ) )
	{
		if ( GetLastError( ) != ERROR_ALREADY_EXISTS )
			return false;
	}
	
	return true;
}

/*
==================
Sys_Mkfifo
Noop on windows because named pipes do not function the same way
==================
*/
FILE* Sys_Mkfifo( const char* ospath )
{
	return NULL;
}

/*
==============
Sys_Cwd
==============
*/
char* Sys_Cwd( void )
{
	static char cwd[MAX_OSPATH];
	
	_getcwd( cwd, sizeof( cwd ) - 1 );
	cwd[MAX_OSPATH - 1] = 0;
	
	return cwd;
}

/*
==============================================================

DIRECTORY SCANNING

==============================================================
*/

#define MAX_FOUND_FILES 0x1000

/*
==============
Sys_ListFilteredFiles
==============
*/
void Sys_ListFilteredFiles( const char* basedir, char* subdirs, const char* filter, char** list, int* numfiles )
{
	char        search[MAX_OSPATH], newsubdirs[MAX_OSPATH];
	char        filename[MAX_OSPATH];
	intptr_t    findhandle;
	struct _finddata_t findinfo;
	
	if ( *numfiles >= MAX_FOUND_FILES - 1 )
	{
		return;
	}
	
	if ( strlen( subdirs ) )
	{
		Com_sprintf( search, sizeof( search ), "%s\\%s\\*", basedir, subdirs );
	}
	else
	{
		Com_sprintf( search, sizeof( search ), "%s\\*", basedir );
	}
	
	findhandle = _findfirst( search, &findinfo );
	if ( findhandle == -1 )
	{
		return;
	}
	
	do
	{
		if ( findinfo.attrib & _A_SUBDIR )
		{
			if ( Q_stricmp( findinfo.name, "." ) && Q_stricmp( findinfo.name, ".." ) )
			{
				if ( strlen( subdirs ) )
				{
					Com_sprintf( newsubdirs, sizeof( newsubdirs ), "%s\\%s", subdirs, findinfo.name );
				}
				else
				{
					Com_sprintf( newsubdirs, sizeof( newsubdirs ), "%s", findinfo.name );
				}
				Sys_ListFilteredFiles( basedir, newsubdirs, filter, list, numfiles );
			}
		}
		if ( *numfiles >= MAX_FOUND_FILES - 1 )
		{
			break;
		}
		Com_sprintf( filename, sizeof( filename ), "%s\\%s", subdirs, findinfo.name );
		if ( !Com_FilterPath( filter, filename, false ) )
			continue;
		list[ *numfiles ] = strdup( filename );
		( *numfiles )++;
	}
	while ( _findnext( findhandle, &findinfo ) != -1 );
	
	_findclose( findhandle );
}

/*
==============
strgtr
==============
*/
static bool strgtr( const char* s0, const char* s1 )
{
	int l0, l1, i;
	
	l0 = strlen( s0 );
	l1 = strlen( s1 );
	
	if ( l1 < l0 )
	{
		l0 = l1;
	}
	
	for ( i = 0; i < l0; i++ )
	{
		if ( s1[i] > s0[i] )
		{
			return true;
		}
		if ( s1[i] < s0[i] )
		{
			return false;
		}
	}
	return false;
}

void ListFilesIn( const char* dir, const char* ext, int& nfiles, char* list[MAX_FOUND_FILES], int baseDirLen )
{
	char        search[MAX_OSPATH];
	struct _finddata_t findinfo;
	intptr_t        findhandle;
	
	Com_sprintf( search, sizeof( search ), "%s\\*%s", dir, ext );
	
	// search
	findhandle = _findfirst( search, &findinfo );
	if ( findhandle == -1 )
	{
		return;
	}
	
	do
	{
		if ( nfiles == MAX_FOUND_FILES - 1 )
		{
			break;
		}
		if ( baseDirLen )
		{
			char fullPath[MAX_OSPATH];
			Com_sprintf( fullPath, sizeof( fullPath ), "%s\\%s", dir, findinfo.name );
			list[ nfiles ] = strdup( fullPath + baseDirLen + 1 );
		}
		else
		{
			list[ nfiles ] = strdup( findinfo.name );
		}
		nfiles++;
	}
	while ( _findnext( findhandle, &findinfo ) != -1 );
	
	_findclose( findhandle );
	
}
/*
==============
Sys_ListFiles
==============
*/
char** Sys_ListFiles( const char* directory, const char* extension, const char* filter, int* numfiles, bool wantsubs )
{
	char        search[MAX_OSPATH];
	int         nfiles;
	char**      listCopy;
	char*       list[MAX_FOUND_FILES];
	struct _finddata_t findinfo;
	intptr_t        findhandle;
	int         flag;
	int         i;
	
	if ( filter )
	{
	
		nfiles = 0;
		Sys_ListFilteredFiles( directory, "", filter, list, &nfiles );
		
		list[ nfiles ] = 0;
		*numfiles = nfiles;
		
		if ( !nfiles )
			return NULL;
			
		listCopy = ( char** )malloc( ( nfiles + 1 ) * sizeof( *listCopy ) );
		for ( i = 0 ; i < nfiles ; i++ )
		{
			listCopy[i] = list[i];
		}
		listCopy[i] = NULL;
		
		return listCopy;
	}
	
	if ( !extension )
	{
		extension = "";
	}
	
	nfiles = 0;
	ListFilesIn( directory, extension, nfiles, list, 0 );
	
	
	// check subdirs
	Com_sprintf( search, sizeof( search ), "%s\\*", directory );
	findhandle = _findfirst( search, &findinfo );
	if ( findhandle != -1 )
	{
		do
		{
			if ( findinfo.attrib & _A_SUBDIR )
			{
				if ( findinfo.name[0] != '.' )
				{
					//printf("subdir: %s\n",findinfo.name);
					char newDir[MAX_OSPATH];
					strcpy( newDir, directory );
					strcat( newDir, "\\" );
					strcat( newDir, findinfo.name );
					ListFilesIn( newDir, extension, nfiles, list, strlen( directory ) );
				}
			}
		}
		while ( _findnext( findhandle, &findinfo ) != -1 );
		_findclose( findhandle );
	}
	
	
	
	// return a copy of the list
	*numfiles = nfiles;
	
	if ( !nfiles )
	{
		return NULL;
	}
	
	listCopy = ( char** )malloc( ( nfiles + 1 ) * sizeof( *listCopy ) );
	for ( i = 0 ; i < nfiles ; i++ )
	{
		listCopy[i] = list[i];
	}
	listCopy[i] = NULL;
	
	do
	{
		flag = 0;
		for ( i = 1; i < nfiles; i++ )
		{
			if ( strgtr( listCopy[i - 1], listCopy[i] ) )
			{
				char* temp = listCopy[i];
				listCopy[i] = listCopy[i - 1];
				listCopy[i - 1] = temp;
				flag = 1;
			}
		}
	}
	while ( flag );
	
	return listCopy;
}

/*
==============
Sys_FreeFileList
==============
*/
void Sys_FreeFileList( char** list )
{
	int i;
	
	if ( !list )
	{
		return;
	}
	
	for ( i = 0 ; list[i] ; i++ )
	{
		free( list[i] );
	}
	
	free( list );
}


/*
==============
Sys_Sleep

Block execution for msec or until input is received.
==============
*/
void Sys_Sleep( int msec )
{
	if ( msec == 0 )
		return;
		
#ifdef DEDICATED
	if ( msec < 0 )
		WaitForSingleObject( GetStdHandle( STD_INPUT_HANDLE ), INFINITE );
	else
		WaitForSingleObject( GetStdHandle( STD_INPUT_HANDLE ), msec );
#else
	// Client Sys_Sleep doesn't support waiting on stdin
	if ( msec < 0 )
		return;
		
	Sleep( msec );
#endif
}

/*
==============
Sys_ErrorDialog

Display an error message
==============
*/
void Sys_ErrorDialog( const char* error )
{
	if ( Sys_Dialog( DT_YES_NO, va( "%s. Copy console log to clipboard?", error ),
					 "Error" ) == DR_YES )
	{
		HGLOBAL memoryHandle;
		char* clipMemory;
		
		memoryHandle = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, CON_LogSize( ) + 1 );
		clipMemory = ( char* )GlobalLock( memoryHandle );
		
		if ( clipMemory )
		{
			char* p = clipMemory;
			char buffer[ 1024 ];
			unsigned int size;
			
			while ( ( size = CON_LogRead( buffer, sizeof( buffer ) ) ) > 0 )
			{
				memcpy( p, buffer, size );
				p += size;
			}
			
			*p = '\0';
			
			if ( OpenClipboard( NULL ) && EmptyClipboard( ) )
				SetClipboardData( CF_TEXT, memoryHandle );
				
			GlobalUnlock( clipMemory );
			CloseClipboard( );
		}
	}
}

/*
==============
Sys_Dialog

Display a win32 dialog box
==============
*/
dialogResult_t Sys_Dialog( dialogType_t type, const char* message, const char* title )
{
	UINT uType;
	
	switch ( type )
	{
		default:
		case DT_INFO:
			uType = MB_ICONINFORMATION | MB_OK;
			break;
		case DT_WARNING:
			uType = MB_ICONWARNING | MB_OK;
			break;
		case DT_ERROR:
			uType = MB_ICONERROR | MB_OK;
			break;
		case DT_YES_NO:
			uType = MB_ICONQUESTION | MB_YESNO;
			break;
		case DT_OK_CANCEL:
			uType = MB_ICONWARNING | MB_OKCANCEL;
			break;
	}
	
	switch ( MessageBox( NULL, message, title, uType ) )
	{
		default:
		case IDOK:
			return DR_OK;
		case IDCANCEL:
			return DR_CANCEL;
		case IDYES:
			return DR_YES;
		case IDNO:
			return DR_NO;
	}
}

#ifndef DEDICATED
static bool SDL_VIDEODRIVER_externallySet = false;
#endif

/*
==============
Sys_GLimpSafeInit

Windows specific "safe" GL implementation initialisation
==============
*/
void Sys_GLimpSafeInit( void )
{
#ifndef DEDICATED
	if ( !SDL_VIDEODRIVER_externallySet )
	{
		// Here, we want to let SDL decide what do to unless
		// explicitly requested otherwise
		_putenv( "SDL_VIDEODRIVER=" );
	}
#endif
}

/*
==============
Sys_GLimpInit

Windows specific GL implementation initialisation
==============
*/
void Sys_GLimpInit( void )
{
#ifndef DEDICATED
	if ( !SDL_VIDEODRIVER_externallySet )
	{
		// It's a little bit weird having in_mouse control the
		// video driver, but from ioq3's point of view they're
		// virtually the same except for the mouse input anyway
		if ( Cvar_VariableIntegerValue( "in_mouse" ) == -1 )
		{
			// Use the windib SDL backend, which is closest to
			// the behaviour of idq3 with in_mouse set to -1
			_putenv( "SDL_VIDEODRIVER=windib" );
		}
		else
		{
			// Use the DirectX SDL backend
			_putenv( "SDL_VIDEODRIVER=directx" );
		}
	}
#endif
}

/*
==============
Sys_PlatformInit

Windows specific initialisation
==============
*/
void Sys_PlatformInit( void )
{
#ifndef DEDICATED
	TIMECAPS ptc;
	const char* SDL_VIDEODRIVER = getenv( "SDL_VIDEODRIVER" );
#endif
	
	Sys_SetFloatEnv();
	
#ifndef DEDICATED
	if ( SDL_VIDEODRIVER )
	{
		Com_Printf( "SDL_VIDEODRIVER is externally set to \"%s\", "
					"in_mouse -1 will have no effect\n", SDL_VIDEODRIVER );
		SDL_VIDEODRIVER_externallySet = true;
	}
	else
		SDL_VIDEODRIVER_externallySet = false;
		
	if ( timeGetDevCaps( &ptc, sizeof( ptc ) ) == MMSYSERR_NOERROR )
	{
		timerResolution = ptc.wPeriodMin;
		
		if ( timerResolution > 1 )
		{
			Com_Printf( "Warning: Minimum supported timer resolution is %ums "
						"on this system, recommended resolution 1ms\n", timerResolution );
		}
		
		timeBeginPeriod( timerResolution );
	}
	else
		timerResolution = 0;
#endif
}

/*
==============
Sys_PlatformExit

Windows specific initialisation
==============
*/
void Sys_PlatformExit( void )
{
#ifndef DEDICATED
	if ( timerResolution )
		timeEndPeriod( timerResolution );
#endif
}

/*
==============
Sys_SetEnv

set/unset environment variables (empty value removes it)
==============
*/
void Sys_SetEnv( const char* name, const char* value )
{
	if ( value )
		_putenv( va( "%s=%s", name, value ) );
	else
		_putenv( va( "%s=", name ) );
}

/*
==============
Sys_PID
==============
*/
int Sys_PID( void )
{
	return GetCurrentProcessId( );
}

/*
==============
Sys_PIDIsRunning
==============
*/
bool Sys_PIDIsRunning( int pid )
{
	DWORD processes[ 1024 ];
	DWORD numBytes, numProcesses;
	int i;
	
	if ( !EnumProcesses( processes, sizeof( processes ), &numBytes ) )
		return false; // Assume it's not running
		
	numProcesses = numBytes / sizeof( DWORD );
	
	// Search for the pid
	for ( i = 0; i < numProcesses; i++ )
	{
		if ( processes[ i ] == pid )
			return true;
	}
	
	return false;
}
