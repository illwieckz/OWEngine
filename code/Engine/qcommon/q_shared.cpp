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
//  File name:   q_shared.cpp
//  Version:     v1.01
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
//  09-16-2015 : Cleaned and removed unused things
//
////////////////////////////////////////////////////////////////////////////

#include "q_shared.h"
#include <api/coreAPI.h>
#include <shared/colorTable.h>

/*
============
COM_SkipPath
============
*/
char* COM_SkipPath( char* pathname )
{
	char*   last;
	
	last = pathname;
	while ( *pathname )
	{
		if ( *pathname == '/' )
			last = pathname + 1;
		pathname++;
	}
	return last;
}

/*
============
COM_StripExtension
============
*/
void COM_StripExtension( const char* in, char* out, int destsize )
{
	const char* dot = strrchr( in, '.' ), *slash;
	if ( dot && ( !( slash = strrchr( in, '/' ) ) || slash < dot ) )
		Q_strncpyz( out, in, ( destsize < dot - in + 1 ? destsize : dot - in + 1 ) );
	else
		Q_strncpyz( out, in, destsize );
}

/*
============
COM_CompareExtension

string compare the end of the strings and return true if strings match
============
*/
bool COM_CompareExtension( const char* in, const char* ext )
{
	int inlen, extlen;
	
	inlen = strlen( in );
	extlen = strlen( ext );
	
	if ( extlen <= inlen )
	{
		in += inlen - extlen;
		
		if ( !Q_stricmp( in, ext ) )
			return true;
	}
	
	return false;
}

/*
==================
COM_DefaultExtension

if path doesn't have an extension, then append
 the specified one (which should include the .)
==================
*/
void COM_DefaultExtension( char* path, int maxSize, const char* extension )
{
	const char* dot = strrchr( path, '.' ), *slash;
	if ( dot && ( !( slash = strrchr( path, '/' ) ) || slash < dot ) )
		return;
	else
		Q_strcat( path, maxSize, extension );
}

/*
============================================================================

                    BYTE ORDER FUNCTIONS

============================================================================
*/

void CopyShortSwap( void* dest, void* src )
{
	byte* to = ( byte* )dest, *from = ( byte* )src;
	
	to[0] = from[1];
	to[1] = from[0];
}

void CopyLongSwap( void* dest, void* src )
{
	byte* to = ( byte* )dest, *from = ( byte* )src;
	
	to[0] = from[3];
	to[1] = from[2];
	to[2] = from[1];
	to[3] = from[0];
}

short   ShortSwap( short l )
{
	byte    b1, b2;
	
	b1 = l & 255;
	b2 = ( l >> 8 ) & 255;
	
	return ( b1 << 8 ) + b2;
}

short   ShortNoSwap( short l )
{
	return l;
}

int    LongSwap( int l )
{
	byte    b1, b2, b3, b4;
	
	b1 = l & 255;
	b2 = ( l >> 8 ) & 255;
	b3 = ( l >> 16 ) & 255;
	b4 = ( l >> 24 ) & 255;
	
	return ( ( int )b1 << 24 ) + ( ( int )b2 << 16 ) + ( ( int )b3 << 8 ) + b4;
}

int LongNoSwap( int l )
{
	return l;
}

float FloatSwap( const float* f )
{
	floatInt_u out;
	
	out.f = *f;
	out.ui = LongSwap( out.ui );
	
	return out.f;
}

float FloatNoSwap( const float* f )
{
	return *f;
}

/*
============================================================================

PARSING

============================================================================
*/

static  char    com_token[MAX_TOKEN_CHARS];
static  char    com_parsename[MAX_TOKEN_CHARS];
static  int     com_lines;

void COM_BeginParseSession( const char* name )
{
	com_lines = 0;
	Com_sprintf( com_parsename, sizeof( com_parsename ), "%s", name );
}

char* COM_Parse( char** data_p )
{
	return COM_ParseExt( data_p, true );
}

/*
==============
COM_Parse

Parse a token out of a string
Will never return NULL, just empty strings

If "allowLineBreaks" is true then an empty
string will be returned if the next token is
a newline.
==============
*/
static char* SkipWhitespace( char* data, bool* hasNewLines )
{
	int c;
	
	while ( ( c = *data ) <= ' ' )
	{
		if ( !c )
		{
			return NULL;
		}
		if ( c == '\n' )
		{
			com_lines++;
			*hasNewLines = true;
		}
		data++;
	}
	
	return data;
}

char* COM_ParseExt( char** data_p, bool allowLineBreaks )
{
	int c = 0, len;
	bool hasNewLines = false;
	char* data;
	
	data = *data_p;
	len = 0;
	com_token[0] = 0;
	
	// make sure incoming data is valid
	if ( !data )
	{
		*data_p = NULL;
		return com_token;
	}
	
	while ( 1 )
	{
		// skip whitespace
		data = SkipWhitespace( data, &hasNewLines );
		if ( !data )
		{
			*data_p = NULL;
			return com_token;
		}
		if ( hasNewLines && !allowLineBreaks )
		{
			*data_p = data;
			return com_token;
		}
		
		c = *data;
		
		// skip double slash comments
		if ( c == '/' && data[1] == '/' )
		{
			data += 2;
			while ( *data && *data != '\n' )
			{
				data++;
			}
		}
		// skip /* */ comments
		else if ( c == '/' && data[1] == '*' )
		{
			data += 2;
			while ( *data && ( *data != '*' || data[1] != '/' ) )
			{
				data++;
			}
			if ( *data )
			{
				data += 2;
			}
		}
		else
		{
			break;
		}
	}
	
	// handle quoted strings
	if ( c == '\"' )
	{
		data++;
		while ( 1 )
		{
			c = *data++;
			if ( c == '\"' || !c )
			{
				com_token[len] = 0;
				*data_p = ( char* ) data;
				return com_token;
			}
			if ( len < MAX_TOKEN_CHARS - 1 )
			{
				com_token[len] = c;
				len++;
			}
		}
	}
	
	// parse a regular word
	do
	{
		if ( len < MAX_TOKEN_CHARS - 1 )
		{
			com_token[len] = c;
			len++;
		}
		data++;
		c = *data;
		if ( c == '\n' )
			com_lines++;
	}
	while ( c > 32 );
	
	com_token[len] = 0;
	
	*data_p = ( char* ) data;
	return com_token;
}

/*
===================
Com_HexStrToInt
===================
*/
int Com_HexStrToInt( const char* str )
{
	if ( !str || !str[ 0 ] )
		return -1;
		
	// check for hex code
	if ( str[ 0 ] == '0' && str[ 1 ] == 'x' )
	{
		int i, n = 0;
		
		for ( i = 2; i < strlen( str ); i++ )
		{
			char digit;
			
			n *= 16;
			
			digit = tolower( str[ i ] );
			
			if ( digit >= '0' && digit <= '9' )
				digit -= '0';
			else if ( digit >= 'a' && digit <= 'f' )
				digit = digit - 'a' + 10;
			else
				return -1;
				
			n += digit;
		}
		
		return n;
	}
	
	return -1;
}

/*
============================================================================

                    LIBRARY REPLACEMENT FUNCTIONS

============================================================================
*/

bool Q_isanumber( const char* s )
{
	char* p;
	double d;
	
	if ( *s == '\0' )
		return false;
		
	d = strtod( s, &p );
	
	return *p == '\0';
}

bool Q_isintegral( float f )
{
	return ( int )f == f;
}

#ifdef _MSC_VER
/*
=============
Q_vsnprintf

Special wrapper function for Microsoft's broken _vsnprintf() function.
MinGW comes with its own snprintf() which is not broken.
=============
*/

int Q_vsnprintf( char* str, size_t size, const char* format, va_list ap )
{
	int retval;
	
	retval = _vsnprintf( str, size, format, ap );
	
	if ( retval < 0 || retval == size )
	{
		// Microsoft doesn't adhere to the C99 standard of vsnprintf,
		// which states that the return value must be the number of
		// bytes written if the output string had sufficient length.
		//
		// Obviously we cannot determine that value from Microsoft's
		// implementation, so we have no choice but to return size.
		
		str[size - 1] = '\0';
		return size;
	}
	
	return retval;
}
#endif

/*
=============
Q_strncpyz

Safe strncpy that ensures a trailing zero
=============
*/
void Q_strncpyz( char* dest, const char* src, int destsize )
{
	if ( !dest )
	{
		g_core->Error( ERR_FATAL, "Q_strncpyz: NULL dest" );
	}
	if ( !src )
	{
		g_core->Error( ERR_FATAL, "Q_strncpyz: NULL src" );
	}
	if ( destsize < 1 )
	{
		g_core->Error( ERR_FATAL, "Q_strncpyz: destsize < 1" );
	}
	
	strncpy( dest, src, destsize - 1 );
	dest[destsize - 1] = 0;
}

int Q_stricmpn( const char* s1, const char* s2, int n )
{
	int     c1, c2;
	
	if ( s1 == NULL )
	{
		if ( s2 == NULL )
			return 0;
		else
			return -1;
	}
	else if ( s2 == NULL )
		return 1;
		
		
		
	do
	{
		c1 = *s1++;
		c2 = *s2++;
		
		if ( !n-- )
		{
			return 0;       // strings are equal until end point
		}
		
		if ( c1 != c2 )
		{
			if ( c1 >= 'a' && c1 <= 'z' )
			{
				c1 -= ( 'a' - 'A' );
			}
			if ( c2 >= 'a' && c2 <= 'z' )
			{
				c2 -= ( 'a' - 'A' );
			}
			if ( c1 != c2 )
			{
				return c1 < c2 ? -1 : 1;
			}
		}
	}
	while ( c1 );
	
	return 0;       // strings are equal
}

int Q_strncmp( const char* s1, const char* s2, int n )
{
	int     c1, c2;
	
	do
	{
		c1 = *s1++;
		c2 = *s2++;
		
		if ( !n-- )
		{
			return 0;       // strings are equal until end point
		}
		
		if ( c1 != c2 )
		{
			return c1 < c2 ? -1 : 1;
		}
	}
	while ( c1 );
	
	return 0;       // strings are equal
}

int Q_stricmp( const char* s1, const char* s2 )
{
	return ( s1 && s2 ) ? Q_stricmpn( s1, s2, 99999 ) : -1;
}


char* Q_strlwr( char* s1 )
{
	char*   s;
	
	s = s1;
	while ( *s )
	{
		*s = tolower( *s );
		s++;
	}
	return s1;
}

char* Q_strupr( char* s1 )
{
	char*   s;
	
	s = s1;
	while ( *s )
	{
		*s = toupper( *s );
		s++;
	}
	return s1;
}


// never goes past bounds or leaves without a terminating 0
void Q_strcat( char* dest, int size, const char* src )
{
	int     l1;
	
	l1 = strlen( dest );
	if ( l1 >= size )
	{
		g_core->Error( ERR_FATAL, "Q_strcat: already overflowed" );
	}
	Q_strncpyz( dest + l1, src, size - l1 );
}

/*
* Find the first occurrence of find in s.
*/
const char* Q_stristr( const char* s, const char* find )
{
	char c, sc;
	size_t len;
	
	if ( ( c = *find++ ) != 0 )
	{
		if ( c >= 'a' && c <= 'z' )
		{
			c -= ( 'a' - 'A' );
		}
		len = strlen( find );
		do
		{
			do
			{
				if ( ( sc = *s++ ) == 0 )
					return NULL;
				if ( sc >= 'a' && sc <= 'z' )
				{
					sc -= ( 'a' - 'A' );
				}
			}
			while ( sc != c );
		}
		while ( Q_stricmpn( s, find, len ) != 0 );
		s--;
	}
	return s;
}


int Q_PrintStrlen( const char* string )
{
	int         len;
	const char* p;
	
	if ( !string )
	{
		return 0;
	}
	
	len = 0;
	p = string;
	while ( *p )
	{
		if ( Q_IsColorString( p ) )
		{
			p += 2;
			continue;
		}
		p++;
		len++;
	}
	
	return len;
}


char* Q_CleanStr( char* string )
{
	char*   d;
	char*   s;
	int     c;
	
	s = string;
	d = string;
	while ( ( c = *s ) != 0 )
	{
		if ( Q_IsColorString( s ) )
		{
			s++;
		}
		else if ( c >= 0x20 && c <= 0x7E )
		{
			*d++ = c;
		}
		s++;
	}
	*d = '\0';
	
	return string;
}

int Q_CountChar( const char* string, char tocount )
{
	int count;
	
	for ( count = 0; *string; string++ )
	{
		if ( *string == tocount )
			count++;
	}
	
	return count;
}

int QDECL Com_sprintf( char* dest, int size, const char* fmt, ... )
{
	int     len;
	va_list     argptr;
	
	va_start( argptr, fmt );
	len = Q_vsnprintf( dest, size, fmt, argptr );
	va_end( argptr );
	
	if ( len >= size )
		g_core->Print( "Com_sprintf: Output length %d too short, require %d bytes.\n", size, len + 1 );
		
	return len;
}

/*
============
va

does a varargs printf into a temp buffer, so I don't need to have
varargs versions of all text functions.
============
*/
char*    QDECL va( char* format, ... )
{
	va_list     argptr;
	static char string[2][32000]; // in case va is called by nested functions
	static int  index = 0;
	char*       buf;
	
	buf = string[index & 1];
	index++;
	
	va_start( argptr, format );
	Q_vsnprintf( buf, sizeof( *string ), format, argptr );
	va_end( argptr );
	
	return buf;
}

/*
============
Com_TruncateLongString

Assumes buffer is atleast TRUNCATE_LENGTH big
============
*/
void Com_TruncateLongString( char* buffer, const char* s )
{
	int length = strlen( s );
	
	if ( length <= TRUNCATE_LENGTH )
		Q_strncpyz( buffer, s, TRUNCATE_LENGTH );
	else
	{
		Q_strncpyz( buffer, s, ( TRUNCATE_LENGTH / 2 ) - 3 );
		Q_strcat( buffer, TRUNCATE_LENGTH, " ... " );
		Q_strcat( buffer, TRUNCATE_LENGTH, s + length - ( TRUNCATE_LENGTH / 2 ) + 3 );
	}
}

/*
=====================================================================

  INFO STRINGS

=====================================================================
*/

/*
===============
Info_ValueForKey

Searches the string for the given
key and returns the associated value, or an empty string.
FIXME: overflow check?
===============
*/
char* Info_ValueForKey( const char* s, const char* key )
{
	char    pkey[BIG_INFO_KEY];
	static  char value[2][BIG_INFO_VALUE];  // use two buffers so compares
	// work without stomping on each other
	static  int valueindex = 0;
	char*   o;
	
	if ( !s || !key )
	{
		return "";
	}
	
	if ( strlen( s ) >= BIG_INFO_STRING )
	{
		g_core->Error( ERR_DROP, "Info_ValueForKey: oversize infostring" );
	}
	
	valueindex ^= 1;
	if ( *s == '\\' )
		s++;
	while ( 1 )
	{
		o = pkey;
		while ( *s != '\\' )
		{
			if ( !*s )
				return "";
			*o++ = *s++;
		}
		*o = 0;
		s++;
		
		o = value[valueindex];
		
		while ( *s != '\\' && *s )
		{
			*o++ = *s++;
		}
		*o = 0;
		
		if ( !Q_stricmp( key, pkey ) )
			return value[valueindex];
			
		if ( !*s )
			break;
		s++;
	}
	
	return "";
}


/*
===================
Info_NextPair

Used to itterate through all the key/value pairs in an info string
===================
*/
void Info_NextPair( const char** head, char* key, char* value )
{
	char*   o;
	const char* s;
	
	s = *head;
	
	if ( *s == '\\' )
	{
		s++;
	}
	key[0] = 0;
	value[0] = 0;
	
	o = key;
	while ( *s != '\\' )
	{
		if ( !*s )
		{
			*o = 0;
			*head = s;
			return;
		}
		*o++ = *s++;
	}
	*o = 0;
	s++;
	
	o = value;
	while ( *s != '\\' && *s )
	{
		*o++ = *s++;
	}
	*o = 0;
	
	*head = s;
}


/*
===================
Info_RemoveKey
===================
*/
void Info_RemoveKey( char* s, const char* key )
{
	char*   start;
	char    pkey[MAX_INFO_KEY];
	char    value[MAX_INFO_VALUE];
	char*   o;
	
	if ( strlen( s ) >= MAX_INFO_STRING )
	{
		g_core->Error( ERR_DROP, "Info_RemoveKey: oversize infostring" );
	}
	
	if ( strchr( key, '\\' ) )
	{
		return;
	}
	
	while ( 1 )
	{
		start = s;
		if ( *s == '\\' )
			s++;
		o = pkey;
		while ( *s != '\\' )
		{
			if ( !*s )
				return;
			*o++ = *s++;
		}
		*o = 0;
		s++;
		
		o = value;
		while ( *s != '\\' && *s )
		{
			if ( !*s )
				return;
			*o++ = *s++;
		}
		*o = 0;
		
		if ( !strcmp( key, pkey ) )
		{
			memmove( start, s, strlen( s ) + 1 ); // remove this part
			
			return;
		}
		
		if ( !*s )
			return;
	}
	
}

/*
===================
Info_RemoveKey_Big
===================
*/
void Info_RemoveKey_Big( char* s, const char* key )
{
	char*   start;
	char    pkey[BIG_INFO_KEY];
	char    value[BIG_INFO_VALUE];
	char*   o;
	
	if ( strlen( s ) >= BIG_INFO_STRING )
	{
		g_core->Error( ERR_DROP, "Info_RemoveKey_Big: oversize infostring" );
	}
	
	if ( strchr( key, '\\' ) )
	{
		return;
	}
	
	while ( 1 )
	{
		start = s;
		if ( *s == '\\' )
			s++;
		o = pkey;
		while ( *s != '\\' )
		{
			if ( !*s )
				return;
			*o++ = *s++;
		}
		*o = 0;
		s++;
		
		o = value;
		while ( *s != '\\' && *s )
		{
			if ( !*s )
				return;
			*o++ = *s++;
		}
		*o = 0;
		
		if ( !strcmp( key, pkey ) )
		{
			strcpy( start, s ); // remove this part
			return;
		}
		
		if ( !*s )
			return;
	}
	
}




/*
==================
Info_Validate

Some characters are illegal in info strings because they
can mess up the server's parsing
==================
*/
bool Info_Validate( const char* s )
{
	if ( strchr( s, '\"' ) )
	{
		return false;
	}
	if ( strchr( s, ';' ) )
	{
		return false;
	}
	return true;
}

/*
==================
Info_SetValueForKey

Changes or adds a key/value pair
==================
*/
void Info_SetValueForKey( char* s, const char* key, const char* value )
{
	char    newi[MAX_INFO_STRING];
	const char* blacklist = "\\;\"";
	
	if ( strlen( s ) >= MAX_INFO_STRING )
	{
		g_core->Error( ERR_DROP, "Info_SetValueForKey: oversize infostring" );
	}
	
	for ( ; *blacklist; ++blacklist )
	{
		if ( strchr( key, *blacklist ) || strchr( value, *blacklist ) )
		{
			g_core->Print( S_COLOR_YELLOW "Can't use keys or values with a '%c': %s = %s\n", *blacklist, key, value );
			return;
		}
	}
	
	Info_RemoveKey( s, key );
	if ( !value || !strlen( value ) )
		return;
		
	Com_sprintf( newi, sizeof( newi ), "\\%s\\%s", key, value );
	
	if ( strlen( newi ) + strlen( s ) >= MAX_INFO_STRING )
	{
		g_core->Print( "Info string length exceeded\n" );
		return;
	}
	
	strcat( newi, s );
	strcpy( s, newi );
}

/*
==================
Info_SetValueForKey_Big

Changes or adds a key/value pair
Includes and retains zero-length values
==================
*/
void Info_SetValueForKey_Big( char* s, const char* key, const char* value )
{
	char    newi[BIG_INFO_STRING];
	const char* blacklist = "\\;\"";
	
	if ( strlen( s ) >= BIG_INFO_STRING )
	{
		g_core->Error( ERR_DROP, "Info_SetValueForKey: oversize infostring" );
	}
	
	for ( ; *blacklist; ++blacklist )
	{
		if ( strchr( key, *blacklist ) || strchr( value, *blacklist ) )
		{
			g_core->Print( S_COLOR_YELLOW "Can't use keys or values with a '%c': %s = %s\n", *blacklist, key, value );
			return;
		}
	}
	
	Info_RemoveKey_Big( s, key );
	if ( !value )
		return;
		
	Com_sprintf( newi, sizeof( newi ), "\\%s\\%s", key, value );
	
	if ( strlen( newi ) + strlen( s ) >= BIG_INFO_STRING )
	{
		g_core->Print( "BIG Info string length exceeded\n" );
		return;
	}
	
	strcat( s, newi );
}




//====================================================================

/*
==================
Com_CharIsOneOfCharset
==================
*/
static bool Com_CharIsOneOfCharset( char c, char* set )
{
	int i;
	
	for ( i = 0; i < strlen( set ); i++ )
	{
		if ( set[ i ] == c )
			return true;
	}
	
	return false;
}

/*
==================
Com_SkipCharset
==================
*/
char* Com_SkipCharset( char* s, char* sep )
{
	char*   p = s;
	
	while ( p )
	{
		if ( Com_CharIsOneOfCharset( *p, sep ) )
			p++;
		else
			break;
	}
	
	return p;
}

/*
==================
Com_SkipTokens
==================
*/
char* Com_SkipTokens( char* s, int numTokens, char* sep )
{
	int     sepCount = 0;
	char*   p = s;
	
	while ( sepCount < numTokens )
	{
		if ( Com_CharIsOneOfCharset( *p++, sep ) )
		{
			sepCount++;
			while ( Com_CharIsOneOfCharset( *p, sep ) )
				p++;
		}
		else if ( *p == '\0' )
			break;
	}
	
	if ( sepCount == numTokens )
		return p;
	else
		return s;
}
