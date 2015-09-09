////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2013 V.
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
//  File name:   stringList.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Simple string list
//               Performs much faster than arraySTD_c<str>
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SHARED_STRINGLIST_H__
#define __SHARED_STRINGLIST_H__

#include "perStringCallback.h"

class stringList_c : public perStringCallbackListener_i
{
    arraySTD_c<char*> list;
    bool bIgnoreDuplicates;
    
    // perStringCallbackListener_i impl
    virtual void perStringCallback( const char* s )
    {
        addString( s );
    }
public:
    stringList_c()
    {
        bIgnoreDuplicates = false;
    }
    ~stringList_c()
    {
        freeMemory();
    }
    void setIgnoreDuplicates( bool bSet )
    {
        bIgnoreDuplicates = bSet;
    }
    void freeMemory()
    {
        for( u32 i = 0; i < list.size(); i++ )
        {
            free( list[i] );
        }
        list.clear();
    }
    void iterateStringList( void ( *callback )( const char* s ) )
    {
        for( u32 i = 0; i < list.size(); i++ )
        {
            callback( list[i] );
        }
    }
    int findIndex( const char* s ) const
    {
        for( u32 i = 0; i < list.size(); i++ )
        {
            const char* check = list[i];
            if( !stricmp( s, check ) )
                return i;
        }
        return -1;
    }
    void addString( const char* s )
    {
        if( bIgnoreDuplicates )
        {
            if( findIndex( s ) >= 0 )
                return; // already on list
        }
        u32 l = strlen( s );
        char* n = ( char* )malloc( l + 1 );
        strcpy( n, s );
        list.push_back( n );
    }
    const char* getString( u32 i ) const
    {
        return list[i];
    }
    static int CompareStringQSort( const void* v0, const void* v1 )
    {
        const char* s0 = *( ( const char** )v0 );
        const char* s1 = *( ( const char** )v1 );
        return stricmp( s0, s1 );
    }
    void sortStrings()
    {
        qsort( list.getArray(), list.size(), list.getElementSize(), CompareStringQSort );
    }
    u32 size() const
    {
        return list.size();
    }
};

#endif // __SHARED_STRINGLIST_H__
