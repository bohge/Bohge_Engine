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
#include "Framework/Framework.h"
#include "LuaClassResource.h"
#include "LuaDobufferHelp.h"
#include "LuaScriptConst.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lobject.h"
}

#include <sstream>
using namespace eastl;
using namespace BohgeEngine;
namespace LuaScriptSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	LuaClassResource::LuaClassResource( lua_State** lua, BohgeEngine::IThreadMutex* mutex )
		:m_rppLuaState(lua),
		m_FactoryReferences(-1),
		m_pFileName( NEW string ),
		m_pScriptSource( NEW string ),
		m_pClassName(NEW string),
		m_pChunkName(NEW string),
		m_rpMutex( mutex )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	LuaClassResource::~LuaClassResource(void)
	{
		SAFE_DELETE( m_pChunkName );
		SAFE_DELETE( m_pClassName );
		SAFE_DELETE( m_pFileName );
		SAFE_DELETE( m_pScriptSource );
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaClassResource::_DoRelease()
	{
		if ( *m_rppLuaState )
		{
			luaL_unref( *m_rppLuaState , LUA_REGISTRYINDEX, m_FactoryReferences );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaClassResource::_DoInitialize()
	{
		DoBuffer( *m_rppLuaState, m_pFileName->c_str(), m_pScriptSource->c_str(), m_pScriptSource->size() );
		lua_getglobal( *m_rppLuaState, (*m_pClassName + LUA_CLASS_FACTORY_FIX).c_str() );//得到创建实例的函数
		ASSERT( lua_isfunction( *m_rppLuaState, -1) );
		m_FactoryReferences = luaL_ref( *m_rppLuaState, LUA_REGISTRYINDEX );//将该函数绑定到索引列表
		SAFE_DELETE( m_pClassName );
		SAFE_DELETE( m_pFileName );
		SAFE_DELETE( m_pScriptSource );
		SAFE_DELETE( m_pChunkName );
		ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
	}
	//-------------------------------------------------------------------------------------------------------
	int LuaClassResource::_DoGetInstance()
	{
		int res = 0;
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_FactoryReferences );
		ASSERT( lua_isfunction( *m_rppLuaState, -1 ) );
		lua_call( *m_rppLuaState, 0, 1 );
		ASSERT( lua_istable( *m_rppLuaState, -1 ) );
		res = luaL_ref( *m_rppLuaState, LUA_REGISTRYINDEX );
		ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
		return res;
		//int res = 0;
		//lua_newtable( *m_rppLuaState );
		//lua_newtable( *m_rppLuaState );
		//lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_ClassReferences );
		//ASSERT( lua_istable( *m_rppLuaState, -1) );
		//lua_setfield(*m_rppLuaState, -2, "__index");
		//lua_setmetatable(*m_rppLuaState, -2);
		//res = luaL_ref( *m_rppLuaState, LUA_REGISTRYINDEX );
		//ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
		//return res;
	}

}