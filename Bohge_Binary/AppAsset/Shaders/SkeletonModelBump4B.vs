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



uniform vec4 Real[MAXJOINTS];//骨骼数据
uniform vec4 Dual[MAXJOINTS];

uniform mat4  matViewProj;
uniform mat4  matTransform;	//moxing de bian huan ju zhen
uniform mat3  NormalTransform;	//faxian de bian huan ju zhen
uniform vec3  vLightDir;
uniform vec3  vCamPos;

attribute vec4 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec4 tangent;	//切线是4最后一位是是否反转binormal的标识
attribute vec4 boneIndex;//骨骼索引
attribute vec4 boneweight;//权重

varying vec2 g_vTexCoord;
varying vec3 g_vLightVec;
varying vec3 g_vViewVec;

varying vec4 g_vColor;


void main()
{
	g_vTexCoord = texCoord;
	
    ivec4 BoneIndices;
    BoneIndices.x = int(boneIndex.x);
    BoneIndices.y = int(boneIndex.y);
	BoneIndices.z = int(boneIndex.z);
	BoneIndices.w = int(boneIndex.w);
	
	vec4 weights = boneweight;
	
	vec4 dp0 = Real[ BoneIndices[0] ];	
	vec4 blend_real = vec4(0,0,0,0);
	vec4 blend_dual = vec4(0,0,0,0);
	for (int i = 0; i < 4; ++ i)
	{
		vec4 joint_real = Real[BoneIndices[i]];
		vec4 joint_dual = Dual[BoneIndices[i]];

		float weight = weights[i];
		
		if (dot(dp0, joint_real) < 0.0)
		{
			joint_real = -joint_real;
			joint_dual = -joint_dual;
		}

		blend_real += joint_real * weight;
		blend_dual += joint_dual * weight;
	}

	float inv_len = 1.0 / length(blend_real);
	blend_real *= inv_len;
	blend_dual *= inv_len;
	
	vec3 pos = position.xyz + 2.0 * cross(blend_real.xyz, cross(blend_real.xyz, position.xyz) + blend_real.w * position.xyz);
	vec3 trans = 2.0 * (blend_real.w * blend_dual.xyz - blend_dual.w * blend_real.xyz + cross(blend_real.xyz, blend_dual.xyz));
	pos += trans;

	vec3 result_normal = normal + 2.0 * cross(blend_real.xyz, cross(blend_real.xyz, normal) + blend_real.w * normal);
	vec3 result_tangent = tangent.xyz + 2.0 * cross(blend_real.xyz, cross(blend_real.xyz, tangent.xyz) + blend_real.w * tangent.xyz);	
	vec4 result_pos = vec4( pos, position.w );
	
	vec4 vPositionES = matTransform * result_pos;
	gl_Position = matViewProj * vPositionES;

    vec3 endNormal = NormalTransform * result_normal;
	vec3 endTangent = NormalTransform * result_tangent;
	vec3 endBinormal = cross( endNormal, endTangent.xyz) * tangent.w;

    vec3 LightWorld = vLightDir;
    vec3 ViewWorld  = vCamPos - vPositionES.xyz;
	
	g_vLightVec.x = dot( LightWorld, endTangent);
    g_vLightVec.y = dot( LightWorld, endBinormal);
    g_vLightVec.z = dot( LightWorld, endNormal);

    g_vViewVec.x  = dot( ViewWorld, endTangent );
    g_vViewVec.y  = dot( ViewWorld, endBinormal );
    g_vViewVec.z  = dot( ViewWorld, endNormal );
	
}
