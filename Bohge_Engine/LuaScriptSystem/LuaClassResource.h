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
#include "Framework/IClassResource.h"


struct lua_State;
namespace BohgeEngine{class IThreadMutex;}
namespace LuaScriptSystemPlugin
{
	class LuaClassResource : public BohgeEngine::IClassResource
	{
	private:
		int							m_FactoryReferences;
		eastl::string*				m_pChunkName;
		eastl::string*				m_pClassName;
		eastl::string*				m_pFileName;
		eastl::string*				m_pScriptSource;
		lua_State**					m_rppLuaState;
		BohgeEngine::IThreadMutex*	m_rpMutex;
	public:
		LuaClassResource( lua_State** lua, BohgeEngine::IThreadMutex* mutex );
		virtual ~LuaClassResource(void);
	private:
		virtual int _DoGetInstance( );//·µ»ØÊµÀýµÄhandle
		virtual void _DoInitialize( );
		virtual void _DoRelease( );
	public:
		BOHGE_FORCEINLINE void SetSource( const eastl::string& source )
		{
			*m_pScriptSource = source;
		}
		BOHGE_FORCEINLINE void SetFileName( const eastl::string& name )
		{
			*m_pFileName = name;
		}
		BOHGE_FORCEINLINE void SetChunkName( const eastl::string& name )
		{
			*m_pChunkName = name;
		}
		BOHGE_FORCEINLINE void SetClassName( const eastl::string& name )
		{
			*m_pClassName = name;
		}
	};
}
