//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix

#include "LinerDepth.glsl"

uniform mat4  matViewProj;
uniform mat4  matTransform;	//moxing de bian huan ju zhen
uniform mat3  NormalTransform;	//faxian de bian huan ju zhen
uniform vec3  vLightPos;
uniform vec3  vCamPos;
uniform mat4  matShadowWorldViewProjBais;
//uniform float fLightRangeInv;
//uniform vec4  vAttenuation;



attribute vec4 position;
attribute vec2 texCoord;
attribute vec3 normal;

//varying float g_fAttenuation;
//varying float g_fDepth;
varying vec2 g_vTexCoord;
varying vec3 g_vLightVec;
varying vec3 g_vViewVec;
varying vec3 g_vNormal;
varying vec4 g_vOutShadowCoord;

void main()
{
	g_vTexCoord = texCoord;
	
	vec4 vPositionWS = matTransform * position;
	gl_Position = matViewProj * vPositionWS;

    g_vNormal = NormalTransform * normal;

    g_vLightVec = vLightPos - vPositionWS.xyz;
    g_vViewVec  = vCamPos - vPositionWS.xyz;

	g_vOutShadowCoord = matShadowWorldViewProjBais * vPositionWS;
	
	//距离衰减
	//highp float dis = length( g_vLightVec );
	//float disRange = clamp( dis * fLightRangeInv, 0.0, 1.0 );
	//g_fAttenuation = (1.0 - disRange ) / ( vAttenuation.x + disRange * vAttenuation.y + disRange * disRange * vAttenuation.z );
	//g_fDepth = GetLineDepthDistance( dis );
}
