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



///////////////////////
//	shader内部的参数	 //
///////////////////////
#include "ShaderParameter.h"
#include "ShaderLibrary.h"

#define  MAKE_STRING_ENUM(x,y) #y , x##y

using namespace std;

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	void ShaderParameter::RegisterShaderParameter( ShaderLibrary& library )
	{
		library.SetGlobal( MAKE_STRING_ENUM( FP_, NONE) );
		library.SetGlobal( MAKE_STRING_ENUM( FP_, LOW) );
		library.SetGlobal( MAKE_STRING_ENUM( FP_, MEDIUM) );
		library.SetGlobal( MAKE_STRING_ENUM( FP_, HIGH) );

		library.SetGlobal( MAKE_STRING_ENUM( VT_, FLOAT ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, VEC2 ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, VEC3 ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, VEC4 ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, VEC2_ARRAY ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, VEC3_ARRAY ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, VEC4_ARRAY ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, MAT3 ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, MAT4 ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, TEXTURE1D ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, TEXTURE2D ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, TEXTURE3D ) );
		library.SetGlobal( MAKE_STRING_ENUM( VT_, TEXTURECUBE ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_POSITION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_COORDNATE ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_NORMAL ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_TANGENT ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_BINORMAL ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_BONE_INEX ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_BONE_WEIGHT ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_POSITION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_NORMAL ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_TANGENT ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_BINORMAL ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_VIEWDIRECTION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_LIGHTDIRECTION ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, SCREENSPACE_POSITION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, SCREENSPACE_COORDNATE ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, SCREENSPACE_NORMAL ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, SCREENSPACE_TANGENT ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, SCREENSPACE_BINORMAL ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_LIGHT_DISTANCE ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_DEPTH ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_0 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_1 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_2 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_3 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_4 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_5 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_6 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_7 ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_REAL_ARRAY ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_DUAL_ARRAY ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALWORLD_TRANSFORM ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALWORLD_ROTATION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSCREEN_TRANSVIEWPROJ ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_WORLDPOSITION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_WORLDDIRECTION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_VIEW ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_PROJECTION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_VIEWPROJ ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_LINERPARAM ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_POSITION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_COLOR ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, AMBIENT_COLOR ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_LIGHTDIRECTION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_LIGHTDIRECTION ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_AMBIENT ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_DIFFUSE ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_SPECULAR ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_SPECULAR_LEVEL ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_SHININESS ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_MATERIAL_AMBIENT ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_MATERIAL_LIGHT ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_MATERIAL_SPECULAR ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, SURFACE_COLOR ) );

		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_AMBIENT ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_DIFFUSE ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_SPECULAR ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_SPLEVEL ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_GLOSSINESS ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_ILLUMINATION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_OPACITY ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_FITER ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_BUMP ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_REFLECTION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_REFRACTION ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_DISPLACEMENT ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_WEIGHT ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_DETAL ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_BASE ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_R ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_G ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_B ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER0 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER1 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER2 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER3 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER4 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER5 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER6 ) );
		library.SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER7 ) );
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderParameter::ShaderParameter( const InputsType& it, const FloatPrecision& fp, const VariablesType& vt, const std::string& name )
		:m_eType(vt),
		m_ePrecision(fp),
		m_eInputType(it),
		m_ParamName( name )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	bool ShaderParameter::isUniform() const
	{
		switch( m_eInputType )
		{
		case IT_LOCALSPACE_POSITION:
		case IT_LOCALSPACE_COORDNATE:
		case IT_LOCALSPACE_NORMAL:
		case IT_LOCALSPACE_TANGENT:
		case IT_LOCALSPACE_BINORMAL:
		case IT_LOCALSPACE_BONE_INEX:
		case IT_LOCALSPACE_BONE_WEIGHT:
		case IT_WORLDSPACE_POSITION:
		case IT_WORLDSPACE_NORMAL:
		case IT_WORLDSPACE_TANGENT:
		case IT_WORLDSPACE_BINORMAL:
			//case IT_WORLDSPACE_VIEWDIRECTION://这个必须计算，不能外部传递
		case IT_SCREENSPACE_POSITION:
		case IT_SCREENSPACE_COORDNATE:
		case IT_SCREENSPACE_NORMAL:
		case IT_SCREENSPACE_TANGENT:
		case IT_SCREENSPACE_BINORMAL:
		case IT_WORLDSPACE_LIGHT_DISTANCE:
		case IT_WORLDSPACE_DEPTH:
		case IT_USER_ATTRIBUTE_0://自定义定点流
		case IT_USER_ATTRIBUTE_1:
		case IT_USER_ATTRIBUTE_2:
		case IT_USER_ATTRIBUTE_3:
		case IT_USER_ATTRIBUTE_4:
		case IT_USER_ATTRIBUTE_5:
		case IT_USER_ATTRIBUTE_6:
		case IT_USER_ATTRIBUTE_7:	return false;
		case IT_LOCALSPACE_REAL_ARRAY:
		case IT_LOCALSPACE_DUAL_ARRAY:
		case IT_LOCALWORLD_TRANSFORM:
		case IT_LOCALWORLD_ROTATION:
		case IT_LOCALSCREEN_TRANSVIEWPROJ:
		case IT_CAMERA_WORLDPOSITION:
		case IT_CAMERA_WORLDDIRECTION:
		case IT_CAMERA_VIEW:
		case IT_CAMERA_PROJECTION:
		case IT_CAMERA_VIEWPROJ:
		case IT_CAMERA_LINERPARAM:
		case IT_LIGHT_POSITION:
		case IT_LOCALSPACE_LIGHTDIRECTION:
		case IT_WORLDSPACE_LIGHTDIRECTION:
		case IT_LIGHT_COLOR:
		case IT_AMBIENT_COLOR:
		case IT_MATERIAL_AMBIENT:
		case IT_MATERIAL_DIFFUSE:
		case IT_MATERIAL_SPECULAR:
		case IT_MATERIAL_SPECULAR_LEVEL:
		case IT_MATERIAL_SHININESS:
		case IT_LIGHT_MATERIAL_AMBIENT:
		case IT_LIGHT_MATERIAL_LIGHT:
		case IT_LIGHT_MATERIAL_SPECULAR:
		case IT_TEXTURE_AMBIENT:
		case IT_TEXTURE_DIFFUSE:
		case IT_TEXTURE_SPECULAR:
		case IT_TEXTURE_SPLEVEL:
		case IT_TEXTURE_GLOSSINESS:
		case IT_TEXTURE_ILLUMINATION:
		case IT_TEXTURE_OPACITY:
		case IT_TEXTURE_FITER:
		case IT_TEXTURE_BUMP:
		case IT_TEXTURE_REFLECTION:
		case IT_TEXTURE_REFRACTION:
		case IT_TEXTURE_DISPLACEMENT:
		case IT_TEXTURE_TERRAIN_WEIGHT:
		case IT_TEXTURE_TERRAIN_DETAL:
		case IT_TEXTURE_TERRAIN_BASE:
		case IT_TEXTURE_TERRAIN_R:
		case IT_TEXTURE_TERRAIN_G:
		case IT_TEXTURE_TERRAIN_B:
		case IT_TEXTURE_USER0:
		case IT_TEXTURE_USER1:
		case IT_TEXTURE_USER2:
		case IT_TEXTURE_USER3:
		case IT_TEXTURE_USER4:
		case IT_TEXTURE_USER5:
		case IT_TEXTURE_USER6:
		case IT_TEXTURE_USER7:  return true;
		//case IT_SURFACE_COLOR://这个必须计算，不能外部传递
		default: ASSERT(false && "unknown input type!");//如果定义了新的属性必须修改这里
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	std::string ShaderParameter::AsParameterString( ) const
	{
		string res;
		//其实这里可以直接写if else，分成两边，没这么写的原因是检验shader的错误吧
		switch( m_eInputType )
		{
		case IT_LOCALSPACE_POSITION:
		case IT_LOCALSPACE_COORDNATE:
		case IT_LOCALSPACE_NORMAL:
		case IT_LOCALSPACE_TANGENT:
		case IT_LOCALSPACE_BINORMAL:
		case IT_LOCALSPACE_BONE_INEX:
		case IT_LOCALSPACE_BONE_WEIGHT:
		case IT_WORLDSPACE_POSITION:
		case IT_WORLDSPACE_NORMAL:
		case IT_WORLDSPACE_TANGENT:
		case IT_WORLDSPACE_BINORMAL:
		//case IT_WORLDSPACE_VIEWDIRECTION://这个必须计算，不能外部传递
		case IT_SCREENSPACE_POSITION:
		case IT_SCREENSPACE_COORDNATE:
		case IT_SCREENSPACE_NORMAL:
		case IT_SCREENSPACE_TANGENT:
		case IT_SCREENSPACE_BINORMAL:
		case IT_USER_ATTRIBUTE_0://自定义定点流
		case IT_USER_ATTRIBUTE_1:
		case IT_USER_ATTRIBUTE_2:
		case IT_USER_ATTRIBUTE_3:
		case IT_USER_ATTRIBUTE_4:
		case IT_USER_ATTRIBUTE_5:
		case IT_USER_ATTRIBUTE_6:
		case IT_USER_ATTRIBUTE_7:	res = "attribute ";break;
		//case IT_WORLDSPACE_LIGHT_DISTANCE:
		//case IT_WORLDSPACE_DEPTH://这个必须计算，不能外部传递
		case IT_LOCALSPACE_REAL_ARRAY:
		case IT_LOCALSPACE_DUAL_ARRAY:
		case IT_LOCALWORLD_TRANSFORM:
		case IT_LOCALWORLD_ROTATION:
		case IT_LOCALSCREEN_TRANSVIEWPROJ:
		case IT_CAMERA_WORLDPOSITION:
		case IT_CAMERA_WORLDDIRECTION:
		case IT_CAMERA_VIEW:
		case IT_CAMERA_PROJECTION:
		case IT_CAMERA_VIEWPROJ:
		case IT_CAMERA_LINERPARAM:
		case IT_LIGHT_POSITION:
		case IT_LOCALSPACE_LIGHTDIRECTION:
		case IT_WORLDSPACE_LIGHTDIRECTION:
		case IT_LIGHT_COLOR:
		case IT_AMBIENT_COLOR:
		case IT_MATERIAL_AMBIENT:
		case IT_MATERIAL_DIFFUSE:
		case IT_MATERIAL_SPECULAR:
		case IT_MATERIAL_SPECULAR_LEVEL:
		case IT_MATERIAL_SHININESS:
		case IT_LIGHT_MATERIAL_AMBIENT:
		case IT_LIGHT_MATERIAL_LIGHT:
		case IT_LIGHT_MATERIAL_SPECULAR:
		case IT_TEXTURE_AMBIENT:
		case IT_TEXTURE_DIFFUSE:
		case IT_TEXTURE_SPECULAR:
		case IT_TEXTURE_SPLEVEL:
		case IT_TEXTURE_GLOSSINESS:
		case IT_TEXTURE_ILLUMINATION:
		case IT_TEXTURE_OPACITY:
		case IT_TEXTURE_FITER:
		case IT_TEXTURE_BUMP:
		case IT_TEXTURE_REFLECTION:
		case IT_TEXTURE_REFRACTION:
		case IT_TEXTURE_DISPLACEMENT:
		case IT_TEXTURE_TERRAIN_WEIGHT:
		case IT_TEXTURE_TERRAIN_DETAL:
		case IT_TEXTURE_TERRAIN_BASE:
		case IT_TEXTURE_TERRAIN_R:
		case IT_TEXTURE_TERRAIN_G:
		case IT_TEXTURE_TERRAIN_B:
		case IT_TEXTURE_USER0:
		case IT_TEXTURE_USER1:
		case IT_TEXTURE_USER2:
		case IT_TEXTURE_USER3:
		case IT_TEXTURE_USER4:
		case IT_TEXTURE_USER5:
		case IT_TEXTURE_USER6:
		case IT_TEXTURE_USER7: res = "uniform ";break;
		//case IT_SURFACE_COLOR://这个必须计算，不能外部传递
		default: ASSERT(false && "unknown input type!");//如果定义了新的属性必须修改这里
		}
		switch( m_ePrecision )
		{
		case FP_NONE:	break;
		case FP_LOW:	res += "lowp "; break;
		case FP_MEDIUM: res += "mediump "; break;
		case FP_HIGH:	res += "highp "; break;
		default: ASSERT(false && "unknown float precision!" );
		}
		switch( m_eType )
		{
		case VT_FLOAT:		res += "float "; break;
		case VT_VEC2:		res += "vec2 "; break;
		case VT_VEC3:		res += "vec3 "; break;
		case VT_VEC4:		res += "vec4 "; break;
		case VT_VEC2_ARRAY: res += "vec2 "; break;
		case VT_VEC3_ARRAY: res += "vec3 "; break;
		case VT_VEC4_ARRAY: res += "vec4 "; break;
		case VT_MAT3:		res += "mat3 "; break;
		case VT_MAT4:		res += "mat4 "; break;
		case VT_TEXTURE1D:	res += "sampler1D "; break;
		case VT_TEXTURE2D:	res += "sampler2D "; break;
		case VT_TEXTURE3D:	res += "sampler3D "; break;
		case VT_TEXTURECUBE:res += "samplerCube "; break;
		default:ASSERT(false && "unknown variables type!" );
		}
		res += m_ParamName;
		
		switch( m_eType )
		{
		case VT_FLOAT:
		case VT_VEC2:
		case VT_VEC3:
		case VT_VEC4:
		case VT_MAT3:
		case VT_MAT4:
		case VT_TEXTURE1D:
		case VT_TEXTURE2D:
		case VT_TEXTURE3D:
		case VT_TEXTURECUBE:	res += ";\r\n"; break;
		case VT_VEC2_ARRAY:
		case VT_VEC3_ARRAY:
		case VT_VEC4_ARRAY:		res += "[MAXARRAYSIZE];\r\n"; break;
		default:ASSERT(false && "unknown variables type!" );
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	std::string ShaderParameter::AsVaryingString( const std::string& name ) const
	{
		string res;
		//其实这里可以直接写if else，分成两边，没这么写的原因是检验shader的错误吧
		switch( m_eInputType )
		{
		case IT_LOCALSPACE_POSITION:
		case IT_LOCALSPACE_COORDNATE:
		case IT_LOCALSPACE_NORMAL:
		case IT_LOCALSPACE_TANGENT:
		case IT_LOCALSPACE_BINORMAL:
		case IT_LOCALSPACE_BONE_INEX:
		case IT_LOCALSPACE_BONE_WEIGHT:
		case IT_WORLDSPACE_POSITION:
		case IT_WORLDSPACE_NORMAL:
		case IT_WORLDSPACE_TANGENT:
		case IT_WORLDSPACE_BINORMAL:
		case IT_WORLDSPACE_VIEWDIRECTION:
		case IT_SCREENSPACE_POSITION:
		case IT_SCREENSPACE_COORDNATE:
		case IT_SCREENSPACE_NORMAL:
		case IT_SCREENSPACE_TANGENT:
		case IT_SCREENSPACE_BINORMAL:
		case IT_WORLDSPACE_LIGHT_DISTANCE:
		case IT_WORLDSPACE_DEPTH:
		case IT_USER_ATTRIBUTE_0://自定义定点流
		case IT_USER_ATTRIBUTE_1:
		case IT_USER_ATTRIBUTE_2:
		case IT_USER_ATTRIBUTE_3:
		case IT_USER_ATTRIBUTE_4:
		case IT_USER_ATTRIBUTE_5:
		case IT_USER_ATTRIBUTE_6:
		case IT_USER_ATTRIBUTE_7:	res = "varying ";break;
		case IT_LOCALSPACE_REAL_ARRAY:
		case IT_LOCALSPACE_DUAL_ARRAY:
		case IT_LOCALWORLD_TRANSFORM:
		case IT_LOCALWORLD_ROTATION:
		case IT_LOCALSCREEN_TRANSVIEWPROJ:
		case IT_CAMERA_WORLDPOSITION:
		case IT_CAMERA_WORLDDIRECTION:
		case IT_CAMERA_VIEW:
		case IT_CAMERA_PROJECTION:
		case IT_CAMERA_VIEWPROJ:
		case IT_CAMERA_LINERPARAM:
		case IT_LIGHT_POSITION:
		case IT_LOCALSPACE_LIGHTDIRECTION:
		case IT_WORLDSPACE_LIGHTDIRECTION:
		case IT_LIGHT_COLOR:
		case IT_AMBIENT_COLOR:
		case IT_MATERIAL_AMBIENT:
		case IT_MATERIAL_DIFFUSE:
		case IT_MATERIAL_SPECULAR:
		case IT_MATERIAL_SPECULAR_LEVEL:
		case IT_MATERIAL_SHININESS:
		case IT_LIGHT_MATERIAL_AMBIENT:
		case IT_LIGHT_MATERIAL_LIGHT:
		case IT_LIGHT_MATERIAL_SPECULAR:
		case IT_TEXTURE_AMBIENT:
		case IT_TEXTURE_DIFFUSE:
		case IT_TEXTURE_SPECULAR:
		case IT_TEXTURE_SPLEVEL:
		case IT_TEXTURE_GLOSSINESS:
		case IT_TEXTURE_ILLUMINATION:
		case IT_TEXTURE_OPACITY:
		case IT_TEXTURE_FITER:
		case IT_TEXTURE_BUMP:
		case IT_TEXTURE_REFLECTION:
		case IT_TEXTURE_REFRACTION:
		case IT_TEXTURE_DISPLACEMENT:
		case IT_TEXTURE_TERRAIN_WEIGHT:
		case IT_TEXTURE_TERRAIN_DETAL:
		case IT_TEXTURE_TERRAIN_BASE:
		case IT_TEXTURE_TERRAIN_R:
		case IT_TEXTURE_TERRAIN_G:
		case IT_TEXTURE_TERRAIN_B:
		case IT_TEXTURE_USER0:
		case IT_TEXTURE_USER1:
		case IT_TEXTURE_USER2:
		case IT_TEXTURE_USER3:
		case IT_TEXTURE_USER4:
		case IT_TEXTURE_USER5:
		case IT_TEXTURE_USER6:
		case IT_TEXTURE_USER7: res = "uniform ";break;
		//case IT_SURFACE_COLOR://这个必须计算，不能外部传递
		default: ASSERT(false && "unknown input type!");//如果定义了新的属性必须修改这里
		}
		switch( m_ePrecision )
		{
		case FP_NONE:	break;
		case FP_LOW:	res += "lowp "; break;
		case FP_MEDIUM: res += "mediump "; break;
		case FP_HIGH:	res += "highp "; break;
		default: ASSERT(false && "unknown float precision!" );
		}
		switch( m_eType )
		{
		case VT_FLOAT:		res += "float "; break;
		case VT_VEC2:		res += "vec2 "; break;
		case VT_VEC3:		res += "vec3 "; break;
		case VT_VEC4:		res += "vec4 "; break;
		case VT_VEC2_ARRAY: res += "vec2 "; break;
		case VT_VEC3_ARRAY: res += "vec3 "; break;
		case VT_VEC4_ARRAY: res += "vec4 "; break;
		case VT_MAT3:		res += "mat3 "; break;
		case VT_MAT4:		res += "mat4 "; break;
		case VT_TEXTURE1D:	res += "sampler1D "; break;
		case VT_TEXTURE2D:	res += "sampler2D "; break;
		case VT_TEXTURE3D:	res += "sampler3D "; break;
		case VT_TEXTURECUBE:res += "samplerCube "; break;
		default:ASSERT(false && "unknown variables type!" );
		}
		res += name;

		switch( m_eType )
		{
		case VT_FLOAT:
		case VT_VEC2:
		case VT_VEC3:
		case VT_VEC4:
		case VT_MAT3:
		case VT_MAT4:
		case VT_TEXTURE1D:
		case VT_TEXTURE2D:
		case VT_TEXTURE3D:
		case VT_TEXTURECUBE:	res += ";\r\n"; break;
		case VT_VEC2_ARRAY:
		case VT_VEC3_ARRAY:
		case VT_VEC4_ARRAY:		res += "[MAXARRAYSIZE];\r\n"; break;
		default:ASSERT(false && "unknown variables type!" );
		}
		return res;
	}

	std::string ShaderParameter::AsVariables() const
	{
		string res;
		switch( m_ePrecision )
		{
		case FP_NONE:	break;
		case FP_LOW:	res += "lowp "; break;
		case FP_MEDIUM: res += "mediump "; break;
		case FP_HIGH:	res += "highp "; break;
		default: ASSERT(false && "unknown float precision!" );
		}
		switch( m_eType )
		{
		case VT_FLOAT:		res += "float "; break;
		case VT_VEC2:		res += "vec2 "; break;
		case VT_VEC3:		res += "vec3 "; break;
		case VT_VEC4:		res += "vec4 "; break;
		case VT_VEC2_ARRAY: res += "vec2 "; break;
		case VT_VEC3_ARRAY: res += "vec3 "; break;
		case VT_VEC4_ARRAY: res += "vec4 "; break;
		case VT_MAT3:		res += "mat3 "; break;
		case VT_MAT4:		res += "mat4 "; break;
		case VT_TEXTURE1D:	res += "sampler1D "; break;
		case VT_TEXTURE2D:	res += "sampler2D "; break;
		case VT_TEXTURE3D:	res += "sampler3D "; break;
		case VT_TEXTURECUBE:res += "samplerCube "; break;
		default:ASSERT(false && "unknown variables type!" );
		}
		res += m_ParamName;
		return res;
	}

}