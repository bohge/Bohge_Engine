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





#SHADER_DEFINE UIVertex

	FUNCTION_INPUT = 
	{
		{ MATERIAL_USER_0,			MEDIUM,	VEC4_ARRAY,	"in_Param", "MAXARRAYSIZE" },
		{ LOCALSPACE_POSITION,		MEDIUM,	VEC4,		"in_Center_Vertex" },
		{ LOCALSPACE_COORDNATE,		MEDIUM,	VEC4,		"in_Coord_Index" },
	}

	FUNCTION_OUTPUT = 
	{
		{ SCREENSPACE_POSITION,	MEDIUM,	VEC4,		"out_Position" },
		{ LOCALSPACE_COORDNATE,	MEDIUM,	VEC2,		"out_Coordinate" },
	}

#SHADER_CODE
	vec2 pos = in_Center_Vertex.xy + in_Center_Vertex.zw;
	pos.x = pos.x * 2.0 - 1.0;
	pos.y = 1.0 - pos.y * 2.0;
	int index = int( in_Coord_Index.z );
	vec4 Parameter = in_Param[index];
	vec4 out_Position = vec4( pos, Parameter.x, 1.0 );
	vec2 out_Coordinate = in_Coord_Index.xy;
#END_CODE
#END_DEFINE