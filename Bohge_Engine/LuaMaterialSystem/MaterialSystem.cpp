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
#include "Framework/MaterialSource.h"
#include "Framework/RenderState.h"
#include "Framework/Utility.hpp"
#include "Framework/Framework.h"
#include "Framework/Log.h"
#include "MaterialSystem.h"
#include "ShaderParser.h"
#include "ScriptParser.h"

#include "lua_tinker.hpp"


#define  MAKE_STRING_ENUM(x,y) #y , x##y
#define BOHGE_MATERIAL_EXPAND ".Material"

using namespace eastl;

static string MATERIAL_DEFINE("#MATERIAL_DEFINE");
static string END_DEFINE("#END_DEFINE");
static string PIPELINE_TYPE_SUFFIX[] = {
	"_FORWARD_SINGLE",
	"_FORWARD_MULTI",
	"_DEFFERRED",
};
static string PIPELINE_PASS_TYPE[] = {
	"#LIGHTING_AMBIENT",
	"#LIGHTING_DIRECTION",
	"#LIGHTING_POINT",
	"#LIGHTING_SPOT",
	"#UNIVERSAL_DEPTH",
	"#UNIVERSAL_NORMAL",
	"#UNIVERSAL_DECAL",
	"#UNIVERSAL_MRT",
	"#UNIVERSAL_POSTEFFECT"
};


using namespace BohgeEngine;
namespace LuaMaterialSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	MaterialSystem::MaterialSystem(void)
		:m_CurrentPassType(static_cast<PipelineProperty::PassType>(-1)),
		m_pScriptParser( NULL ),
		m_pPassTypeNameArray(NULL),
		m_pPassFunctionName(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	MaterialSystem::~MaterialSystem(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	MaterialProperty::Blend MaterialSystem::_MaterialConstant2Blend( const Material_Parser_Constant& mc ) const
	{
		switch( mc )
		{
		case MPC_ZERO: return MaterialProperty::BL_ZERO;
		case MPC_ONE: return MaterialProperty::BL_ONE;
		case MPC_SRC_COLOR: return MaterialProperty::BL_SRC_COLOR;
		case MPC_ONE_MINUS_SRC_COLOR: return MaterialProperty::BL_ONE_MINUS_SRC_COLOR;
		case MPC_SRC_ALPHA: return MaterialProperty::BL_SRC_ALPHA;
		case MPC_ONE_MINUS_SRC_ALPHA: return MaterialProperty::BL_ONE_MINUS_SRC_ALPHA;
		case MPC_DST_ALPHA: return MaterialProperty::BL_DST_ALPHA;
		case MPC_ONE_MINUS_DST_ALPHA: return MaterialProperty::BL_ONE_MINUS_DST_ALPHA;
		case MPC_DST_COLOR: return MaterialProperty::BL_DST_COLOR;
		case MPC_ONE_MINUS_DST_COLOR: return MaterialProperty::BL_ONE_MINUS_DST_COLOR;
		default:ASSERT(false);
		}
		return MaterialProperty::BL_ONE;
	}
	//-------------------------------------------------------------------------------------------------------
	MaterialProperty::Function MaterialSystem::_MaterialConstant2Function( const Material_Parser_Constant& mc ) const
	{
		switch( mc )
		{
		case MPC_DEPTH_FUNCTION_NEVER: return MaterialProperty::FN_NEVER;
		case MPC_DEPTH_FUNCTION_LESS: return MaterialProperty::FN_LESS;
		case MPC_DEPTH_FUNCTION_GREATER: return MaterialProperty::FN_GEQUAL;
		case MPC_DEPTH_FUNCTION_ALWAYS: return MaterialProperty::FN_ALWAYS;

		case MPC_STENCIL_FUNCTION_NEVER: return MaterialProperty::FN_NEVER;
		case MPC_STENCIL_FUNCTION_LESS: return MaterialProperty::FN_LESS;
		case MPC_STENCIL_FUNCTION_EQUAL:return MaterialProperty::FN_EQUAL;
		case MPC_STENCIL_FUNCTION_LEQUAL:return MaterialProperty::FN_LEQUAL;
		case MPC_STENCIL_FUNCTION_GREATER: return MaterialProperty::FN_GREATER;
		case MPC_STENCIL_FUNCTION_NOTEQUAL:return MaterialProperty::FN_NOTEQUAL;
		case MPC_STENCIL_FUNCTION_GEQUAL:return MaterialProperty::FN_GEQUAL;
		case MPC_STENCIL_FUNCTION_ALWAYS:return MaterialProperty::FN_ALWAYS;
		default:ASSERT(false);
		}
		return MaterialProperty::FN_LESS;
	}
	//-------------------------------------------------------------------------------------------------------
	MaterialProperty::CullFaceMode MaterialSystem::_MaterialConstant2CullFaceMode( const Material_Parser_Constant& mc ) const
	{
		switch( mc )
		{
		case MPC_CULL_FACE_NONE: return MaterialProperty::CFM_NONE;
		case MPC_CULL_FACE_BACK: return MaterialProperty::CFM_BACK;
		case MPC_CULL_FACE_FRONT: return MaterialProperty::CFM_FRONT;
		default:ASSERT(false);
		}
		return MaterialProperty::CFM_NONE;
	}
	//-------------------------------------------------------------------------------------------------------
	MaterialProperty::Operation MaterialSystem::_MaterialConstant2Operation( const Material_Parser_Constant& mc ) const
	{
		switch( mc )
		{
		case MPC_STENCIL_OPERATION_ZERO: return MaterialProperty::ON_ZERO;
		case MPC_STENCIL_OPERATION_ONE: return MaterialProperty::ON_ONE;
		case MPC_STENCIL_OPERATION_KEEP: return MaterialProperty::ON_KEEP;
		case MPC_STENCIL_OPERATION_REPLACE: return MaterialProperty::ON_REPLACE;
		case MPC_STENCIL_OPERATION_INCR: return MaterialProperty::ON_INCR;
		case MPC_STENCIL_OPERATION_DECR: return MaterialProperty::ON_DECR;
		case MPC_STENCIL_OPERATION_INVERT: return MaterialProperty::ON_INVERT;
		case MPC_STENCIL_OPERATION_INCR_WRAP: return MaterialProperty::ON_INCR_WRAP;
		case MPC_STENCIL_OPERATION_DECR_WRAP: return MaterialProperty::ON_DECR_WRAP;
		default:ASSERT(false);
		}
		return MaterialProperty::ON_ZERO;
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialSystem::_SetTextureGlobal( const char* name, MaterialProperty::Material_Attribute ma )
	{
		m_TextureSLotNameMap.insert( make_pair( ma, name ));
		m_pScriptParser->SetGlobal( name, ma );
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialSystem::_OnPipelineTypeSet()
	{
		ASSERT( 9 == PipelineProperty::PT_PASS_TYPE_COUNT );
		m_pPassTypeNameArray[PipelineProperty::PT_LIGHTING_AMBIENT]
		= PIPELINE_PASS_TYPE[PipelineProperty::PT_LIGHTING_AMBIENT] + PIPELINE_TYPE_SUFFIX[GetPipelienType()];
		m_pPassTypeNameArray[PipelineProperty::PT_LIGHTING_DIRECTION]
			= PIPELINE_PASS_TYPE[PipelineProperty::PT_LIGHTING_DIRECTION] + PIPELINE_TYPE_SUFFIX[GetPipelienType()];
		m_pPassTypeNameArray[PipelineProperty::PT_LIGHTING_POINT]
			= PIPELINE_PASS_TYPE[PipelineProperty::PT_LIGHTING_POINT] + PIPELINE_TYPE_SUFFIX[GetPipelienType()];
		m_pPassTypeNameArray[PipelineProperty::PT_LIGHTING_SPOT]
			= PIPELINE_PASS_TYPE[PipelineProperty::PT_LIGHTING_SPOT] + PIPELINE_TYPE_SUFFIX[GetPipelienType()];
		m_pPassTypeNameArray[PipelineProperty::PT_UNIVERSAL_DEPTH] = PIPELINE_PASS_TYPE[PipelineProperty::PT_UNIVERSAL_DEPTH];
		m_pPassTypeNameArray[PipelineProperty::PT_UNIVERSAL_NORMAL] = PIPELINE_PASS_TYPE[PipelineProperty::PT_UNIVERSAL_NORMAL];
		m_pPassTypeNameArray[PipelineProperty::PT_UNIVERSAL_DECAL] = PIPELINE_PASS_TYPE[PipelineProperty::PT_UNIVERSAL_DECAL];
		m_pPassTypeNameArray[PipelineProperty::PT_UNIVERSAL_MULTI_RENDER_TARGET] = PIPELINE_PASS_TYPE[PipelineProperty::PT_UNIVERSAL_MULTI_RENDER_TARGET];
		m_pPassTypeNameArray[PipelineProperty::PT_UNIVERSAL_POST_EFFECT] = PIPELINE_PASS_TYPE[PipelineProperty::PT_UNIVERSAL_POST_EFFECT];
	}
	//-------------------------------------------------------------------------------------------------------
	PipelineProperty::ActivePassTypeArray MaterialSystem::ParseMaterial( const eastl::string& path, eastl::string& matstr )
	{
		string luaSource;
		vector<string> funcs;
		PipelineProperty::ActivePassTypeArray apta;
		Utility::SeparateAllString( MATERIAL_DEFINE, END_DEFINE, funcs, matstr );//将材质分拆开来
		for( int i = 0; i < funcs.size() ; ++ i )
		{
			Utility::MakeLuaFunction( MATERIAL_DEFINE, END_DEFINE, funcs[i] );//每个材质做成函数
			luaSource += "\r\n" + funcs[i];
		}
		m_pScriptParser->DoScript( luaSource );//加载lua脚本
		m_CurrentPassType = static_cast<BohgeEngine::PipelineProperty::PassType>(-1);

		const int passCount = IMaterialSystem::Instance()->GetActivePassTypeCount();
		for ( int i = 0 ; i < passCount ; ++ i )
		{
			PipelineProperty::PassType pt = IMaterialSystem::Instance()->GetPassType( i );
			if( Utility::MacroName( m_pPassTypeNameArray[pt], matstr, m_pPassFunctionName[pt] ) )
			{
				apta.push_back( pt );
			}
			else
			{
				m_pPassFunctionName[pt].clear();
				DEBUGLOG("empty %s pass function in file %s!\n", m_pPassTypeNameArray[pt].c_str(), path.c_str() );
			}
		}
		return apta;
	}
	//-------------------------------------------------------------------------------------------------------
	const eastl::string& MaterialSystem::GetPassShaderName( PipelineProperty::PassType type )
	{
		return m_pPassFunctionName[type];
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialSystem::FlushRenderState( PipelineProperty::PassType type, RenderState& state )
	{
		if ( !m_pPassFunctionName[type].empty() )
		{
			_DoCallPassTypeFunction(type);
			{//处理队列
				uint que = m_pScriptParser->GetGlobal<uint>( "RENDER_QUEUE" );
				state.SetRenderQueue( que );
			}
			{//处理alpha
				LuaTable alpha = m_pScriptParser->GetGlobal<LuaTable>( "ALPAH_MODE" );
				Material_Parser_Constant mode = alpha.get<Material_Parser_Constant>(1);
				switch( mode )
				{
				case MPC_ALPAH_OFF: state.SetAlphaBlend(false); state.SetAlphaTest(false); break;
				case MPC_ALPAH_COVERAGE: state.SetAlphaBlend(false); state.SetAlphaTest(true); break;
				case MPC_ALPAH_BLEND:
					{
						state.SetAlphaBlend(true);
						state.SetAlphaTest(false);
						state.SetBlendFunction(
							_MaterialConstant2Blend(alpha.get<Material_Parser_Constant>(2)),
							_MaterialConstant2Blend(alpha.get<Material_Parser_Constant>(3)),
							_MaterialConstant2Blend(alpha.get<Material_Parser_Constant>(4)),
							_MaterialConstant2Blend(alpha.get<Material_Parser_Constant>(5))
							);
					}break;
				default:ASSERT(false);
				}
			}

			{//处理draw
				LuaTable darw = m_pScriptParser->GetGlobal<LuaTable>( "DRAW_MODE" );
				Material_Parser_Constant mode = darw.get<Material_Parser_Constant>(1);
				state.SetCullFace( _MaterialConstant2CullFaceMode( mode ) );				
				mode = darw.get<Material_Parser_Constant>(2);
				state.SetDepthMask( MPC_DEPTH_MASK_OFF == mode ? false : true );
				mode = darw.get<Material_Parser_Constant>(3);
				state.SetDepthTest( MPC_DEPTH_TEST_OFF == mode ? false : true );
				if ( state.GetDepthTest() )
				{
					mode = darw.get<Material_Parser_Constant>(4);
					state.SetDepthFunciton( _MaterialConstant2Function( mode ) );
				}
			}

			{//color
				Material_Parser_Constant color = m_pScriptParser->GetGlobal<Material_Parser_Constant>( "COLOR_MASK" );
				switch( color )
				{
				case MPC_COLOR_NONE: state.SetColorMask(false,false,false,false);break;
				case MPC_COLOR_RGB:	 state.SetColorMask(true,true,true,false);break;
				case MPC_COLOR_RGBA: state.SetColorMask(true,true,true,true);break;
				default:ASSERT(false);break;
				}
			}

			{//stencil
				LuaTable stencil = m_pScriptParser->GetGlobal<LuaTable>( "STENCIL_MODE" );
				Material_Parser_Constant mode = stencil.get<Material_Parser_Constant>(1);
				state.SetStencil( MPC_STENCIL_OFF == mode ? false : true );
				if ( MPC_STENCIL_ON == mode )
				{
					state.SetStencilMask( stencil.get<uint>(2) );
					state.SetStencilFunction(
						_MaterialConstant2Function( stencil.get<Material_Parser_Constant>(3) ),
						stencil.get<uint>(4),
						stencil.get<uint>(5)
						);
					state.SetStencilOperation(
						_MaterialConstant2Operation( stencil.get<Material_Parser_Constant>(6) ),
						_MaterialConstant2Operation( stencil.get<Material_Parser_Constant>(7) ),
						_MaterialConstant2Operation( stencil.get<Material_Parser_Constant>(8) )
						);
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::vector<eastl::string> MaterialSystem::GetVertexSegmentNames( BohgeEngine::PipelineProperty::PassType type )
	{
		eastl::vector<eastl::string> segNames;
		_DoCallPassTypeFunction(type);
		bool isProviderCoordinate = false;
		LuaTable vs = m_pScriptParser->GetGlobal<LuaTable>( "VERTEX_SHADER" );
		segNames.reserve( vs.size() );
		for ( int i = 1; i <= vs.size(); i ++ )
		{
			segNames.push_back( vs.get<char*>(i) );
		}
		return segNames;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::vector<eastl::string> MaterialSystem::GetFragmentegmentNames( BohgeEngine::PipelineProperty::PassType type )
	{
		eastl::vector<eastl::string> segNames;
		_DoCallPassTypeFunction(type);
		bool isProviderCoordinate = false;
		LuaTable vs = m_pScriptParser->GetGlobal<LuaTable>( "FRAGMENT_SHADER" );
		segNames.reserve( vs.size() );
		for ( int i = 1; i <= vs.size(); i ++ )
		{
			segNames.push_back( vs.get<char*>(i) );
		}
		return segNames;
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialSystem::_DoCallPassTypeFunction( BohgeEngine::PipelineProperty::PassType type )
	{
		if ( type != m_CurrentPassType )
		{
			m_CurrentPassType = type;
			m_pScriptParser->ParserCall( m_pPassFunctionName[type].c_str() );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialSystem::Parser( const eastl::string& root, const StringList& includes, const StringList& vs, const StringList& ps, ShaderSource& shader )
	{
		ShaderParser::Instance()->Parser( root, includes, vs, ps, shader );
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialSystem::RegistEvent( Framework& framework )
	{
		IMaterialSystem::RegistEvent( framework );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_ENTRY, MEMBER_FUNC_PTR( &MaterialSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_EXIT, MEMBER_FUNC_PTR( &MaterialSystem::OnFrameworkExit ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialSystem::OnFrameworkEntry( float time, BohgeEngine::Framework& framework )
	{
		m_pPassFunctionName = NEW string[BohgeEngine::PipelineProperty::PT_PASS_TYPE_COUNT];
		m_pPassTypeNameArray = NEW string[BohgeEngine::PipelineProperty::PT_PASS_TYPE_COUNT];
		m_pScriptParser = NEW ScriptParser;
		ShaderParser::Create( );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MaterialProperty::MRQ_, BACKGROUND) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MaterialProperty::MRQ_, OPAQUE) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MaterialProperty::MRQ_, TRANSPARENT) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MaterialProperty::MRQ_, POSTEFFECT) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MaterialProperty::MRQ_, OVERLAY) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, ALPAH_OFF) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, ALPAH_COVERAGE) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, ALPAH_BLEND) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, ZERO) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, ONE) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, SRC_COLOR) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, ONE_MINUS_SRC_COLOR) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, SRC_ALPHA) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, ONE_MINUS_SRC_ALPHA) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, DST_ALPHA) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, ONE_MINUS_DST_ALPHA) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, DST_COLOR) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, ONE_MINUS_DST_COLOR) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, DEPTH_TEST_ON) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, DEPTH_TEST_OFF) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, DEPTH_MASK_ON) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, DEPTH_MASK_OFF) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, DEPTH_FUNCTION_NEVER) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, DEPTH_FUNCTION_LESS) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, DEPTH_FUNCTION_GREATER) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, DEPTH_FUNCTION_ALWAYS) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, CULL_FACE_NONE) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, CULL_FACE_BACK) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, CULL_FACE_FRONT) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, COLOR_NONE) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, COLOR_RGBA) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, COLOR_RGB) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_ON) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_OFF) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_FUNCTION_NEVER) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_FUNCTION_LESS) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_FUNCTION_EQUAL) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_FUNCTION_LEQUAL) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_FUNCTION_GREATER) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_FUNCTION_NOTEQUAL) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_FUNCTION_GEQUAL) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_FUNCTION_ALWAYS) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_OPERATION_ZERO) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_OPERATION_ONE) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_OPERATION_KEEP) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_OPERATION_REPLACE) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_OPERATION_INCR) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_OPERATION_DECR) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_OPERATION_INVERT) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_OPERATION_INCR_WRAP) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( MPC_, STENCIL_OPERATION_DECR_WRAP) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::PF_, AUTO) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::PF_, A8) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::PF_, L8) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::PF_, R8G8B8) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::PF_, R5G6B5) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::PF_, R8G8B8A8) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::PF_, R4G4B4A4) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::PF_, DEPTH16) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::PF_, DEPTH24_STENCIL8) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::TW_, REPEAT) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::TW_, CLAMP_TO_EDGE) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::TW_, MIRRORED_REPEAT) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::TF_, NEAREST) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::TF_, LINEAR) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::TF_, NEAREST_MIPMAP_NEAREST) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::TF_, LINEAR_MIPMAP_NEAREST) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::TF_, NEAREST_MIPMAP_LINEAR) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( TextureProperty::TF_, LINEAR_MIPMAP_LINEAR) );

		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_AMBIENT) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_DIFFUSE) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_SPECULAR) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_SPLEVEL) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_GLOSSINESS) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_ILLUMINATION) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_OPACITY) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_FITER) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_BUMP) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_REFLECTION) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_REFRACTION) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_DISPLACEMENT) );

		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_TERRAIN_WEIGHT) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_TERRAIN_DETAL) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_TERRAIN_BASE) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_TERRAIN_R) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_TERRAIN_G) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_TERRAIN_B) );

		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_USER0) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_USER1) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_USER2) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_USER3) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_USER4) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_USER5) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_USER6) );
		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_USER7) );

		_SetTextureGlobal( MAKE_STRING_ENUM( MaterialProperty::ATT_, TEXTURE_LIGHT_MAP) );
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialSystem::OnFrameworkExit( float time, BohgeEngine::Framework& framework )
	{
		SAFE_DELETE_ARRAY( m_pPassFunctionName );
		SAFE_DELETE_ARRAY( m_pPassTypeNameArray );
		SAFE_DELETE(m_pScriptParser);
		m_TextureSLotNameMap.clear();
		ShaderParser::Destroy();
	}
//-------------------------------------------------------------------------------------------------------
	//MaterialProperty::TextureSlotPropertyMap MaterialParser::GetTextureSlotStatus( Pipeline::PassType type )
	//{
	//	MaterialProperty::TextureSlotPropertyMap res;
	//	_DoCallPassTypeFunction(type);
	//	LuaTable ts = m_ScriptParser->GetLuaTable( "TEXTURE_SLOT" );
	//	for ( int i = 1; i <= ts.size(); i ++ )
	//	{
	//		MaterialProperty::TextureSlotProperty slot;
	//		MaterialProperty::Material_Attribute ma = ts.get<MaterialProperty::Material_Attribute>(i);
	//		LuaTable param = m_ScriptParser->GetLuaTable( m_TextureSLotNameMap[ma].c_str() );
	//		slot.m_Warp.m_SWarp = param.get<TextureProperty::TextureWarp>(1);
	//		slot.m_Warp.m_TWarp = param.get<TextureProperty::TextureWarp>(2);
	//		slot.m_Filter.m_MagFilter = param.get<TextureProperty::TextureFilter>(3);
	//		slot.m_Filter.m_MagFilter = param.get<TextureProperty::TextureFilter>(4);
	//		slot.m_PixelFormat = param.get<TextureProperty::PixelFormat>(5);
	//		slot.m_Anisotropic = param.get<uint>(6);
	//		res.insert( make_pair( ma, slot) );
	//	}
	//	return res;
	//}

}