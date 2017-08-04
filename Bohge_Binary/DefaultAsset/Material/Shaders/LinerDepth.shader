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




#SHADER_DEFINE LinerDepth

	FUNCTION_INPUT = 
	{
		{ WORLDSPACE_POSITION,			HIGH,	VEC4,		"in_Position" },
		{ LIGHT_POSITION, 				HIGH,	VEC3,		"in_LightPosition" },
		{ CAMERA_LINERPARAM, 			HIGH,	VEC3,		"in_CameraLineDepthParam" },
	}

	FUNCTION_OUTPUT = 
	{
		{ WORLDSPACE_DEPTH, 			HIGH,	FLOAT,		"out_Depth" },
	}

#SHADER_CODE
	highp float cdepth = distance(in_Position.xyz, in_LightPosition);
	highp float out_Depth = ( cdepth - in_CameraLineDepthParam.x ) * in_CameraLineDepthParam.y;
#END_CODE
#END_DEFINE


#SHADER_DEFINE LinerDepthDistance

	FUNCTION_INPUT = 
	{
		{ WORLDSPACE_LIGHT_DISTANCE,	HIGH,	FLOAT,		"in_Distance" },
		{ CAMERA_LINERPARAM, 			HIGH,	VEC3,		"in_CameraLineDepthParam" },
	}

	FUNCTION_OUTPUT = 
	{
		{ WORLDSPACE_DEPTH, 			HIGH,	FLOAT,		"out_Depth" },
	}

#SHADER_CODE
	highp float out_Depth = ( in_Distance - in_CameraLineDepthParam.x ) * in_CameraLineDepthParam.y;
#END_CODE
#END_DEFINE