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
//  File name:   ast.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Abstract Syntax Tree for Doom 3 material expressions
//               evaluation
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SHARED_AST_H__
#define __SHARED_AST_H__

#include "typedefs.h"

class astInputAPI_i
{
public:
    virtual float getTableValue( const char* tableName, float index ) const = 0;
    virtual float getVariableValue( const char* varName ) const = 0;
};
class astAPI_i
{
public:
    virtual ~astAPI_i() { }
    
    virtual astAPI_i* duplicateAST() const = 0;
    virtual void destroyAST() = 0;
    virtual float execute( const class astInputAPI_i* in ) const = 0;
};

class astAPI_i* AST_ParseExpression( const char* s );

#endif // __SHARED_AST_H__
