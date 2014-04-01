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

#include "ShaderLibrary.h"
#include "IFile.h"
#include "IOSystem.h"
#include "Utility.h"
#include "ShaderSegment.h"
#include "ShaderParameter.h"
#include "ShaderFile.h"

using namespace std;




namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	ShaderLibrary::ShaderLibrary(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderLibrary::~ShaderLibrary(void)
	{
		ReleaseResource();
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderLibrary::Initialization()
	{
		_NewLuaState();
		_SetGlobalClassesIndex();
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderLibrary::LoadScript( const std::string& filename )
	{
		ASSERT( false );//废弃
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderLibrary::_OnScriptLoaded()
	{
		ASSERT( false );//废弃
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderLibrary::DoScript( const std::string& buffer )
	{
		lua_tinker::dostring( LuaState(), buffer.c_str() );
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderLibrary::UnLoadScript()
	{
		_CloseLuaState();
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderLibrary::_SetGlobalClassesIndex()
	{
		ShaderParameter::RegisterShaderParameter( *this );
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
	void ShaderLibrary::HandleShaderSegment( const std::string& filename )
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
	ShaderSegment* ShaderLibrary::GetCodeSegment( const std::string& name )
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
		return NULL;
	}
}