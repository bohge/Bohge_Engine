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



#include "LinerDepth.glsl"

uniform vec4 Real[MAXJOINTS];
uniform vec4 Dual[MAXJOINTS];

uniform mat4 matWorldViewProj;
uniform mat4  matWorldTransform;

attribute vec4 position;
attribute vec4 boneIndex;
attribute vec4 boneweight;

varying highp float g_fDepth;


void main()
{
    int BoneIndices;
    BoneIndices = int(boneIndex.x);
	
	vec4 dp0 = Real[ BoneIndices ];	
	vec4 blend_real = vec4(0,0,0,0);
	vec4 blend_dual = vec4(0,0,0,0);

	vec4 joint_real = Real[BoneIndices];
	vec4 joint_dual = Dual[BoneIndices];

	float weight = boneweight.x;
		
	if (dot(dp0, joint_real) < 0.0)
	{
		joint_real = -joint_real;
		joint_dual = -joint_dual;
	}

	blend_real += joint_real * weight;
	blend_dual += joint_dual * weight;

	float inv_len = 1.0 / length(blend_real);
	blend_real *= inv_len;
	blend_dual *= inv_len;
	
	vec3 pos = position.xyz + 2.0 * cross(blend_real.xyz, cross(blend_real.xyz, position.xyz) + blend_real.w * position.xyz);
	vec3 trans = 2.0 * (blend_real.w * blend_dual.xyz - blend_dual.w * blend_real.xyz + cross(blend_real.xyz, blend_dual.xyz));
	pos += trans;
	
	vec4 result_pos = vec4( pos, position.w );
	
	gl_Position = matWorldViewProj * result_pos;
	vec4 worldPos = matWorldTransform * result_pos;
	g_fDepth = GetLineDepth( worldPos.xyz );
}