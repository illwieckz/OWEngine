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
//  File name:   perPixelLighting.fx
//  Version:     v1.00
//  Created:     
//  Compilers:   
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

float4x4 worldViewProjectionMatrix;
float3 viewOrigin;

texture cubeMapTexture;

samplerCUBE cubeMap = sampler_state
{
	Texture = (cubeMapTexture);
	MagFilter = Point;
	MinFilter = Point;
	MipFilter = Point;
    AddressU = Clamp;  
    AddressV = Clamp;  
};

struct VS_INPUT
{
	float3 position : POSITION;
};

struct VS_OUTPUT
{
	float4 position : POSITION;
	float3 positionRAW : TEXCOORD0;
};

VS_OUTPUT VS_GeneringShader(VS_INPUT IN)
{
    VS_OUTPUT OUT;
    
    OUT.position = mul(float4(IN.position, 1.0f), worldViewProjectionMatrix);
    OUT.positionRAW = IN.position;
    return OUT;
}

float4 PS_GeneringShader(VS_OUTPUT IN) : COLOR
{
    float3 eyeRay = IN.positionRAW-viewOrigin;
    eyeRay = normalize(eyeRay);
	float4 ret = texCUBE(cubeMap,eyeRay);
	return ret;
}

technique GeneringShader
{
	pass
	{
		VertexShader = compile vs_2_0 VS_GeneringShader();
		PixelShader = compile ps_2_0 PS_GeneringShader();
	}
}
