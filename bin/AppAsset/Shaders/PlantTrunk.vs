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
uniform mat4 matWorldViewProj;
uniform vec3 vCameraPos;
uniform vec3 vLightDir;
uniform vec3 vAmbient;
uniform vec3 vLightColor;
uniform float Shininess;
uniform vec3 vSpecularLight;
uniform vec3 g_FogParams;

attribute vec4 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 g_vTexCoord;
varying vec3 g_vLightColor;
//varying vec3 g_vSpecular;
varying float g_fogFactor;


void main()
{
    g_vTexCoord = texCoord;
	gl_Position = matWorldViewProj * vec4( position.xyz, 1.0);
	g_vLightColor = vAmbient + vLightColor * position.w;
	//fog
	g_fogFactor = fogFactor( gl_Position.w, g_FogParams );
		
    //vec3 vNormal= normalize( normal );
    //vec3 vView   = normalize( vCameraPos - position.xyz );
    //vec3 vHalf   = normalize( vLightDir + vView );
    
    //float dotLightNormal= dot( vLightDir, normal );
	//float fDiffuse = saturate( dotLightNormal );
	//float fSpecular = pow( saturate( dot( vHalf, normal ) ) , Shininess );
	//g_vSpecular = fSpecular * vSpecularLight;
	//g_vLightColor = vAmbient + vLightColor * fDiffuse;
}
