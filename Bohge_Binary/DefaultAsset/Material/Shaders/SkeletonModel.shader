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





#SHADER_DEFINE Skeleton2Bone

	FUNCTION_INPUT = 
	{
		{ LOCALSPACE_REAL_ARRAY,	HIGH,	VEC4_ARRAY,	"in_RealArray" },
		{ LOCALSPACE_DUAL_ARRAY,	HIGH,	VEC4_ARRAY,	"in_DualArray" },
		{ LOCALSPACE_BONE_INEX,		HIGH,	VEC4,		"in_BoneIndex" },
		{ LOCALSPACE_BONE_WEIGHT,	HIGH,	VEC4,		"in_Boneweight" },
		{ LOCALSPACE_POSITION,		HIGH,	VEC4,		"in_Position" },
		{ LOCALSPACE_NORMAL, 		HIGH,	VEC3,		"in_Normal" },
		{ LOCALSPACE_TANGENT,		HIGH,	VEC4,		"in_Tangent" },
	}

	FUNCTION_OUTPUT = 
	{
		{ LOCALSPACE_POSITION,	HIGH,	VEC4,		"out_Position" },
		{ LOCALSPACE_NORMAL, 	HIGH,	VEC3,		"out_Normal" },
		{ LOCALSPACE_TANGENT, 	HIGH,	VEC3,		"out_Tangent" },
	}

#SHADER_CODE
    ivec2 BoneIndices;
    BoneIndices.x = int(in_BoneIndex.x);
    BoneIndices.y = int(in_BoneIndex.y);
	
	vec4 dp0 = in_RealArray[ BoneIndices[0] ];	
	vec4 blend_real = vec4(0,0,0,0);
	vec4 blend_dual = vec4(0,0,0,0);
	for (int i = 0; i < 2; ++ i)
	{
		vec4 joint_real = in_RealArray[BoneIndices[i]];
		vec4 joint_dual = in_DualArray[BoneIndices[i]];

		float weight = in_Boneweight[i];
		
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
	
	vec3 pos = in_Position.xyz + 2.0 * cross(blend_real.xyz, cross(blend_real.xyz, in_Position.xyz) + blend_real.w * in_Position.xyz);
	vec3 trans = 2.0 * (blend_real.w * blend_dual.xyz - blend_dual.w * blend_real.xyz + cross(blend_real.xyz, blend_dual.xyz));
	pos += trans;

	vec3 out_Normal	 = in_Normal + 2.0 * cross(blend_real.xyz, cross(blend_real.xyz, in_Normal) + blend_real.w * in_Normal);
	vec3 out_Tangent = in_Tangent.xyz + 2.0 * cross(blend_real.xyz, cross(blend_real.xyz, in_Tangent.xyz) + blend_real.w * in_Tangent.xyz);	
	out_Tangent *= in_Tangent.w;
	vec4 out_Position = vec4( pos, in_Position.w );
#END_CODE
#END_DEFINE