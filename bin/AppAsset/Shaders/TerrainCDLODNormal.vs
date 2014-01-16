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
uniform mat4 matViewProj;
uniform vec3 vCamPos;
uniform vec4 vM_S_C;
uniform vec2 vOffsetScale; // fang da hou de suo fang
uniform int dIndex;


attribute vec4 position;
attribute vec4 diffHeight;
attribute vec3 normal;


varying vec3 g_vNormal;



void main()
{
	vec4 localDiff = diffHeight;
	vec4 newPosition = position;
	vec3 Dispos = position.xyz;
	Dispos.xz += vOffsetScale;
	float morphLerpK = distance( Dispos.xyz, vCamPos );
	morphLerpK = 1.0 - clamp( vM_S_C.x - morphLerpK * vM_S_C.y , 0.0, 1.0 );
    vec2 fracPart = fract( position.xz * ( vM_S_C.z * 0.5) ) * vM_S_C.w;
	newPosition.xz = newPosition.xz - fracPart * morphLerpK;
	newPosition.y += localDiff[dIndex] * morphLerpK;
	newPosition.xz += vOffsetScale;
	gl_Position = matViewProj * newPosition;
    g_vNormal = normalize( normal ) * 0.5 + 0.5 ;
}
