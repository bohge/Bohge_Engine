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

#define  MAKE_STRING_ENUM(x,y) #y , x##y

using namespace eastl;

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	ShaderParameter::ShaderParameter( const InputsType& it, const FloatPrecision& fp, const VariablesType& vt, const eastl::string& name )
		:m_eType(vt),
		m_ePrecision(fp),
		m_eInputType(it),
		m_ParamName( name ),
		m_ArraySize("0")
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderParameter::ShaderParameter( const InputsType& it, const FloatPrecision& fp, const VariablesType& vt, const eastl::string& name, const eastl::string& arraysize )
		:m_eType(vt),
		m_ePrecision(fp),
		m_eInputType(it),
		m_ParamName( name ),
		m_ArraySize( arraysize )
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
		case IT_LOCALSPACE_COLOR:
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
		case IT_EMITTER_OFFSET:
		case IT_EMITTER_LIFE:
		case IT_EMITTER_BORN:
		//case IT_EMITTER_AGE:
		case IT_EMITTER_SPEED:
		case IT_EMITTER_ACCELERATION:
		case IT_EMITTER_SIZE:
		case IT_EMITTER_SIZE_CURVATURE:
		case IT_EMITTER_COLOR:
		case IT_EMITTER_COLOR_CURVATURE:
		case IT_EMITTER_SPIN:
		case IT_LIGHT_VERTEX_DIRECTION:
		case IT_USER_ATTRIBUTE_0://自定义定点流
		case IT_USER_ATTRIBUTE_1:
		case IT_USER_ATTRIBUTE_2:
		case IT_USER_ATTRIBUTE_3:
		case IT_USER_ATTRIBUTE_4:
		case IT_USER_ATTRIBUTE_5:
		case IT_USER_ATTRIBUTE_6:
		case IT_USER_ATTRIBUTE_7:	return false;
		case IT_SYSTEM_TIME:
		case IT_SYSTEM_TIME_SPAN:
		case IT_LOCALSPACE_REAL_ARRAY:
		case IT_LOCALSPACE_DUAL_ARRAY:
		case IT_LOCALWORLD_TRANSFORM:
		case IT_LOCALWORLD_ROTATION:
		case IT_LOCALSCREEN_TRANSVIEWPROJ:
		case IT_CAMERA_WORLDROTATION:
		case IT_CAMERA_WORLDPOSITION:
		case IT_CAMERA_WORLDDIRECTION:
		case IT_CAMERA_VIEW:
		case IT_CAMERA_PROJECTION:
		case IT_CAMERA_VIEWPROJ:
		case IT_CAMERA_LINERPARAM:
		case IT_LIGHT_POSITION:
		case IT_LIGHT_GIVEN_DIRECTION:
		case IT_LIGHT_COLOR:
		case IT_AMBIENT_COLOR:
		case IT_LIGHT_ANGLE:
		case IT_LIGHT_INNER_DIFF_INV:
		case IT_LIGHT_RANGE_INV:
		case IT_LIGHT_ATTENUATION:
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
		case IT_TEXTURE_USER7:
		case IT_COLOR_RED:
		case IT_COLOR_GREEN:
		case IT_COLOR_BLUE:
		case IT_COLOR_ALPHA:
		case IT_MATERIAL_USER_0:
		case IT_MATERIAL_USER_1:
		case IT_MATERIAL_USER_2:
		case IT_MATERIAL_USER_3:
		case IT_MATERIAL_USER_4:
		case IT_MATERIAL_USER_5:
		case IT_MATERIAL_USER_6:
		case IT_MATERIAL_USER_7:
		case IT_MATERIAL_USER_8:
		case IT_MATERIAL_USER_9:  return true;
		//case IT_SURFACE_COLOR://这个必须计算，不能外部传递
		//case IT_LIGHTING_DIFFUSE:
		//case IT_LIGHTING_SPECULAR:
		default: ASSERT(false && "unknown input type!");//如果定义了新的属性必须修改这里
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string ShaderParameter::AsParameterString( ) const
	{
		string res;
		//其实这里可以直接写if else，分成两边，没这么写的原因是检验shader的错误吧
		switch( m_eInputType )
		{
		case IT_LOCALSPACE_POSITION:
		case IT_LOCALSPACE_COORDNATE:
		case IT_LOCALSPACE_NORMAL:
		case IT_LOCALSPACE_COLOR:
		case IT_LOCALSPACE_TANGENT:
		case IT_LOCALSPACE_BINORMAL:
		case IT_LOCALSPACE_BONE_INEX:
		case IT_LOCALSPACE_BONE_WEIGHT:
		//case IT_WORLDSPACE_POSITION://注释掉的必须计算，不能外部传递
		//case IT_WORLDSPACE_NORMAL:
		//case IT_WORLDSPACE_TANGENT:
		//case IT_WORLDSPACE_BINORMAL:
		//case IT_WORLDSPACE_VIEWDIRECTION:
		//case IT_SCREENSPACE_POSITION:
		//case IT_SCREENSPACE_COORDNATE:
		//case IT_SCREENSPACE_NORMAL:
		//case IT_SCREENSPACE_TANGENT:
		//case IT_SCREENSPACE_BINORMAL:
		case IT_EMITTER_OFFSET:
		case IT_EMITTER_LIFE:
		case IT_EMITTER_BORN:
		//case IT_EMITTER_AGE:
		case IT_EMITTER_SPEED:
		case IT_EMITTER_ACCELERATION:
		case IT_EMITTER_SIZE:
		case IT_EMITTER_SIZE_CURVATURE:
		case IT_EMITTER_COLOR:
		case IT_EMITTER_COLOR_CURVATURE:
		case IT_EMITTER_SPIN:
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
		case IT_SYSTEM_TIME:
		case IT_SYSTEM_TIME_SPAN:
		case IT_LOCALSPACE_REAL_ARRAY:
		case IT_LOCALSPACE_DUAL_ARRAY:
		case IT_LOCALWORLD_TRANSFORM:
		case IT_LOCALWORLD_ROTATION:
		case IT_LOCALSCREEN_TRANSVIEWPROJ:
		case IT_CAMERA_WORLDROTATION:
		case IT_CAMERA_WORLDPOSITION:
		case IT_CAMERA_WORLDDIRECTION:
		case IT_CAMERA_VIEW:
		case IT_CAMERA_PROJECTION:
		case IT_CAMERA_VIEWPROJ:
		case IT_CAMERA_LINERPARAM:
		case IT_LIGHT_POSITION:
		//case IT_VERTEX_LIGHTDIRECTION:
		case IT_LIGHT_GIVEN_DIRECTION:
		case IT_LIGHT_ANGLE:
		case IT_LIGHT_INNER_DIFF_INV:
		case IT_LIGHT_RANGE_INV:
		case IT_LIGHT_ATTENUATION:
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
		case IT_TEXTURE_USER7:
		case IT_COLOR_RED:
		case IT_COLOR_GREEN:
		case IT_COLOR_BLUE:
		case IT_COLOR_ALPHA:
		case IT_MATERIAL_USER_0:
		case IT_MATERIAL_USER_1:
		case IT_MATERIAL_USER_2:
		case IT_MATERIAL_USER_3:
		case IT_MATERIAL_USER_4:
		case IT_MATERIAL_USER_5:
		case IT_MATERIAL_USER_6:
		case IT_MATERIAL_USER_7:
		case IT_MATERIAL_USER_8:
		case IT_MATERIAL_USER_9: res = "uniform ";break;
		//case IT_SURFACE_COLOR://这个必须计算，不能外部传递
		//case IT_LIGHTING_DIFFUSE:
		//case IT_LIGHTING_SPECULAR:
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
		case VT_VEC4_ARRAY:		res += "[" + m_ArraySize + "];\r\n"; break;
		default:ASSERT(false && "unknown variables type!" );
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string ShaderParameter::AsVaryingString( const eastl::string& name ) const
	{
		string res;
		////其实这里可以直接写if else，分成两边，没这么写的原因是检验shader的错误吧
		//switch( m_eInputType )
		//{
		//case IT_LOCALSPACE_POSITION:
		//case IT_LOCALSPACE_COORDNATE:
		//case IT_LOCALSPACE_NORMAL:
		//case IT_LOCALSPACE_COLOR:
		//case IT_LOCALSPACE_TANGENT:
		//case IT_LOCALSPACE_BINORMAL:
		//case IT_LOCALSPACE_BONE_INEX:
		//case IT_LOCALSPACE_BONE_WEIGHT:
		//case IT_WORLDSPACE_POSITION:
		//case IT_WORLDSPACE_NORMAL:
		//case IT_WORLDSPACE_TANGENT:
		//case IT_WORLDSPACE_BINORMAL:
		//case IT_WORLDSPACE_VIEWDIRECTION:
		//case IT_SCREENSPACE_POSITION:
		//case IT_SCREENSPACE_COORDNATE:
		//case IT_SCREENSPACE_NORMAL:
		//case IT_SCREENSPACE_TANGENT:
		//case IT_SCREENSPACE_BINORMAL:
		//case IT_WORLDSPACE_LIGHT_DISTANCE:
		//case IT_WORLDSPACE_DEPTH:
		//case IT_LIGHT_VERTEX_DIRECTION:
		//case IT_USER_ATTRIBUTE_0://自定义定点流
		//case IT_USER_ATTRIBUTE_1:
		//case IT_USER_ATTRIBUTE_2:
		//case IT_USER_ATTRIBUTE_3:
		//case IT_USER_ATTRIBUTE_4:
		//case IT_USER_ATTRIBUTE_5:
		//case IT_USER_ATTRIBUTE_6:
		//case IT_USER_ATTRIBUTE_7:	res = "varying ";break;
		////case IT_GIVEN_LIGHTDIRECTION:
		////case IT_LOCALSPACE_REAL_ARRAY:
		////case IT_LOCALSPACE_DUAL_ARRAY:
		////case IT_LOCALWORLD_TRANSFORM:
		////case IT_LOCALWORLD_ROTATION:
		////case IT_LOCALSCREEN_TRANSVIEWPROJ:
		////case IT_CAMERA_WORLDPOSITION:
		////case IT_CAMERA_WORLDDIRECTION:
		////case IT_CAMERA_VIEW:
		////case IT_CAMERA_PROJECTION:
		////case IT_CAMERA_VIEWPROJ:
		////case IT_CAMERA_LINERPARAM:
		////case IT_LIGHT_POSITION:
		////case IT_LIGHT_ANGLE:
		////case IT_LIGHT_INNER_DIFF_INV:
		////case IT_LIGHT_RANGE_INV:
		////case IT_LIGHT_ATTENUATION:
		////case IT_LIGHT_COLOR:
		////case IT_AMBIENT_COLOR:
		////case IT_MATERIAL_AMBIENT:
		////case IT_MATERIAL_DIFFUSE:
		////case IT_MATERIAL_SPECULAR:
		////case IT_MATERIAL_SPECULAR_LEVEL:
		////case IT_MATERIAL_SHININESS:
		////case IT_LIGHT_MATERIAL_AMBIENT:
		////case IT_LIGHT_MATERIAL_LIGHT:
		////case IT_LIGHT_MATERIAL_SPECULAR:
		////case IT_TEXTURE_AMBIENT:
		////case IT_TEXTURE_DIFFUSE:
		////case IT_TEXTURE_SPECULAR:
		////case IT_TEXTURE_SPLEVEL:
		////case IT_TEXTURE_GLOSSINESS:
		////case IT_TEXTURE_ILLUMINATION:
		////case IT_TEXTURE_OPACITY:
		////case IT_TEXTURE_FITER:
		////case IT_TEXTURE_BUMP:
		////case IT_TEXTURE_REFLECTION:
		////case IT_TEXTURE_REFRACTION:
		////case IT_TEXTURE_DISPLACEMENT:
		////case IT_TEXTURE_TERRAIN_WEIGHT:
		////case IT_TEXTURE_TERRAIN_DETAL:
		////case IT_TEXTURE_TERRAIN_BASE:
		////case IT_TEXTURE_TERRAIN_R:
		////case IT_TEXTURE_TERRAIN_G:
		////case IT_TEXTURE_TERRAIN_B:
		////case IT_TEXTURE_USER0:
		////case IT_TEXTURE_USER1:
		////case IT_TEXTURE_USER2:
		////case IT_TEXTURE_USER3:
		////case IT_TEXTURE_USER4:
		////case IT_TEXTURE_USER5:
		////case IT_TEXTURE_USER6:
		////case IT_TEXTURE_USER7:
		////case IT_MATERIAL_USE_0:
		////case IT_MATERIAL_USE_1:
		////case IT_MATERIAL_USE_2:
		////case IT_MATERIAL_USE_3:
		////case IT_MATERIAL_USE_4:
		////case IT_MATERIAL_USE_5:
		////case IT_MATERIAL_USE_6:
		////case IT_MATERIAL_USE_7:
		////case IT_MATERIAL_USE_8:
		////case IT_MATERIAL_USE_9: res = "uniform ";break;
		////case IT_SURFACE_COLOR://这个必须计算，不能外部传递
		////case IT_LIGHTING_DIFFUSE:
		////case IT_LIGHTING_SPECULAR:
		//default: ASSERT(false && "unknown input type!");//如果定义了新的属性必须修改这里
		//}
		res = "varying ";
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
		case VT_VEC4_ARRAY:		res += "[" + m_ArraySize + "];\r\n"; break;
		default:ASSERT(false && "unknown variables type!" );
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string ShaderParameter::AsVariables() const
	{
		string res = "\t";
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
		case VT_TEXTURECUBE:	break;
		case VT_VEC2_ARRAY:
		case VT_VEC3_ARRAY:
		case VT_VEC4_ARRAY:		res += "[" + m_ArraySize + "]"; break;
		default:ASSERT(false && "unknown variables type!" );
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string ShaderParameter::AsVaryingEquation( const eastl::string& output ) const
	{
		string res;
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
		case VT_TEXTURECUBE:
			{
				res = "\t" + output + " = " + m_ParamName + ";\r\n";
			}break;
		case VT_VEC2_ARRAY:
		case VT_VEC3_ARRAY:
		case VT_VEC4_ARRAY:
			{
				res = "\tfor( int i = 0 ; i < " + m_ArraySize + " ; ++ i ){\r\n"
					+ "\t\t" + output + "[i] = " + m_ParamName + "[i];\r\n\t}\r\n";
			}break;
		default:ASSERT(false && "unknown variables type!" );
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string ShaderParameter::AsEquaToVarying( const eastl::string& intput ) const
	{
		string res;
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
		case VT_TEXTURECUBE:
			{
				res = "\t" + m_ParamName + " = " + intput + ";\r\n";
			}break;
		case VT_VEC2_ARRAY:
		case VT_VEC3_ARRAY:
		case VT_VEC4_ARRAY:
			{
				res = "\tfor( int i = 0 ; i < " + m_ArraySize + " ; ++ i ){\r\n"
					+ "\t\t" + m_ParamName + "[i] = " + intput + "[i];\r\n\t}\r\n";
			}break;
		default:ASSERT(false && "unknown variables type!" );
		}
		return res;
	}

}