////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
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
//  File name:   doom3ProcPVSClass.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Realtime visibility checks for Doom3 .PROC files
//               NOTE: .PROC files don't have PVS precomputed!
//                     Base upon idPVS class from Doom3 GPL
//                     Source Code
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SHARED_DOOM3PROCPVSCLASS_H__
#define __SHARED_DOOM3PROCPVSCLASS_H__

#include "typedefs.h"

/*
===================================================================================

    PVS

    Note: mirrors and other special view portals are not taken into account

===================================================================================
*/


typedef struct pvsHandle_s
{
	int                 i;          // index to current pvs
	unsigned int        h;          // handle for current pvs
} pvsHandle_t;


typedef struct pvsCurrent_s
{
	pvsHandle_t         handle;     // current pvs handle
	byte*               pvs;        // current pvs bit string
} pvsCurrent_t;

#define MAX_CURRENT_PVS     16 //8      // must be a power of 2

typedef enum
{
	PVS_NORMAL              = 0,    // PVS through portals taking portal states into account
	PVS_ALL_PORTALS_OPEN    = 1,    // PVS through portals assuming all portals are open
	PVS_CONNECTED_AREAS     = 2     // PVS considering all topologically connected areas visible
} pvsType_t;


class idPVS
{
	public:
		idPVS( void );
		~idPVS( void );
		// setup for the given portalized BSP
		void                Init( const class portalizedWorldAPI_i* newPortalWorld );
		void                Shutdown( void );
		// get the area(s) the source is in
		int                 GetPVSArea( const class vec3_c& point ) const;      // returns the area number
		int                 GetPVSAreas( const class aabb& bounds, int* areas, int maxAreas ) const;    // returns number of areas
		// setup current PVS for the source
		pvsHandle_t         SetupCurrentPVS( const vec3_c& source, const pvsType_t type = PVS_NORMAL ) const;
		pvsHandle_t         SetupCurrentPVS( const aabb& source, const pvsType_t type = PVS_NORMAL ) const;
		pvsHandle_t         SetupCurrentPVS( const int sourceArea, const pvsType_t type = PVS_NORMAL ) const;
		pvsHandle_t         SetupCurrentPVS( const int* sourceAreas, const int numSourceAreas, const pvsType_t type = PVS_NORMAL ) const;
		pvsHandle_t         MergeCurrentPVS( pvsHandle_t pvs1, pvsHandle_t pvs2 ) const;
		void                FreeCurrentPVS( pvsHandle_t handle ) const;
		// returns true if the target is within the current PVS
		bool                InCurrentPVS( const pvsHandle_t handle, const vec3_c& target ) const;
		bool                InCurrentPVS( const pvsHandle_t handle, const aabb& target ) const;
		bool                InCurrentPVS( const pvsHandle_t handle, const int targetArea ) const;
		bool                InCurrentPVS( const pvsHandle_t handle, const int* targetAreas, int numTargetAreas ) const;
		// draw all portals that are within the PVS of the source
		void                DrawPVS( const vec3_c& source, const pvsType_t type = PVS_NORMAL ) const;
		void                DrawPVS( const aabb& source, const pvsType_t type = PVS_NORMAL ) const;
		// visualize the PVS the handle points to
		void                DrawCurrentPVS( const pvsHandle_t handle, const vec3_c& source ) const;
		
	private:
		// portal world accessor
		const class portalizedWorldAPI_i* portalWorld;
		// derived data
		int                 numAreas;
		int                 numPortals;
		bool*               connectedAreas;
		int*                areaQueue;
		byte*               areaPVS;
		// current PVS for a specific source possibly taking portal states (open/closed) into account
		mutable pvsCurrent_t currentPVS[MAX_CURRENT_PVS];
		// used to create PVS
		int                 portalVisBytes;
		int                 portalVisLongs;
		int                 areaVisBytes;
		int                 areaVisLongs;
		struct pvsPortal_s* pvsPortals;
		struct pvsArea_s*   pvsAreas;
		
	private:
		int                 GetPortalCount( void ) const;
		void                CreatePVSData( void );
		void                DestroyPVSData( void );
		void                CopyPortalPVSToMightSee( void ) const;
		void                FloodFrontPortalPVS_r( struct pvsPortal_s* portal, int areaNum ) const;
		void                FrontPortalPVS( void ) const;
		struct pvsStack_s*  FloodPassagePVS_r( struct pvsPortal_s* source, const struct pvsPortal_s* portal, struct pvsStack_s* prevStack ) const;
		void                PassagePVS( void ) const;
		void                AddPassageBoundaries( const class cmWinding_c& source, const cmWinding_c& pass, bool flipClip, class plane_c* bounds, int& numBounds, int maxBounds ) const;
		void                CreatePassages( void ) const;
		void                DestroyPassages( void ) const;
		int                 AreaPVSFromPortalPVS( void ) const;
		void                GetConnectedAreas( int srcArea, bool* connectedAreas ) const;
		pvsHandle_t         AllocCurrentPVS( unsigned int h ) const;
};

#endif // __SHARED_DOOM3PROCPVSCLASS_H__
