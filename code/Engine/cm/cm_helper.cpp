////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2012 V.
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
//  File name:   cm_helper.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "cm_local.h"
#include "cm_model.h"
#include "cm_helper.h"

cmHelper_c::cmHelper_c()
{
	myCompound = 0;
}
cmHelper_c::~cmHelper_c()
{
	if ( myCompound )
	{
		delete myCompound;
	}
}
class cmCompound_c* cmHelper_c::registerCompound()
{
		if ( this->myCompound == 0 )
		{
			this->myCompound = new cmCompound_c( "helperCompound" );
		}
		return this->myCompound;
}
bool cmHelper_c::hasCompoundModel() const
{
	if ( myCompound )
		return true;
	return false;
}
class cmCompound_c* cmHelper_c::getCompound() const
{
		return myCompound;
}
class cmCompound_i* cmHelper_c::getCompoundAPI() const
{
		return myCompound;
}
