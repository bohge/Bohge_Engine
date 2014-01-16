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



/////////////////////////////////
//			材质编译			   //
/////////////////////////////////
#include "MaterialCompiler.h"
#include "Utility.h"
#include "Material.h"
#include "ShaderLibrary.h"
#include "ShaderSegment.h"
#include "ShaderAssembler.h"
#include "AssembledMaterialShader.h"
#include "TextureState.h"

using namespace std;

static string MATERIAL_DEFINE("#MATERIAL_DEFINE");
static string END_DEFINE("#END_DEFINE");
static string LIGHT_PASS("#LIGHT_PASS");
static string DEPTH_PASS("#DEPTH_PASS");
static string NORMAL_PASS("#NORMAL_PASS");

string BaseCoordinate("BaseCoordinate"); //在ShaderLibrary中被使用



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	MaterialCompiler::MaterialCompiler( )
	{
		m_pShaderLibrary = NEW ShaderLibrary();
		m_pShaderAssembler = NEW ShaderAssembler();
	}
	//-------------------------------------------------------------------------------------------------------
	MaterialCompiler::~MaterialCompiler( )
	{
		SAFE_DELETE( m_pShaderLibrary );
		SAFE_DELETE( m_pShaderAssembler );
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::_OnScriptLoaded()
	{
		ASSERT( false );//废弃
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::LoadScript( const std::string& filename )
	{
		ASSERT( false );//废弃
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::DoScript( const std::string& buffer )
	{
		lua_tinker::dostring( LuaState(), buffer.c_str() );
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::UnLoadScript()
	{
		_CloseLuaState();
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::ReleaseResource()
	{
		UnLoadScript();
		m_pShaderLibrary->ReleaseResource();
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::_SetGlobalClassesIndex()
	{
		SetGlobal("ALPAH_OFF", ALPAH_OFF );
		SetGlobal("ALPAH_COVERAGE", ALPAH_COVERAGE );
		SetGlobal("ALPAH_BLEND", ALPAH_BLEND );
		SetGlobal("ZERO", ZERO );
		SetGlobal("ONE", ONE );
		SetGlobal("SRC_COLOR", SRC_COLOR );
		SetGlobal("ONE_MINUS_SRC_COLOR", ONE_MINUS_SRC_COLOR );
		SetGlobal("SRC_ALPHA", SRC_ALPHA );
		SetGlobal("ONE_MINUS_SRC_ALPHA", ONE_MINUS_SRC_ALPHA );
		SetGlobal("DST_ALPHA", DST_ALPHA );
		SetGlobal("ONE_MINUS_DST_ALPHA ", ONE_MINUS_DST_ALPHA );
		SetGlobal("DST_COLOR", DST_COLOR );
		SetGlobal("ONE_MINUS_DST_COLOR", ONE_MINUS_DST_COLOR );
		SetGlobal("DEPTH_TEST_ON", DEPTH_TEST_ON );
		SetGlobal("DEPTH_TEST_OFF", DEPTH_TEST_OFF );
		SetGlobal("DEPTH_MASK_ON", DEPTH_MASK_ON );
		SetGlobal("DEPTH_MASK_OFF", DEPTH_MASK_OFF );
		SetGlobal("DEPTH_FUNCTION_NEVER", DEPTH_FUNCTION_NEVER );
		SetGlobal("DEPTH_FUNCTION_LESS", DEPTH_FUNCTION_LESS );
		SetGlobal("DEPTH_FUNCTION_GREATER", DEPTH_FUNCTION_GREATER );
		SetGlobal("DEPTH_FUNCTION_ALWAYS", DEPTH_FUNCTION_ALWAYS );
		SetGlobal("CULL_FACE_NONE", CULL_FACE_NONE );
		SetGlobal("CULL_FACE_BACK", CULL_FACE_BACK );
		SetGlobal("CULL_FACE_FRONT", CULL_FACE_FRONT );

		TextureState::RegisterShaderParameter( *this );
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::Initialization( )
	{
		_NewLuaState();
		_SetGlobalClassesIndex();
		m_pShaderLibrary->Initialization();
	}
	//-------------------------------------------------------------------------------------------------------
	Material* MaterialCompiler::CompilerMaterial( std::string& matstr )
	{
		//处理include
		string final;
		vector<string> funcs;
		vector<string> includes;
		Utility::FindIncludes( includes, matstr );//得到includ文件
		m_pShaderLibrary->Being();
		for( int i = 0; i < includes.size() ; i ++ )
		{
			m_pShaderLibrary->HandleShaderSegment( includes[i] );
		}
		Utility::SeparateAllString( MATERIAL_DEFINE, END_DEFINE, funcs, matstr );//将材质分拆开来
		for( int i = 0; i < funcs.size() ; i ++ )
		{
			Utility::MakeLuaFunction( MATERIAL_DEFINE, END_DEFINE, funcs[i] );//每个材质做成函数
			final += "\r\n" + funcs[i];
		}
		DoScript( final );
		Material* res = NEW Material();
		ASSERT( Pipeline::PT_COUNT == 3 && "if you add new type, plz modify here" );
		if( Utility::MacroName( LIGHT_PASS, matstr, final ) )
		{
			CallLua<void>( final.c_str() );
			_MaterialAlphaDrawState( Pipeline::PT_LIGHTING ,*res );
			_MaterialSahder( Pipeline::PT_LIGHTING ,*res );
		}
		if( Utility::MacroName( DEPTH_PASS, matstr, final ) )
		{
			CallLua<void>( final.c_str() );
			_MaterialAlphaDrawState( Pipeline::PT_DEPTH ,*res );
			_MaterialSahder( Pipeline::PT_DEPTH ,*res );
		}
		if( Utility::MacroName( NORMAL_PASS, matstr, final ) )
		{
			CallLua<void>( final.c_str() );
			_MaterialAlphaDrawState( Pipeline::PT_NORMAL ,*res );
			_MaterialSahder( Pipeline::PT_NORMAL ,*res );
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	Device::Blend MaterialCompiler::_MaterialConstant2Blend( const Material_Constant& mc ) const
	{
		switch( mc )
		{
		case ZERO: return Device::BL_ZERO;
		case ONE: return Device::BL_ONE;
		case SRC_COLOR: return Device::BL_SRC_COLOR;
		case ONE_MINUS_SRC_COLOR: return Device::BL_ONE_MINUS_SRC_COLOR;
		case SRC_ALPHA: return Device::BL_SRC_ALPHA;
		case ONE_MINUS_SRC_ALPHA: return Device::BL_ONE_MINUS_SRC_ALPHA;
		case DST_ALPHA: return Device::BL_DST_ALPHA;
		case ONE_MINUS_DST_ALPHA: return Device::BL_ONE_MINUS_DST_ALPHA;
		case DST_COLOR: return Device::BL_DST_COLOR;
		case ONE_MINUS_DST_COLOR: return Device::BL_ONE_MINUS_DST_COLOR;
		}
		return Device::BL_ONE;
	}
	//-------------------------------------------------------------------------------------------------------
	Device::Function MaterialCompiler::_MaterialConstant2Function( const Material_Constant& mc ) const
	{
		switch( mc )
		{
		case DEPTH_FUNCTION_NEVER: return Device::FN_NEVER;
		case DEPTH_FUNCTION_LESS: return Device::FN_LESS;
		case DEPTH_FUNCTION_GREATER: return Device::FN_GEQUAL;
		case DEPTH_FUNCTION_ALWAYS: return Device::FN_ALWAYS;
		}
		return Device::FN_LESS;
	}
	//-------------------------------------------------------------------------------------------------------
	Device::CullFaceMode MaterialCompiler::_MaterialConstant2CullFaceMode( const Material_Constant& mc ) const
	{
		switch( mc )
		{
		case CULL_FACE_NONE: return Device::CFM_NONE;
		case CULL_FACE_BACK: return Device::CFM_BACK;
		case CULL_FACE_FRONT: return Device::CFM_FRONT;
		}
		return Device::CFM_NONE;
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::_MaterialAlphaDrawState( const Pipeline::PassType& pass,  Material& mat ) const
	{
		{//处理alpha
			LuaTable alpha = GetGlobal<LuaTable>( "ALPAH_MODE" );
			Device::AlphaState& alphastate = mat.GetAlpahState( pass );
			Material_Constant mode = alpha.get<Material_Constant>(1);
			switch( mode )
			{
			case ALPAH_OFF: alphastate.m_isAlphaEnable = false; alphastate.m_isAlphaTest = false; break;
			case ALPAH_COVERAGE: alphastate.m_isAlphaEnable = false; alphastate.m_isAlphaTest = true; break;
			case ALPAH_BLEND:
				{
					alphastate.m_isAlphaEnable = true;
					alphastate.m_isAlphaTest = false;
					mode = alpha.get<Material_Constant>(2);
					alphastate.m_Src =  _MaterialConstant2Blend( mode );
					mode = alpha.get<Material_Constant>(3);
					alphastate.m_SrcA =  _MaterialConstant2Blend( mode );
					mode = alpha.get<Material_Constant>(4);
					alphastate.m_Des =  _MaterialConstant2Blend( mode );
					mode = alpha.get<Material_Constant>(5);
					alphastate.m_DesA =  _MaterialConstant2Blend( mode );
				}break;
			}
		}

		{//处理draw
			LuaTable darw = GetGlobal<LuaTable>( "DRAW_MODE" );
			Device::DrawState& darwstate = mat.GetDrawState( pass );
			Material_Constant mode = darw.get<Material_Constant>(1);
			darwstate.m_isDepthTest = DEPTH_TEST_OFF == mode ? false : true;
			mode = darw.get<Material_Constant>(2);
			darwstate.m_isDepthMask = DEPTH_MASK_OFF == mode ? false : true;
			mode = darw.get<Material_Constant>(3);
			darwstate.m_DepthFunction = _MaterialConstant2Function( mode );
			mode = darw.get<Material_Constant>(4);
			darwstate.m_isCullFace = _MaterialConstant2CullFaceMode( mode );
		}

	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::_ReadTextureState( Material& mat, const std::list<ShaderParameter::InputsType>& list )
	{
		FOR_EACH_CONST( std::list<ShaderParameter::InputsType>, it, list )
		{
			switch ( *it )
			{
			case ShaderParameter::IT_TEXTURE_AMBIENT:		_ReadTextureState( mat, Material::ATT_TEX_AMBIENT, "TEXTURE_AMBIENT");break;
			case ShaderParameter::IT_TEXTURE_DIFFUSE:		_ReadTextureState( mat, Material::ATT_TEX_DIFFUSE, "TEXTURE_DIFFUSE");break;
			case ShaderParameter::IT_TEXTURE_SPECULAR:		_ReadTextureState( mat, Material::ATT_TEX_SPECULAR, "TEXTURE_SPECULAR");break;
			case ShaderParameter::IT_TEXTURE_SPLEVEL:		_ReadTextureState( mat, Material::ATT_TEX_SPLEVEL, "TEXTURE_SPLEVEL");break;
			case ShaderParameter::IT_TEXTURE_GLOSSINESS:	_ReadTextureState( mat, Material::ATT_TEX_GLOSSINESS, "TEXTURE_GLOSSINESS");break;
			case ShaderParameter::IT_TEXTURE_ILLUMINATION:	_ReadTextureState( mat, Material::ATT_TEX_ILLUMINATION, "TEXTURE_ILLUMINATION");break;
			case ShaderParameter::IT_TEXTURE_OPACITY:		_ReadTextureState( mat, Material::ATT_TEX_OPACITY, "TEXTURE_OPACITY");break;
			case ShaderParameter::IT_TEXTURE_FITER:			_ReadTextureState( mat, Material::ATT_TEX_FITER, "TEXTURE_FITER");break;
			case ShaderParameter::IT_TEXTURE_BUMP:			_ReadTextureState( mat, Material::ATT_TEX_BUMP, "TEXTURE_BUMP");break;
			case ShaderParameter::IT_TEXTURE_REFLECTION:	_ReadTextureState( mat, Material::ATT_TEX_REFLECTION, "TEXTURE_REFLECTION");break;
			case ShaderParameter::IT_TEXTURE_REFRACTION:	_ReadTextureState( mat, Material::ATT_TEX_REFRACTION, "TEXTURE_REFRACTION");break;
			case ShaderParameter::IT_TEXTURE_DISPLACEMENT:	_ReadTextureState( mat, Material::ATT_TEX_DISPLACEMENT, "TEXTURE_DISPLACEMENT");break;
			case ShaderParameter::IT_TEXTURE_TERRAIN_WEIGHT:_ReadTextureState( mat, Material::ATT_TEX_TERRAIN_WEIGHT, "TEXTURE_TERRAIN_WEIGHT");break;
			case ShaderParameter::IT_TEXTURE_TERRAIN_DETAL:	_ReadTextureState( mat, Material::ATT_TEX_TERRAIN_DETAL, "TEXTURE_TERRAIN_DETAL");break;
			case ShaderParameter::IT_TEXTURE_TERRAIN_BASE:	_ReadTextureState( mat, Material::ATT_TEX_TERRAIN_BASE, "TEXTURE_TERRAIN_BASE");break;
			case ShaderParameter::IT_TEXTURE_TERRAIN_R:		_ReadTextureState( mat, Material::ATT_TEX_TERRAIN_R, "TEXTURE_TERRAIN_R");break;
			case ShaderParameter::IT_TEXTURE_TERRAIN_G:		_ReadTextureState( mat, Material::ATT_TEX_TERRAIN_G, "TEXTURE_TERRAIN_G");break;
			case ShaderParameter::IT_TEXTURE_TERRAIN_B:		_ReadTextureState( mat, Material::ATT_TEX_TERRAIN_B, "TEXTURE_TERRAIN_B");break;
			case ShaderParameter::IT_TEXTURE_USER0:			_ReadTextureState( mat, Material::ATT_TEX_USER0, "TEXTURE_USER0");break;
			case ShaderParameter::IT_TEXTURE_USER1:			_ReadTextureState( mat, Material::ATT_TEX_USER1, "TEXTURE_USER1");break;
			case ShaderParameter::IT_TEXTURE_USER2:			_ReadTextureState( mat, Material::ATT_TEX_USER2, "TEXTURE_USER2");break;
			case ShaderParameter::IT_TEXTURE_USER3:			_ReadTextureState( mat, Material::ATT_TEX_USER3, "TEXTURE_USER3");break;
			case ShaderParameter::IT_TEXTURE_USER4:			_ReadTextureState( mat, Material::ATT_TEX_USER4, "TEXTURE_USER4");break;
			case ShaderParameter::IT_TEXTURE_USER5:			_ReadTextureState( mat, Material::ATT_TEX_USER5, "TEXTURE_USER5");break;
			case ShaderParameter::IT_TEXTURE_USER6:			_ReadTextureState( mat, Material::ATT_TEX_USER6, "TEXTURE_USER6");break;
			case ShaderParameter::IT_TEXTURE_USER7:			_ReadTextureState( mat, Material::ATT_TEX_USER7, "TEXTURE_USER7");break;
			default: ASSERT( false && "unkown texture type" );
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::_ReadTextureState(Material& mat, int type, const char* name )
	{
		LuaTable param = GetGlobal<LuaTable>( name );
		mat.SetDefaultTextureState( static_cast<Material::Material_Attribute>(type),
			TextureState( param.get<Device::TextureWarp>(1),
				param.get<Device::TextureWarp>(2),
				param.get<Device::TextureFilter>(3),
				param.get<Device::TextureFilter>(4) )
			);
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialCompiler::_MaterialSahder( const Pipeline::PassType& pass, Material& mat )
	{
		Segments VertexSegment;
		Segments FargmentSegment;
		std::list<ShaderParameter::InputsType> Textures;
		AssembledMaterialShader* shadercode = NEW AssembledMaterialShader;//在shaderManager中被析构，我不太喜欢

		{//处理vs
			bool isProviderCoordinate = false;
			LuaTable vs = GetGlobal<LuaTable>( "VERTEX_SHADER" );
			for ( int i = 1; i <= vs.size(); i ++ )
			{
				ShaderSegment* vsseg = m_pShaderLibrary->GetCodeSegment( vs.get<char*>(i) );
				VertexSegment.push_back( NEW ShaderSegment( *vsseg ) );
			}
		}

		{//处理ps
			LuaTable ps = GetGlobal<LuaTable>( "FRAGMENT_SHADER" );
			for ( int i = 1; i <= ps.size(); i ++ )
			{
				ShaderSegment* psseg =  m_pShaderLibrary->GetCodeSegment( ps.get<char*>(i) );
				FargmentSegment.push_back( NEW ShaderSegment( *psseg ) );
			}
		}

		{//编译,设置
			m_pShaderAssembler->CompileShader( VertexSegment, FargmentSegment, *shadercode, Textures );
			shadercode->Initialization();
			int index = Engine::Instance().GetShaderManage()->InsertAssembledShader( shadercode );
			mat.SetShader( pass, index );
		}
		{//纹理属性
			_ReadTextureState( mat, Textures );
		}
		FOR_EACH( Segments, it, VertexSegment )
		{
			SAFE_DELETE(*it);
		}
		FOR_EACH( Segments, it, FargmentSegment )
		{
			SAFE_DELETE(*it);
		}
	}
}