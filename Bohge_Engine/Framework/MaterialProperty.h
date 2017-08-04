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
#pragma once
#include "Predefine.h"
#include "TextureProperty.h"



namespace BohgeEngine
{
	class MaterialProperty : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( MaterialProperty )
	private:
		MaterialProperty(void){}
		~MaterialProperty(void){}
	public:
		enum RenderQueue
		{
			MRQ_BACKGROUND			= 0,
			MRQ_OPAQUE				= 1000,
			MRQ_TRANSPARENT_CUTOUT	= 2000,
			MRQ_TRANSPARENT			= 3000,
			MRQ_POSTEFFECT			= 4000,
			MRQ_OVERLAY				= 5000,
		};
		enum UserParameterType
		{
			UPT_VEC1 = 0,
			UPT_VEC2,
			UPT_VEC3,
			UPT_VEC4,
			UPT_MAT3,
			UPT_MAT4,

			UPT_VEC2_ARRAY,
			UPT_VEC3_ARRAY,
			UPT_VEC4_ARRAY,
			UPT_COUNT,
		};
		enum Function
		{
			FN_NEVER = 0,
			FN_LESS,
			FN_EQUAL,
			FN_LEQUAL,
			FN_GREATER,
			FN_NOTEQUAL,
			FN_GEQUAL,
			FN_ALWAYS,
		};
		enum CullFaceMode
		{
			CFM_NONE = 0,
			CFM_FRONT,
			CFM_BACK,
		};
		enum Operation
		{
			ON_ZERO = 0,
			ON_ONE,
			ON_KEEP,
			ON_REPLACE,
			ON_INCR,
			ON_DECR,
			ON_INVERT,
			ON_INCR_WRAP,
			ON_DECR_WRAP,
		};
		enum Blend
		{
			BL_ZERO = 0,
			BL_ONE,
			BL_SRC_COLOR,
			BL_ONE_MINUS_SRC_COLOR,
			BL_SRC_ALPHA,
			BL_ONE_MINUS_SRC_ALPHA,
			BL_DST_ALPHA,
			BL_ONE_MINUS_DST_ALPHA ,
			BL_DST_COLOR,
			BL_ONE_MINUS_DST_COLOR,
		};
		enum Material_Attribute	//材质包含的子项,模型输出的时候导出
		{
			ATT_AMBIENT = 1001,//环境光
			ATT_DIFFUSE,//满反射
			ATT_SPECULAR,//反射
			ATT_EMIT,
			ATT_OPCITY,//是否透明
			ATT_SPECULAR_LEVEL,//反射强度
			ATT_SHININESS,
			ATT_TEXTURES = 1501,//材质贴图开始输出
				ATT_TEXTURE_UNKONW,//未知,缩进的意思是这个属于ATT_TEXTURES的字属性
				ATT_TEXTURE_AMBIENT,//环境贴图
				ATT_TEXTURE_DIFFUSE,//纹理属性
				ATT_TEXTURE_SPECULAR,//反射颜色
				ATT_TEXTURE_SPLEVEL,//发射等级
				ATT_TEXTURE_GLOSSINESS,//发光
				ATT_TEXTURE_ILLUMINATION,//自发光
				ATT_TEXTURE_OPACITY,//透明贴图
				ATT_TEXTURE_FITER,//过滤贴图
				ATT_TEXTURE_BUMP,//法线贴图
				ATT_TEXTURE_REFLECTION,//发射贴图
				ATT_TEXTURE_REFRACTION,//折射贴图
				ATT_TEXTURE_DISPLACEMENT,//视察贴图

				ATT_TEXTURE_TERRAIN_WEIGHT,//地形的贴图
				ATT_TEXTURE_TERRAIN_DETAL,
				ATT_TEXTURE_TERRAIN_BASE,
				ATT_TEXTURE_TERRAIN_R,
				ATT_TEXTURE_TERRAIN_G,
				ATT_TEXTURE_TERRAIN_B,

				ATT_TEXTURE_USER0,//用户自定义的
				ATT_TEXTURE_USER1,//用户自定义的
				ATT_TEXTURE_USER2,//用户自定义的
				ATT_TEXTURE_USER3,//用户自定义的
				ATT_TEXTURE_USER4,//用户自定义的
				ATT_TEXTURE_USER5,//用户自定义的
				ATT_TEXTURE_USER6,//用户自定义的
				ATT_TEXTURE_USER7,//用户自定义的

				//新增的必须放在后面
				ATT_TEXTURE_LIGHT_MAP,//light map

				ATT_TEXTURE_END,//最后一个
				ATT_TEXTURE_COUNT = ATT_TEXTURE_END - ATT_TEXTURES - 1,

				ATT_MATERIAL_USE_0 = ATT_TEXTURE_END + 1,//用户自定义的
				ATT_MATERIAL_USE_1,//用户自定义的
				ATT_MATERIAL_USE_2,//用户自定义的
				ATT_MATERIAL_USE_3,//用户自定义的
				ATT_MATERIAL_USE_4,//用户自定义的
				ATT_MATERIAL_USE_5,//用户自定义的
				ATT_MATERIAL_USE_6,//用户自定义的
				ATT_MATERIAL_USE_7,//用户自定义的
				ATT_MATERIAL_USE_8,//用户自定义的
				ATT_MATERIAL_USE_9,//用户自定义的
				ATT_MATERIAL_USE_END,
				ATT_MATERIAL_USE_COUNT = ATT_MATERIAL_USE_END - ATT_MATERIAL_USE_0,

			ATTRIBUTE_END = ATT_AMBIENT + 999, //材质属性结束
		};
		//struct TextureSlotProperty
		//{
		//	TextureProperty::WarpType			m_Warp;
		//	TextureProperty::FilterType			m_Filter;
		//	TextureProperty::PixelFormat		m_PixelFormat;
		//	int									m_Anisotropic;
		//	TextureSlotProperty():m_PixelFormat(TextureProperty::PixelFormat::PF_AUTO){}
		//};
		//typedef eastl::map< Material_Attribute, TextureSlotProperty > TextureSlotPropertyMap;

		struct UserParameter
		{
			MaterialProperty::UserParameterType		m_eUserParameterType;
			const Object*								m_pObject;
			bool									m_isReference;
			UserParameter( void )
				:m_isReference(false),
				m_eUserParameterType( MaterialProperty::UPT_COUNT ),
				m_pObject(NULL)
			{
			}
			~UserParameter( void )
			{
				if ( !m_isReference )
				{
					SAFE_DELETE( m_pObject );
				}
			}
		};
	};
}