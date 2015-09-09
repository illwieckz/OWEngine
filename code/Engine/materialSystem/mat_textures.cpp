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
//  File name:   mat_textures.cpp
//  Version:     v1.00
//  Created:
//  Compilers:   Visual Studio
//  Description: Textures system frontend
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include <api/textureAPI.h>
#include <api/rbAPI.h>
#include <api/imgAPI.h>
#include <api/coreAPI.h>
#include <shared/str.h>
#include <shared/hashTableTemplate.h>
#include <shared/textureWrapMode.h>

class textureIMPL_c : public textureAPI_i
{
		str name;
		union
		{
			u32 handleU32;
			void* handleV;
		};
		// second handle for DX10 backend
		void* extraHandle;
		u32 w, h;
		textureWrapMode_e wrapMode;
		textureIMPL_c* hashNext;
	public:
		textureIMPL_c()
		{
			hashNext = 0;
			w = h = 0;
			handleV = 0;
			extraHandle = 0;
			wrapMode = TWM_REPEAT; // use GL_REPEAT by default
		}
		~textureIMPL_c()
		{
			if ( rb == 0 )
				return;
			rb->freeTextureData( this );
		}
		
		// returns the path to the texture file (with extension)
		virtual const char* getName() const
		{
			return name;
		}
		void setName( const char* newName )
		{
			name = newName;
		}
		void setWrapMode( textureWrapMode_e newWrapMode )
		{
			wrapMode = newWrapMode;
		}
		virtual u32 getWidth() const
		{
			return w;
		}
		virtual u32 getHeight() const
		{
			return h;
		}
		virtual void setWidth( u32 newWidth )
		{
			w = newWidth;
		}
		virtual void setHeight( u32 newHeight )
		{
			h = newHeight;
		}
		// TWM_CLAMP_TO_EDGE should be set to true for skybox textures
		virtual textureWrapMode_e getWrapMode() const
		{
			return wrapMode;
		}
		virtual void* getInternalHandleV() const
		{
			return handleV;
		}
		virtual void setInternalHandleV( void* newHandle )
		{
			handleV = newHandle;
		}
		virtual u32 getInternalHandleU32() const
		{
			return handleU32;
		}
		virtual void setInternalHandleU32( u32 newHandle )
		{
			handleU32 = newHandle;
		}
		
		// second extra pointer for DX10 backend
		virtual void* getExtraUserPointer() const
		{
			return extraHandle;
		}
		virtual void setExtraUserPointer( void* newHandle )
		{
			extraHandle = newHandle;
		}
		
		textureIMPL_c* getHashNext()
		{
			return hashNext;
		}
		void setHashNext( textureIMPL_c* p )
		{
			hashNext = p;
		}
};

static hashTableTemplateExt_c<textureIMPL_c> mat_textures;
static textureIMPL_c* mat_defaultTexture = 0;

class textureAPI_i* MAT_GetDefaultTexture()
{
		if ( mat_defaultTexture == 0 )
		{
			g_core->Print( "Magic sizeof(textureIMPL_c) number: %i\n", sizeof( textureIMPL_c ) );
			mat_defaultTexture = new textureIMPL_c;
			mat_defaultTexture->setName( "default" );
			byte* data;
			u32 w, h;
			g_img->getDefaultImage( &data, &w, &h );
			rb->uploadTextureRGBA( mat_defaultTexture, data, w, h );
			// we must not free the *default* texture data
		}
		return mat_defaultTexture;
}
class textureAPI_i* MAT_CreateLightmap( int index, const byte* data, u32 w, u32 h, bool rgba )
{
		// for lightmaps
		textureIMPL_c* nl =  new textureIMPL_c;
		rb->uploadLightmap( nl, data, w, h, rgba );
		nl->setName( va( "*lightmap%i", index ) );
		if ( mat_textures.addObject( nl ) )
		{
			g_core->Print( "Warning: %s added more than once.\n", nl->getName() );
			mat_textures.addObject( nl, true );
		}
		return nl;
}
// texString can contain doom3-like modifiers
// TODO: what if a texture is reused with different picmip setting?
class textureAPI_i* MAT_RegisterTexture( const char* texString, enum textureWrapMode_e wrapMode )
{
		textureIMPL_c* ret = mat_textures.getEntry( texString );
		if ( ret )
		{
			return ret;
		}
		if ( !stricmp( texString, "default" ) )
		{
			return MAT_GetDefaultTexture();
		}
		byte* data = 0;
		u32 w, h;
		const char* fixedPath = g_img->loadImage( texString, &data, &w, &h );
		if ( data == 0 )
		{
			return MAT_GetDefaultTexture();
		}
		ret = new textureIMPL_c;
		ret->setName( fixedPath );
		ret->setWrapMode( wrapMode );
		rb->uploadTextureRGBA( ret, data, w, h );
		g_img->freeImageData( data );
		if ( mat_textures.addObject( ret ) )
		{
			g_core->Print( "Warning: %s added more than once.\n", texString );
			mat_textures.addObject( ret, true );
		}
		return ret;
}
class textureAPI_i* MAT_CreateTexture( const char* texName, const byte* picData, u32 w, u32 h )
{
		textureIMPL_c* ret = mat_textures.getEntry( texName );
		if ( ret )
		{
			return ret;
		}
		ret = new textureIMPL_c;
		ret->setName( texName );
		ret->setWrapMode( TWM_REPEAT );
		rb->uploadTextureRGBA( ret, picData, w, h );
		if ( mat_textures.addObject( ret ) )
		{
			g_core->Print( "Warning: %s added more than once.\n", texName );
			mat_textures.addObject( ret, true );
		}
		return ret;
}
//void MAT_FreeTexture(class textureAPI_i **p) {
//  textureAPI_i *ptr = *p;
//  if(ptr == 0)
//      return;
//  (*p) = 0;
//  textureIMPL_c *impl = (textureIMPL_c*) ptr;
//  mat_textures.removeEntry(impl);
//  delete impl;
//}
void MAT_FreeAllTextures()
{
	for ( u32 i = 0; i < mat_textures.size(); i++ )
	{
		textureIMPL_c* t = mat_textures[i];
		delete t;
		mat_textures[i] = 0;
	}
	mat_textures.clear();
	// free the default, build-in image as well
	if ( mat_defaultTexture )
	{
		delete mat_defaultTexture;
		mat_defaultTexture = 0;
	}
}