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



//最普通的参数输出最基本的数据
#SHADER_DEFINE BaseTransfrom

	FUNCTION_INPUT = 
	{
		{ LOCALSCREEN_TRANSVIEWPROJ,	HIGH,	MAT4,		"in_TransViewPorj" },
		{ LOCALWORLD_ROTATION,			HIGH,	MAT3,		"in_Rotation" },
		{ LOCALSPACE_POSITION, 			HIGH,	VEC4,		"in_Position" },
		{ LOCALSPACE_NORMAL, 			HIGH,	VEC3,		"in_Normal" },
	}

	FUNCTION_OUTPUT = 
	{
		{ SCREENSPACE_POSITION, 		HIGH,	VEC4,		"out_Position" },
		{ WORLDSPACE_NORMAL, 			HIGH,	VEC3,		"out_Normal" },
	}

#SHADER_CODE
	vec4 out_Position = in_WorldViewPorj * in_Position;
    vec3 out_Normal = in_WorldRotation * in_Normal;
#END_CODE
#END_DEFINE


//输出顶点的世界坐标的shader，光源的一些计算需要这个参数
#SHADER_DEFINE TransfromWorldPosition

	FUNCTION_INPUT = 
	{
		{ CAMERA_VIEWPROJ,			HIGH,	MAT4,		"in_ViewPorj" },
		{ LOCALWORLD_TRANSFORM,		HIGH,	MAT4,		"in_Transfrom" },
		{ LOCALWORLD_ROTATION,		HIGH,	MAT3,		"in_WorldRotation" },
		{ LOCALSPACE_POSITION, 		HIGH,	VEC4,		"in_Position" },
		{ LOCALSPACE_NORMAL, 		HIGH,	VEC3,		"in_Normal" },
	}

	FUNCTION_OUTPUT = 
	{
		{ SCREENSPACE_POSITION, 	HIGH,	VEC4,		"out_Position" },
		{ WORLDSPACE_NORMAL, 		HIGH,	VEC3,		"out_Normal" },
		{ WORLDSPACE_POSITION,		HIGH,	VEC4,		"out_PositionWS" },
	}

#SHADER_CODE
    vec3 out_Normal = in_WorldRotation * in_Normal;
    vec4 out_PositionWS  = in_Transfrom * in_Position;
	vec4 out_Position = in_ViewPorj * out_PositionWS;
#END_CODE
#END_DEFINE