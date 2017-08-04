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



#define lerp        mix

uniform	float fStep;
uniform vec4 corner00;
uniform vec4 corner01;
uniform vec4 corner10;
uniform vec4 corner11;

attribute vec4 position;
attribute vec2 texCoord;

varying float g_fStep;
varying vec2 g_tc;
varying vec3 tc_m_dx;
varying vec3 tc_p_dx;
varying vec3 tc_m_dy;
varying vec3 tc_p_dy;

void main()
{
	gl_Position = position;
	g_fStep = fStep;
	vec2 tc = texCoord + vec2(-fStep,0);
	vec4 meh = lerp(lerp(corner00,corner01,tc.x),lerp(corner10,corner11,tc.x),tc.y);
	tc_m_dx = meh.xyz/meh.w;
	
	tc = texCoord + vec2(+fStep,0);
	meh = lerp(lerp(corner00,corner01,tc.x),lerp(corner10,corner11,tc.x),tc.y);
	tc_p_dx = meh.xyz/meh.w;
	
	tc = texCoord + vec2(0,-fStep);
	meh = lerp(lerp(corner00,corner01,tc.x),lerp(corner10,corner11,tc.x),tc.y);
	tc_m_dy = meh.xyz/meh.w;
	
	tc = texCoord + vec2(0,fStep);
	meh = lerp(lerp(corner00,corner01,tc.x),lerp(corner10,corner11,tc.x),tc.y);
	tc_p_dy = meh.xyz/meh.w;

	g_tc = texCoord;
}