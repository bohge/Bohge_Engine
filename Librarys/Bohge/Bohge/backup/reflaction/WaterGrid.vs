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
#include "Utility.glsl"

uniform float Time;
uniform float Tile;
uniform float WaveSpeed;
uniform mat4  matViewProj;
uniform vec3  vCamPos;
uniform float reflrefr_offset;
uniform float ColorChangeDepthInv;
uniform mat4 matShadowWorldViewProjBais;
uniform vec3 g_FogParams;
uniform float g_BumpScale;//法线纹理索引的缩放


attribute vec4 position;
attribute vec3 normal;

varying vec3 g_vViewVec;
varying vec3 g_vNormal;
varying vec4 g_vSSPos;
varying vec2 g_SurfaceHeightCoord;
varying vec4 g_RefrCoord_ReflCoord;
varying vec4 g_NoiseCoord;
//varying vec2 g_LightFresnel;
varying vec4 g_vOutShadowCoord;
varying float g_fogFactor;
varying float g_fDepth;



void main()
{
	g_vSSPos = matViewProj * vec4( position.xyz, 1.0 );
   	g_vOutShadowCoord = matShadowWorldViewProjBais * vec4( position.xyz, 1.0 );
	g_fDepth = g_vOutShadowCoord.z / g_vOutShadowCoord.w;
	gl_Position = g_vSSPos;

	//计算波动的纹理
	vec2 NoiseCoord = ( position.xz ) * Tile;
	NoiseCoord += sin( (NoiseCoord + Time) * WaveSpeed );
	g_NoiseCoord.xy = NoiseCoord;
	g_NoiseCoord.zw = NoiseCoord * g_BumpScale;
	
	//fog
	g_fogFactor = fogFactor( gl_Position.w, g_FogParams );
	
	g_vSSPos.xy = (g_vSSPos.xy/g_vSSPos.w + 1.0) * 0.5;
    g_vViewVec  = vCamPos - position.xyz;
    g_vNormal = normalize( normal );
    
    //water color coord
    float SurfaceHeight = saturate( position.w * ColorChangeDepthInv );
    g_SurfaceHeightCoord = vec2( SurfaceHeight );
    
    //refr coord
    float fReflRefr_Offset_On_Z = reflrefr_offset*g_vSSPos.w/g_vSSPos.z;    
	g_RefrCoord_ReflCoord.xy = g_vSSPos.xy - fReflRefr_Offset_On_Z*g_vNormal.xz;
	g_RefrCoord_ReflCoord.zw = g_vSSPos.xy;
	g_RefrCoord_ReflCoord.z = 1.0 - g_RefrCoord_ReflCoord.z;
	//g_LightFresnel = position.xz * Tile;
	//g_LightFresnel = g_LightFresnel - fReflRefr_Offset_On_Z * 2.0*g_vNormal.xz;
	
	//refl coord
}
