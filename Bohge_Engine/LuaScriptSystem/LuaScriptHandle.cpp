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
#include "LuaFunctionInvoker.h"
#include "LuaScriptHandle.h"
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
	LuaScriptHandle::LuaScriptHandle( lua_State** lua, BohgeEngine::IThreadMutex* mutex )
		:m_rppLuaState(lua),
		m_rpMutex( mutex )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	LuaScriptHandle::~LuaScriptHandle(void)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptHandle::_DoRelease( int h )
	{
		if ( *m_rppLuaState )
		{
			luaL_unref( *m_rppLuaState , LUA_REGISTRYINDEX, h );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool LuaScriptHandle::_isFunctionExist( int h, const eastl::string& name )
	{
		ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, h );
		ASSERT( lua_istable(*m_rppLuaState, -1 ) );
		lua_getfield( *m_rppLuaState, -1, name.c_str() );
		bool res = lua_isfunction( *m_rppLuaState, -1 );
		lua_pop(*m_rppLuaState,2);
		ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	IFunctionInvoker* LuaScriptHandle::_DoSignFunciton( int h, const eastl::string& name, uint argCount )
	{
		LuaFunctionInvoker* func = NEW LuaFunctionInvoker( h, m_rpMutex, m_rppLuaState );
		func->Initialize( name.c_str(), argCount );
		return func;
	}
	//-------------------------------------------------------------------------------------------------------
	BohgeEngine::IFunctionInvoker* LuaScriptHandle::_DoSignFunciton( int h, int userdata, uint argCount )
	{
		LuaFunctionInvoker* func = NEW LuaFunctionInvoker( h, m_rpMutex, m_rppLuaState );
		func->Initialize( userdata, argCount );
		return func;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptHandle::_DoReleaseFunction( int h, BohgeEngine::IFunctionInvoker* func )
	{
		LuaFunctionInvoker* lfunc = static_cast< LuaFunctionInvoker* >(func);
		SAFE_DELETE( lfunc );
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptHandle::_DoSetMember( int h, const char* name, const Object* obj )
	{
		ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, h );
		ASSERT( lua_istable(*m_rppLuaState, -1 ) );
		lua_pushstring(*m_rppLuaState, name );//将名称推入堆栈
		ScriptValue* userdata = (ScriptValue*)lua_newuserdata(*m_rppLuaState, sizeof(ScriptValue) );
		userdata->SetValue( obj, &ScriptValue::NullCleaner );	
		lua_getglobal( *m_rppLuaState, obj->GetTypeName() );
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_setmetatable( *m_rppLuaState, -2 );
		lua_rawset(*m_rppLuaState, -3);
		lua_pop(*m_rppLuaState, 1);
		ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
	}
	//-------------------------------------------------------------------------------------------------------
	const const void* LuaScriptHandle::_DoGetMember( int h, const char* name )
	{
		ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, h );
		ASSERT( lua_istable(*m_rppLuaState, -1 ) );
		lua_getfield( *m_rppLuaState, -1, name );
		const void* res = NULL;
		switch( lua_type( *m_rppLuaState, -1 ) )
		{
		case LUA_TNUMBER: res = lua_tonumberpf( *m_rppLuaState, -1 ); break;
		case LUA_TBOOLEAN: res = lua_tobooleanpb( *m_rppLuaState, -1 ); break;
		case LUA_TSTRING: res = lua_tostring( *m_rppLuaState, -1 ); break;
		case LUA_TUSERDATA: res = lua_touserdata( *m_rppLuaState, -1 ); break;
		default:ASSERT(false);
		}
		lua_pop( *m_rppLuaState, 2 );
		ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
		return res;
	}
}