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
uniform float  CamFarInv;
uniform float Time;
uniform mat4  matViewProj;
uniform vec3  vLightDir;
uniform vec3  vCamPos;

attribute vec4 position;
attribute vec4 texCoord;
attribute vec3 normal;

varying vec4 g_vTexCoord;
varying vec3 g_vLightVec;
varying vec3 g_vViewVec;
varying vec3 g_vNormal;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

const vec2 dir1 = vec2(0,-1);
const vec2 dir2 = vec2(1,-1);

void main()
{
	vec4 newPos = position;
	vec2 arguments;
	arguments.x = dot( dir1, position.xz) * 0.6 + Time * 1.2;
	arguments.y = dot( dir2, position.xz) * 0.2 + Time;
	newPos.xz -= 0.5*1.5 * cos( arguments.x );
	newPos.y += 1.5 * sin(arguments.x );
	newPos.xz -= 0.3*0.8 * cos( arguments.y );
	newPos.y += 0.8 * sin( arguments.y );
	gl_Position = matViewProj * newPos;
	g_vTexCoord = texCoord;
    g_vLightVec = vLightDir; 
    g_vViewVec  = vCamPos - position.xyz;
    g_vNormal.x += -dir1.x*0.6*1.5*cos(arguments.x);
    g_vNormal.z += -dir1.y*0.6*1.5*cos(arguments.x);
    g_vNormal.y += 1.0 - 0.5*0.6*1.5*sin(arguments.x);
    
    g_vNormal.x += -dir2.x*0.2*0.5*cos(arguments.y);
    g_vNormal.z += -dir2.y*0.2*0.8*cos(arguments.y);
    g_vNormal.y += 1.0 - 0.3*0.2*0.8*sin(arguments.y);
}
