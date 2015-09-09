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
//  File name:   rf_entities.h
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __RF_ENTITIES_H__
#define __RF_ENTITIES_H__

#include <shared/array.h>
#include <shared/flags32.h>
#include <shared/str.h>
#include <math/vec3.h>
#include <math/axis.h>
#include <math/matrix.h>
#include <math/aabb.h>
#include <api/rEntityAPI.h>
#include <renderer/rfSurfsFlagsArray.h>
#include "../pointLightSample.h"

class rEntityImpl_c : public rEntityAPI_i
{
		matrix_c matrix;
		vec3_c angles;
		axis_c axis;
		vec3_c origin;
		class rModelAPI_i* model;
		aabb absBB;
		bool bFirstPersonOnly;
		bool bThirdPersonOnly;
		bool bHidden;
		bool bIsPlayerModel;
		rfSurfsFlagsArray_t surfaceFlags; // surfaceFlags.size() == this->getNumSurfaces()
		str skinName;
		// the skin definition itself
		//class rSkinRemap_c *skin;
		// model materials after applying the skin
		//class rSkinMatList_c *skinMatList;
		// used only for static (non-animated) model entities
		class simpleDecalBatcher_c* staticDecals;
		// only for skeletal models; model geometry instanced at the current time of animation
		class r_model_c* instance;
		class skelAnimController_c* skelAnimCtrl;
		class skelAnimController_c* skelAnimCtrlTorso;
		class q3AnimCtrl_c* q3AnimCtrl;
		// for ragdolls
		const class afDeclAPI_i* myRagdollDef;
		class boneOrQPArray_t* ragOrs;
		class boneOrArray_c* finalBones;
		arraySTD_c<matrix_c> boneParentBody2Bone;
		// incremented every time absolute entity silhuette is changed
		// used for shadow volumes creation
		// (shadow volumes are rebuild only when it's changed)
		u32 absSilChangeCount;
		// lightgrid lighting sample at entity abs bounds center
		pointLightSample_s centerLightSample;
		bool bCenterLightSampleValid;
		// areas touching entity absBounds
		arraySTD_c<u32> touchingAreas;
		// used to avoid updating animated entity several times in single frame
		u32 animatedEntityUpdateFrame;
		// corresponding serverside entity index
		int networkingEntityNumber;
		// entityType_e
		int entityType;
		
		// this is called when a model skin, or a model itself is changed
		void updateModelSkin();
		
		u32 getEntityTriangleCount() const;
	public:
		rEntityImpl_c();
		~rEntityImpl_c();
		
		void recalcABSBounds();
		void recalcMatrix();
		virtual void setOrigin( const vec3_c& newXYZ );
		virtual void setAngles( const vec3_c& newAngles );
		virtual void setModel( class rModelAPI_i* newModel );
		virtual void setAnim( const class skelAnimAPI_i* anim, int newFlags );
		virtual void setAnim( const char* animName, int newFlags );
		virtual void setTorsoAnim( const class skelAnimAPI_i* anim, int newFlags );
		virtual void setThirdPersonOnly( bool bOn )
		{
			bThirdPersonOnly = bOn;
		}
		virtual void setFirstPersonOnly( bool bOn )
		{
			bFirstPersonOnly = bOn;
		}
		virtual void setIsPlayerModel( bool bNewIsPlayerModel )
		{
			bIsPlayerModel = bNewIsPlayerModel;
		}
		virtual bool isPlayerModel() const
		{
			return bIsPlayerModel;
		}
		virtual void setRagdoll( const class afDeclAPI_i* af );
		virtual void setRagdollBodyOr( u32 partIndex, const class boneOrQP_c& or );
		virtual void setDeclModelAnimLocalIndex( int localAnimIndex, int newFlags );
		virtual void setQ3LegsAnimLocalIndex( int localAnimIndex );
		virtual void setQ3TorsoAnimLocalIndex( int localAnimIndex );
		virtual void setSkin( const char* skinName );
		virtual void setNetworkingEntityNumber( int newNetEntNum )
		{
			networkingEntityNumber = newNetEntNum;
		}
		virtual void setEntityType( int newEntityType )
		{
			entityType = newEntityType;
		}
		
		virtual void hideModel();
		virtual void showModel();
		virtual int addDecalWorldSpace( const class vec3_c& pos,
										const class vec3_c& normal, float radius, class mtrAPI_i* material );
										
		bool isFirstPersonOnly() const
		{
			return bFirstPersonOnly;
		}
		bool isThirdPersonOnly() const
		{
			return bThirdPersonOnly;
		}
		bool isHidden() const
		{
			return bHidden;
		}
		virtual bool isRagdoll() const
		{
			if ( myRagdollDef )
				return true;
			return false;
		}
		virtual bool hasDeclModel() const;
		virtual bool isQ3PlayerModel() const;
		virtual bool isAnimated() const;
		virtual bool isSprite() const;
		virtual bool hasAnim( const char* animName ) const;
		
		bool hasStageWithoutBlendFunc() const;
		u32 getSilChangeCount() const
		{
			return absSilChangeCount;
		}
		const arraySTD_c<u32>& getTouchingAreas() const
		{
			return touchingAreas;
		}
		int getEntityType() const
		{
			return entityType;
		}
		
		virtual rModelAPI_i* getModel() const;
		virtual const char* getModelName() const;
		virtual const axis_c& getAxis() const
		{
			return axis;
		}
		virtual const vec3_c& getOrigin() const
		{
			return origin;
		}
		virtual const matrix_c& getMatrix() const
		{
			return matrix;
		}
		virtual const class aabb& getBoundsABS() const
		{
				return absBB;
		}
		virtual int getNetworkingEntityNumber() const
		{
			return networkingEntityNumber;
		}
		class simpleDecalBatcher_c* getStaticDecalsBatch()
		{
				return staticDecals;
		}
		
		const class r_model_c* getCurrentRModelInstance() const;
		
		virtual void hideSurface( u32 surfNum );
		
		void updateAnimatedEntity();
		void addDrawCalls();
		
		virtual bool rayTraceLocal( class trace_c& tr ) const;
		virtual bool rayTraceWorld( class trace_c& tr ) const;
		virtual bool getBoneWorldOrientation( const char* boneName, class matrix_c& out );
		virtual bool getBoneWorldOrientation( int localBoneIndex, class matrix_c& out );
};

// NULL == worldspawn
extern class rEntityAPI_i* rf_currentEntity;

#endif // __RF_ENTITIES_H__
