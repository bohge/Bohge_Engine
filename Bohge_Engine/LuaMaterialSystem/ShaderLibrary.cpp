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



/////////////////////////////////////
//		新的shader段的管理库		   //
/////////////////////////////////////
#include "Framework/ShaderParameter.h"
#include "Framework/Utility.hpp"
#include "Framework/IIOSystem.h"
#include "Framework/IFile.h"
#include "Framework/Log.h"
#include "ShaderSegment.h"
#include "ShaderLibrary.h"
#include "ShaderFile.h"


#define  MAKE_STRING_ENUM(x,y) #y , BohgeEngine::ShaderParameter::x##y

using namespace eastl;
using namespace BohgeEngine;

namespace LuaMaterialSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	ShaderLibrary::ShaderLibrary(void)
		:m_pScriptParser( NEW ScriptParser )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderLibrary::~ShaderLibrary(void)
	{
		SAFE_DELETE( m_pScriptParser );
		ReleaseResource();
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderLibrary::Initialize()
	{
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( FP_, NONE) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( FP_, LOW) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( FP_, MEDIUM) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( FP_, HIGH) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, FLOAT ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, VEC2 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, VEC3 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, VEC4 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, VEC2_ARRAY ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, VEC3_ARRAY ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, VEC4_ARRAY ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, MAT3 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, MAT4 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, TEXTURE1D ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, TEXTURE2D ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, TEXTURE3D ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( VT_, TEXTURECUBE ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_POSITION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_COORDNATE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_NORMAL ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_COLOR ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_TANGENT ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_BINORMAL ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_BONE_INEX ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_BONE_WEIGHT ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_POSITION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_NORMAL ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_TANGENT ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_BINORMAL ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_VIEWDIRECTION ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, SCREENSPACE_POSITION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, SCREENSPACE_COORDNATE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, SCREENSPACE_NORMAL ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, SCREENSPACE_TANGENT ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, SCREENSPACE_BINORMAL ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_LIGHT_DISTANCE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, WORLDSPACE_DEPTH ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_OFFSET ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_LIFE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_BORN ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_AGE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_SPEED ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_ACCELERATION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_SIZE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_SIZE_CURVATURE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_COLOR ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_COLOR_CURVATURE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, EMITTER_SPIN ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_0 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_1 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_2 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_3 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_4 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_5 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_6 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, USER_ATTRIBUTE_7 ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, SYSTEM_TIME ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, SYSTEM_TIME_SPAN ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_REAL_ARRAY ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSPACE_DUAL_ARRAY ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALWORLD_TRANSFORM ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALWORLD_ROTATION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LOCALSCREEN_TRANSVIEWPROJ ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_WORLDROTATION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_WORLDPOSITION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_WORLDDIRECTION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_VIEW ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_PROJECTION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_VIEWPROJ ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, CAMERA_LINERPARAM ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_POSITION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_COLOR ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, AMBIENT_COLOR ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_ANGLE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_INNER_DIFF_INV ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_RANGE_INV ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_ATTENUATION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_VERTEX_DIRECTION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_GIVEN_DIRECTION ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_AMBIENT ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_DIFFUSE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_SPECULAR ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_SPECULAR_LEVEL ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_SHININESS ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_MATERIAL_AMBIENT ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_MATERIAL_LIGHT ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHT_MATERIAL_SPECULAR ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, SURFACE_COLOR ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHTING_DIFFUSE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, LIGHTING_SPECULAR ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_AMBIENT ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_DIFFUSE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_SPECULAR ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_SPLEVEL ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_GLOSSINESS ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_ILLUMINATION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_OPACITY ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_FITER ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_BUMP ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_REFLECTION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_REFRACTION ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_DISPLACEMENT ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_WEIGHT ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_DETAL ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_BASE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_R ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_G ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_TERRAIN_B ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER0 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER1 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER2 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER3 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER4 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER5 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER6 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, TEXTURE_USER7 ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, COLOR_RED ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, COLOR_GREEN ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, COLOR_BLUE ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, COLOR_ALPHA ) );

		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_USER_0 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_USER_1 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_USER_2 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_USER_3 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_USER_4 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_USER_5 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_USER_6 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_USER_7 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_USER_8 ) );
		m_pScriptParser->SetGlobal( MAKE_STRING_ENUM( IT_, MATERIAL_USER_9 ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderLibrary::ReleaseResource()
	{
		FOR_EACH( ShaderFileMap, it, m_IncludedShaderFiles )
		{
			SAFE_DELETE( it->second );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderLibrary::HandleShaderSegment( const eastl::string& filename )
	{
		ShaderFile* shaders = NULL;
		ShaderFileMap::iterator it = m_IncludedShaderFiles.find( Utility::HashCode(filename) );
		if ( m_IncludedShaderFiles.end() != it  )
		{
			shaders = it->second;
		}
		else
		{
			shaders = NEW ShaderFile();
			shaders->LoadShaderFile( filename );
			m_IncludedShaderFiles.insert( make_pair( Utility::HashCode( filename ), shaders ) );
		}
		m_CodeSegmentMap.insert( shaders->GetShaderMap().begin(), shaders->GetShaderMap().end() );
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderSegment* ShaderLibrary::GetCodeSegment( const eastl::string& name )
	{
		ShaderSegmentMap::iterator it = m_CodeSegmentMap.find( Utility::HashCode( name ) );
		if ( m_CodeSegmentMap.end() != it )
		{
			if ( it->second->isOriginal() )
			{
				it->second->ProcessSegment( *this );
			}
			return it->second;
		}
		DEBUGLOG("Unkonw code segment \"%s\"! Did you forget include the code head?\n", name.c_str() );
		ASSERT(false);
		return NULL;
	}
}