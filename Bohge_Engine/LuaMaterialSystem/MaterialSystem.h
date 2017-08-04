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
#include "Framework/MaterialProperty.h"
#include "Framework/PipelineProperty.h"
#include "Framework/IMaterialSystem.h"
#include "Framework/RenderState.h"



#include <EASTL/string.h>
#include <EASTL/list.h>
#include <EASTL/map.h>
#include <EASTL/vector.h>


namespace LuaMaterialSystemPlugin
{
	class ScriptParser;
	class MaterialSystem : public BohgeEngine::IMaterialSystem
	{
	private:
		enum Material_Parser_Constant
		{
			//alpha混合相关
			MPC_ERRORCODE = 0,//Lua得不到数据的时候会设置数据为0
			MPC_ALPAH_OFF,
			MPC_ALPAH_COVERAGE,
			MPC_ALPAH_BLEND,
			MPC_ZERO,
			MPC_ONE,
			MPC_SRC_COLOR,
			MPC_ONE_MINUS_SRC_COLOR,
			MPC_SRC_ALPHA,
			MPC_ONE_MINUS_SRC_ALPHA,
			MPC_DST_ALPHA,
			MPC_ONE_MINUS_DST_ALPHA ,
			MPC_DST_COLOR,
			MPC_ONE_MINUS_DST_COLOR,

			//渲染状态相关
			MPC_DEPTH_TEST_ON,
			MPC_DEPTH_TEST_OFF,
			MPC_DEPTH_MASK_ON,
			MPC_DEPTH_MASK_OFF,
			MPC_DEPTH_FUNCTION_NEVER,
			MPC_DEPTH_FUNCTION_LESS,
			MPC_DEPTH_FUNCTION_GREATER,
			MPC_DEPTH_FUNCTION_ALWAYS,
			MPC_CULL_FACE_NONE,
			MPC_CULL_FACE_BACK,
			MPC_CULL_FACE_FRONT,

			//Color Mask
			MPC_COLOR_NONE,
			MPC_COLOR_RGBA,
			MPC_COLOR_RGB,

			//Stencil
			MPC_STENCIL_ON,
			MPC_STENCIL_OFF,
			MPC_STENCIL_FUNCTION_NEVER,
			MPC_STENCIL_FUNCTION_LESS,
			MPC_STENCIL_FUNCTION_EQUAL,
			MPC_STENCIL_FUNCTION_LEQUAL,
			MPC_STENCIL_FUNCTION_GREATER,
			MPC_STENCIL_FUNCTION_NOTEQUAL,
			MPC_STENCIL_FUNCTION_GEQUAL,
			MPC_STENCIL_FUNCTION_ALWAYS,
			MPC_STENCIL_OPERATION_ZERO,
			MPC_STENCIL_OPERATION_ONE,
			MPC_STENCIL_OPERATION_KEEP,
			MPC_STENCIL_OPERATION_REPLACE,
			MPC_STENCIL_OPERATION_INCR,
			MPC_STENCIL_OPERATION_DECR,
			MPC_STENCIL_OPERATION_INVERT,
			MPC_STENCIL_OPERATION_INCR_WRAP,
			MPC_STENCIL_OPERATION_DECR_WRAP,
		};
	private:
		typedef eastl::map< BohgeEngine::MaterialProperty::Material_Attribute, eastl::string > TextureSlotNameMap;
	private:
		TextureSlotNameMap							m_TextureSLotNameMap;
		eastl::string*								m_pPassFunctionName;
		eastl::string*								m_pPassTypeNameArray;
		ScriptParser*								m_pScriptParser;
		BohgeEngine::PipelineProperty::PassType		m_CurrentPassType;
	public:
		MaterialSystem(void);
		~MaterialSystem(void);
	private:
		BohgeEngine::MaterialProperty::Blend _MaterialConstant2Blend( const Material_Parser_Constant& mc ) const;
		BohgeEngine::MaterialProperty::Function _MaterialConstant2Function( const Material_Parser_Constant& mc ) const;
		BohgeEngine::MaterialProperty::Operation _MaterialConstant2Operation( const Material_Parser_Constant& mc ) const;
		BohgeEngine::MaterialProperty::CullFaceMode _MaterialConstant2CullFaceMode( const Material_Parser_Constant& mc ) const;
		void _SetTextureGlobal( const char* name, BohgeEngine::MaterialProperty::Material_Attribute ma );
		void _DoCallPassTypeFunction( BohgeEngine::PipelineProperty::PassType type );
	private:
		virtual void _OnPipelineTypeSet();
	public:
		BohgeEngine::PipelineProperty::ActivePassTypeArray ParseMaterial( const eastl::string& path, eastl::string& matstr );
		//MaterialProperty::TextureSlotPropertyMap GetTextureSlotStatus( Pipeline::PassType type );//返回需要用到的纹理的
		void FlushRenderState( BohgeEngine::PipelineProperty::PassType type, BohgeEngine::RenderState& state );//返回某个pipline的材质属性
		eastl::vector<eastl::string> GetVertexSegmentNames( BohgeEngine::PipelineProperty::PassType type );
		eastl::vector<eastl::string> GetFragmentegmentNames( BohgeEngine::PipelineProperty::PassType type );
		void Parser( const eastl::string& root, const StringList& includes, const StringList& vs, const StringList& ps, BohgeEngine::ShaderSource& shader );
		virtual const eastl::string& GetPassShaderName( BohgeEngine::PipelineProperty::PassType type );
	public:
		virtual void RegistEvent( BohgeEngine::Framework& framework ); //注册事件
		void OnFrameworkEntry( float time, BohgeEngine::Framework& framework );
		void OnFrameworkExit( float time, BohgeEngine::Framework& framework );
	};
}

