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





#SHADER_DEFINE DirectionLightVertex

	FUNCTION_INPUT = 
	{
		{ CAMERA_WORLDPOSITION,		HIGH,	VEC3,	"in_CameraPosition" },
		{ WORLDSPACE_POSITION,		HIGH,	VEC4,	"in_PositionWS" },
	}

	FUNCTION_OUTPUT = 
	{
		{ WORLDSPACE_VIEWDIRECTION,	HIGH,	VEC3,	"out_ViewDirection" },
	}

#SHADER_CODE
    vec3 out_ViewDirection  = in_CameraPosition - in_PositionWS.xyz;
#END_CODE
#END_DEFINE


#SHADER_DEFINE DirectionLightSurface

	FUNCTION_INPUT = 
	{
		{ WORLDSPACE_NORMAL, 			MEDIUM,		VEC3,		"in_Normal" },
		{ LIGHT_GIVEN_DIRECTION,		MEDIUM,		VEC3,		"in_LightDirection" },
		{ WORLDSPACE_VIEWDIRECTION,		MEDIUM,		VEC3,		"in_ViewDirection" },
		{ MATERIAL_SHININESS,			MEDIUM,		FLOAT,		"in_Shininess" },
		{ LIGHT_MATERIAL_AMBIENT,		LOW,		VEC3,		"in_Ambient_Light" },
		{ LIGHT_MATERIAL_LIGHT,			LOW,		VEC3,		"in_LightColor" },
		{ LIGHT_MATERIAL_SPECULAR,		LOW,		VEC3,		"in_Specular_Light" },
	}

	FUNCTION_OUTPUT = 
	{
		{ LIGHTING_DIFFUSE,				LOW,	VEC3,		"out_Diffuse" },
		{ LIGHTING_SPECULAR,			LOW,	VEC3,		"out_Specular" },
	}

#SHADER_CODE
    mediump vec3 vNormal    	= normalize( in_Normal );
    
    mediump vec3  vLight    	= normalize( in_LightDirection );
    mediump vec3  vView     	= normalize( in_ViewDirection );
    mediump vec3  vHalf     	= normalize( vLight + vView );
    mediump float dotLightNormal= dot( vLight, vNormal );
	mediump float fDiffuse = clamp( dotLightNormal, 0.0, 1.0 );	
	mediump float fSpecular = pow( clamp( dot( vHalf, vNormal ), 0.0, 1.0 ) , in_Shininess );		
	
	lowp vec3 out_Diffuse = in_Ambient_Light + fDiffuse * in_LightColor;
	lowp vec3 out_Specular = fSpecular * in_Specular_Light;
#END_CODE
#END_DEFINE