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
#include "Framework/IThreadMutex.h"
#include "LuaFunctionInvoker.h"
#include "LuaDobufferHelp.h"
#include "Framework/Log.h"



extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lobject.h"
}

using namespace BohgeEngine;
namespace LuaScriptSystemPlugin
{	
	//-------------------------------------------------------------------------------------------------------
	LuaFunctionInvoker::LuaFunctionInvoker( int cr, BohgeEngine::IThreadMutex* mutex, lua_State** state )
		:m_rppLuaState( state ),
		m_Invoker(NULL),
		m_rpMutex(mutex),
		m_ClassReferences(cr),
		m_FuncitonReferences(0)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	LuaFunctionInvoker::~LuaFunctionInvoker(void)
	{
		if ( *m_rppLuaState )//保证系统没有退出
		{
			luaL_unref( *m_rppLuaState , LUA_REGISTRYINDEX, m_FuncitonReferences );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Initialize( const char* fn, uint paraCount )
	{
		m_ParamArray.resize( paraCount );
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_ClassReferences );
		lua_getfield( *m_rppLuaState, -1, fn );
		ASSERT( lua_isfunction( *m_rppLuaState, -1) );
		m_FuncitonReferences = luaL_ref( *m_rppLuaState, LUA_REGISTRYINDEX );
		lua_pop( *m_rppLuaState, 1 );
		//if ( hasReturn )
		//{
		//	m_Invoker = &LuaFunctionInvoker::_InvokeWithResults;
		//}
		//else
		{
			m_Invoker = &LuaFunctionInvoker::_InvokeNoResults;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Initialize( int ref, uint paraCount )
	{
		m_ParamArray.resize( paraCount );
		m_FuncitonReferences = ref;
		//if ( hasReturn )
		//{
		//	m_Invoker = &LuaFunctionInvoker::_InvokeWithResults;
		//}
		//else
		{
			m_Invoker = &LuaFunctionInvoker::_InvokeNoResults;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_InvokeWithResults()
	{
		ASSERT(false);//需要重写输出
		m_rpMutex->Lock();
#ifdef _DEBUG
		lua_pushcclosure( *m_rppLuaState, on_error, 0 );
		int errfunc = lua_gettop( *m_rppLuaState );
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_FuncitonReferences );
		ASSERT( lua_isfunction( *m_rppLuaState, -1) );
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_ClassReferences );
		for ( int i = 0; i < m_ParamArray.size(); ++ i )
		{
			ScriptValue* sv = static_cast<ScriptValue*>( lua_newuserdata( *m_rppLuaState, sizeof(ScriptValue) ) );
			sv->SetValue( m_ParamArray[i], &ScriptValue::NullCleaner );
			lua_getglobal( *m_rppLuaState, sv->GetObject()->GetTypeName() );
			lua_setmetatable( *m_rppLuaState, -2 );
		}
		ASSERT( lua_pcall(*m_rppLuaState, m_ParamArray.size() + 1, 1, errfunc) == 0 );
		lua_pop(*m_rppLuaState, 2);
#else
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_FuncitonReferences );
		ASSERT( lua_isfunction( *m_rppLuaState, -1) );
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_ClassReferences );
		for ( int i = 0; i < m_ParamArray.size(); ++ i )
		{
			ScriptValue* sv = static_cast<ScriptValue*>( lua_newuserdata( *m_rppLuaState, sizeof(ScriptValue) ) );
			sv->SetValue( m_ParamArray[i], &ScriptValue::NullCleaner );
			lua_getglobal( *m_rppLuaState, sv->GetObject()->GetTypeName() );
			lua_setmetatable( *m_rppLuaState, -2 );
		}
		lua_call( *m_rppLuaState, m_ParamArray.size() + 1, 1 );
		int t = lua_gettop( *m_rppLuaState );
		lua_pop(*m_rppLuaState, 2);
#endif
		ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
		m_rpMutex->Unlock();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_InvokeNoResults()
	{
		m_rpMutex->Lock();
#ifdef _DEBUG
		lua_pushcclosure( *m_rppLuaState, on_error, 0 );
		int errfunc = lua_gettop( *m_rppLuaState );
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_FuncitonReferences );
		ASSERT( lua_isfunction( *m_rppLuaState, -1) );
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_ClassReferences );
		for ( int i = 0; i < m_ParamArray.size(); ++ i )
		{
			ScriptValue* sv = static_cast<ScriptValue*>( lua_newuserdata( *m_rppLuaState, sizeof(ScriptValue) ) );
			sv->SetValue( m_ParamArray[i], &ScriptValue::NullCleaner );
			lua_getglobal( *m_rppLuaState, sv->GetObject()->GetTypeName() );
			lua_setmetatable( *m_rppLuaState, -2 );
		}
		ASSERT( lua_pcall(*m_rppLuaState, m_ParamArray.size() + 1, 0, errfunc) == 0 );
		lua_pop( *m_rppLuaState, 1 );
#else
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_FuncitonReferences );
		ASSERT( lua_isfunction( *m_rppLuaState, -1) );
		lua_rawgeti( *m_rppLuaState, LUA_REGISTRYINDEX, m_ClassReferences );
		for ( int i = 0; i < m_ParamArray.size(); ++ i )
		{
			ScriptValue* sv = static_cast<ScriptValue*>( lua_newuserdata( *m_rppLuaState, sizeof(ScriptValue) ) );
			sv->SetValue( m_ParamArray[i], &ScriptValue::NullCleaner );
			lua_getglobal( *m_rppLuaState, sv->GetObject()->GetTypeName() );
			lua_setmetatable( *m_rppLuaState, -2 );
		}
		lua_call( *m_rppLuaState, m_ParamArray.size() + 1, 0 );
#endif
		ASSERT( 0 == lua_gettop( *m_rppLuaState ) );
		m_rpMutex->Unlock();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Invoke()
	{
		ASSERT( 0 == m_ParamArray.size() );
		return (this->*m_Invoker)();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Invoke( const Object* p1 )
	{
		ASSERT( 1 == m_ParamArray.size() );
		m_ParamArray[0] = p1;
		return (this->*m_Invoker)();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Invoke( const Object* p1, const Object* p2 )
	{
		ASSERT( 2 == m_ParamArray.size() );
		m_ParamArray[0] = p1;
		m_ParamArray[1] = p2;
		return (this->*m_Invoker)();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Invoke( const Object* p1, const Object* p2, const Object* p3 )
	{
		ASSERT( 3 == m_ParamArray.size() );
		m_ParamArray[0] = p1;
		m_ParamArray[1] = p2;
		m_ParamArray[2] = p3;
		return (this->*m_Invoker)();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Invoke( const Object* p1, const Object* p2, const Object* p3, const Object* p4 )
	{
		ASSERT( 4 == m_ParamArray.size() );
		m_ParamArray[0] = p1;
		m_ParamArray[1] = p2;
		m_ParamArray[2] = p3;
		m_ParamArray[3] = p4;
		return (this->*m_Invoker)();
	}
}