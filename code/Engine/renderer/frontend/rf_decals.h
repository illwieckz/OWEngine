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
//  File name:   rf_decals.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Decals management, storage and drawing
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifndef __RF_DECALS_H__
#define __RF_DECALS_H__

#include <shared/array.h>

// TODO: pvsDecalBatcher for BSP trees
// and octreeDecalBatches for larger outdoor scenes?
class simpleDecalBatcher_c
{
    arraySTD_c<class r_surface_c*> batches;
    arraySTD_c<struct simpleDecal_s*> decals;
    arraySTD_c<class mtrAPI_i*> touchedMaterials; // used by addDecalToBatch and rebuildBatches
    
    void addDecalsWithMatToBatch( class mtrAPI_i* mat, class r_surface_c* batch );
    bool hasDecalWithMat( class mtrAPI_i* m );
    void rebuildBatchWithMat( class mtrAPI_i* m );
public:
    simpleDecalBatcher_c();
    ~simpleDecalBatcher_c();
    
    void addDecalToBatch( const struct simplePoly_s& decalPoly );
    void rebuildBatches();
    void addDrawCalls();
};

#endif // __RF_DECALS_H__
