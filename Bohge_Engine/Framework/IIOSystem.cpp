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
#include "IIOSystem.h"
#include "Framework.h"
#include "IFile.h"
#include "Log.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace eastl;

namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( IFile );
	RTTI_TYPE_IMPLEMENT( IIOSystem );
	SYSTEM_SINGLETON_IMPLEMENT( IIOSystem );
	//-------------------------------------------------------------------------------------------------------
	IIOSystem::IIOSystem( void )
		:m_pAssetRoot(NULL),
		m_pDocPath(NULL),
		m_pLitePath(NULL),
		m_pDefaultAssetPath(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//-------------------------------------------------------------------------------------------------------
	IIOSystem::~IIOSystem( void )
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//-------------------------------------------------------------------------------------------------------
	void IIOSystem::SetRootPath(const string& path)
	{
		DEBUGLOG("set root path %s\n", path.c_str());
		*m_pAssetRoot = path;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IIOSystem::SetResourcePath(const eastl::string& path)
	{
		*m_pDocPath = path;
		*m_pDefaultAssetPath = path + "DefaultAsset/";
		MakeFolder(*m_pDocPath);
	}
	//-------------------------------------------------------------------------------------------------------
	void IIOSystem::SetLiteappPath( const eastl::string& path )
	{
		if ( path.size() > IO_IDENTIFIERS_LENGTH )
		{
			*m_pLitePath = PathAssembly( path );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void IIOSystem::MakeFolder( const eastl::string& path )
	{
		_DoMakeFolder( PathAssembly(path) );
	}
	//-------------------------------------------------------------------------------------------------------
	bool IIOSystem::isFileExist( const eastl::string& path )
	{
		return _DoIsFileExist( PathAssembly(path) );
	}
	//-------------------------------------------------------------------------------------------------------
	bool IIOSystem::DeleteFile( const eastl::string& path )
	{
		return _DoDeleteFile( PathAssembly(path) );
	}
	//-------------------------------------------------------------------------------------------------------
	BohgeEngine::IFile* IIOSystem::FileFactory( const eastl::string& path )
	{
		return _DoFileFactory( PathAssembly(path) );
	}
	//-------------------------------------------------------------------------------------------------------
	void IIOSystem::OnFrameworkEntry( float time, Framework& framework )
	{
		m_pAssetRoot = NEW string;
		m_pDocPath = NEW string;
		m_pLitePath = NEW string;
		m_pDefaultAssetPath = NEW string;
		m_pAssetRoot->reserve( IO_MAX_LITE_PATH_SIZE );
		m_pDocPath->reserve( IO_MAX_LITE_PATH_SIZE );
		m_pLitePath->reserve( IO_MAX_LITE_PATH_SIZE );
		m_pDefaultAssetPath->reserve( IO_MAX_LITE_PATH_SIZE );
		SetRootPath( framework.GetConfig().GetRootPath() );
		SetResourcePath( framework.GetConfig().GetResourcePath() );
		SetLiteappPath( framework.GetConfig().GetLitePath() );
	}
	//-------------------------------------------------------------------------------------------------------
	void IIOSystem::OnFrameworkExit( float time, Framework& framework )
	{
		SAFE_DELETE( m_pAssetRoot );
		SAFE_DELETE( m_pDocPath );
		SAFE_DELETE( m_pLitePath );
		SAFE_DELETE( m_pDefaultAssetPath );
	}
	//-------------------------------------------------------------------------------------------------------
	void IIOSystem::RegistEvent( Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_ENTRY, MEMBER_FUNC_PTR( &IIOSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_EXIT, MEMBER_FUNC_PTR( &IIOSystem::OnFrameworkExit ) );
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string IIOSystem::PathAssembly( const eastl::string& path )
	{
		//uint id = 0;
		//for ( int i = 0 ; i < IO_IDENTIFIERS_LENGTH ; ++ i )
		//{
		//	id |= static_cast<int>(path[i]) << ( (IO_IDENTIFIERS_LENGTH - i - 1) * 8 );
		//}
		uint id = static_cast<int>(path[0]) << 24
			| static_cast<int>(path[1]) << 16
			| static_cast<int>(path[2]) << 8
			| static_cast<int>(path[3]);
		switch( id )
		{
		case 'root':
			{
				return *m_pAssetRoot + path.substr( IO_SUBPATH_POS, path.length() );
			}break;
		case 'docs':
			{
				return *m_pDocPath + path.substr( IO_SUBPATH_POS, path.length() );
			}break;
		case 'lite':
			{
				return *m_pLitePath + path.substr( IO_SUBPATH_POS, path.length() );
			}
		case 'defs':
			{
				return *m_pDefaultAssetPath + path.substr( IO_SUBPATH_POS, path.length() );
			}
		default:
			{
				DEBUGLOG( "Raw path %s\n", path.c_str() );
				return path;
			}break;
		}
	}
}