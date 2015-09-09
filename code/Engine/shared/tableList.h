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
//  File name:   tableList.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Tables manager for Doom3 .MTR tables
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SHARED_TABLELIST_H__
#define __SHARED_TABLELIST_H__

#include "hashTableTemplate.h"
#include <api/tableListAPI.h>
#include "str.h"

typedef bool ( *TABLE_FindTableTextFunc_t )( const char* tableName, const char** p, const char** textBase, const char** sourceFileName );

class table_c
{
    str name;
    table_c* hashNext;
    arraySTD_c<float> values;
    bool bSnap;
public:
    bool parse( const char* at, const char* textBase, const char* sourceFileName );
    float getValue( float idx ) const;
    
    void setName( const char* newName )
    {
        name = newName;
    }
    void setHashNext( table_c* n )
    {
        hashNext = n;
    }
    const char* getName() const
    {
        return name.c_str();
    }
    table_c* getHashNext() const
    {
        return hashNext;
    }
};

class tableList_c : public tableListAPI_i
{
    TABLE_FindTableTextFunc_t findTextFunc;
    mutable hashTableTemplateExt_c<table_c> tables;
public:
    tableList_c( TABLE_FindTableTextFunc_t newFindTextFunc );
    ~tableList_c();
    
    virtual float getTableValue( const char* tableName, float index ) const;
};

#endif // __SHARED_TABLELIST_H__
