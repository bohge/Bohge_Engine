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
uniform float Time;
uniform float Tile;
uniform mat4  matViewProj;
uniform vec3  vCamPos;
uniform float reflrefr_offset;
uniform float ColorChangeDepthInv;


attribute vec4 position;
attribute vec2 texture;
attribute vec3 normal;

varying vec3 g_vViewVec;
varying vec3 g_vNormal;
varying vec4 g_vSSPos;
varying vec2 g_vtexCoord;
varying float g_fReflRefr_Offset_On_Z;
varying float g_fHeightNoise;
varying highp float g_SurfaceHeight;


void main()
{
	g_vtexCoord = ( position.xz ) * Tile;
	g_vtexCoord += sin( (g_vtexCoord + Time) * 0.6 );
	g_vSSPos = matViewProj * position;
	gl_Position = g_vSSPos;
	g_vSSPos.xy = (g_vSSPos.xy/g_vSSPos.w + 1.0) * 0.5;
    g_fReflRefr_Offset_On_Z = reflrefr_offset*g_vSSPos.w/g_vSSPos.z;
    g_vViewVec  = vCamPos - position.xyz;
    g_vNormal = normal;
    g_fHeightNoise = texture.x;
    g_SurfaceHeight = saturate( texture.y * ColorChangeDepthInv );
}
