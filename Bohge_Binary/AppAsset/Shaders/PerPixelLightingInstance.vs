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




attribute vec4 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec4 matColum1;
attribute vec4 matColum2;
attribute vec4 matColum3;
attribute vec4 matColum4;

uniform mat4  ViewProj;
uniform vec3  vLightDir;
uniform vec3  vCamPos;


varying vec2 g_vTexCoord;
varying vec3 g_vLightVec;
varying vec3 g_vViewVec;
varying vec3 g_vNormal;


void main()
{
    g_vTexCoord = texCoord;
	mat3 NormalTransform = mat3( matColum1.xyz, matColum2.xyz, matColum3.xyz ) ;
	mat4 Transfrom = mat4( matColum1, matColum2, matColum3, matColum4 );
    g_vNormal   = NormalTransform * normal;

	vec4 vPositionES = Transfrom * position;

    g_vLightVec = vLightDir;
    g_vViewVec  = vCamPos - vPositionES.xyz;
    
	gl_Position = ViewProj * vPositionES;
}
