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
#include "IOSystemBase.h"
#include "Predefine.h"
#include "Log.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	IOSystemBase::IOSystemBase()
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	IOSystemBase::~IOSystemBase()
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void IOSystemBase::SetRootPath(const string& path)
	{
		DEBUGLOG("set root path %s\n", path.c_str());
		m_RootPath = path;
		m_TextureFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Texture") + PLANTFORMDELIMITER; //do not change / becouse app need!!!
		m_ShaderFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Shaders") + PLANTFORMDELIMITER;
		m_SceneFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Scene") + PLANTFORMDELIMITER;
		m_ModelFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Model") + PLANTFORMDELIMITER;
		m_SoundFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Sound") + PLANTFORMDELIMITER;
		m_FontFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Font") + PLANTFORMDELIMITER;
		m_UIFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("UI") + PLANTFORMDELIMITER;
		m_ScriptFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Script") + PLANTFORMDELIMITER;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOSystemBase::SetWritePath(const std::string& path)
	{
		ASSERT( 0 == m_WriteFolder.size() );
		m_WriteFolder = path;
		MakeFolder(m_WriteFolder);
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOSystemBase::SetAddtionPath( const std::string& path )
	{
		ASSERT( 0 == m_AddtionPath.size() );
		m_AddtionPath = path;
		MakeFolder(m_AddtionPath);
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	IFile* IOSystemBase::FileDestroy( IFile* ptr )
	{
		SAFE_DELETE( ptr );
		return NULL;
	}

}