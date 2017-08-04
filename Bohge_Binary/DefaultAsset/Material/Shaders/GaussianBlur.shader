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





#SHADER_DEFINE GaussianVertex

	FUNCTION_INPUT = 
	{
		{ LOCALSPACE_COORDNATE,		MEDIUM,		VEC2,			"in_Coordinate" },
		{ MATERIAL_USER_0,			MEDIUM,		VEC2,			"in_vStepSize" },
	}
	FUNCTION_OUTPUT =
	{
		{ MATERIAL_USER_9,			MEDIUM,		VEC2_ARRAY,		"out_GuassianCoord", "7" },
	}

#SHADER_CODE
	mediump vec2 out_GuassianCoord[7];
    out_GuassianCoord[0] = in_Coordinate;
    out_GuassianCoord[1] = out_GuassianCoord[0] - in_vStepSize;
    out_GuassianCoord[2] = out_GuassianCoord[0] + in_vStepSize;
    out_GuassianCoord[3] = out_GuassianCoord[1] - in_vStepSize;
    out_GuassianCoord[4] = out_GuassianCoord[2] + in_vStepSize;
    out_GuassianCoord[5] = out_GuassianCoord[3] - in_vStepSize;
    out_GuassianCoord[6] = out_GuassianCoord[4] + in_vStepSize;
#END_CODE
#END_DEFINE



#SHADER_DEFINE GaussianFragment

	FUNCTION_INPUT = 
	{
		{ TEXTURE_DIFFUSE,		NONE,	TEXTURE2D,		"in_DiffuseTexture" },
		{ MATERIAL_USER_9,		MEDIUM,	VEC2_ARRAY,		"in_GuassianCoord", "7" },
	}
	FUNCTION_OUTPUT = 
	{
		{ SURFACE_COLOR,		LOW,	VEC4,		"out_SurfaceColor" },
	}

#SHADER_CODE
	mediump float Weight[7];
	Weight[0] = 0.1752;
	Weight[1] = Weight[2] = 0.0829;
	Weight[3] = Weight[4] = 0.07015;
	Weight[5] = Weight[6] = 0.05315;
	
    lowp vec4 out_SurfaceColor = texture2D( in_DiffuseTexture, in_GuassianCoord[0] );;
	out_SurfaceColor.rgb =  Weight[0] * out_SurfaceColor.rgb;
	for( int i = 1 ; i < 7 ; i ++ )
	{
		out_SurfaceColor.xyz += Weight[i] * texture2D( in_DiffuseTexture, in_GuassianCoord[i] ).rgb;
	}
#END_CODE
#END_DEFINE