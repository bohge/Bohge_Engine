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
#include "Framework/ScriptValue.h"

extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace LuaScriptSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	static void* GetScriptResultValueInstance( void* L, void* index )
	{
		return NEW int;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetScriptResultValueNumber( void* L, void* index )
	{
		return NEW float;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetScriptResultValueBool( void* L, void* index )
	{
		return NEW bool;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetScriptResultValueString( void* L, void* index )
	{
		return NEW eastl::string;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetScriptResultValueClass( void* L, void* index )
	{
		return lua_newuserdata( (lua_State*)L, sizeof(BohgeEngine::ScriptValue) );
	}
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	static void* GetScriptResultPushInstance( void* L, void* volue )
	{
		lua_rawgeti( (lua_State*)L, LUA_REGISTRYINDEX, *(int*)volue );
		SAFE_DELETE( volue );
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetScriptResultPushNumber( void* L, void* volue )
	{
		lua_pushnumber( (lua_State*)L, *(float*)volue );
		SAFE_DELETE( volue );
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetScriptResultPushBool( void* L, void* volue )
	{
		lua_pushboolean( (lua_State*)L, *(bool*)volue );
		SAFE_DELETE( volue );
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetScriptResultPushString( void* L, void* volue )
	{
		lua_pushstring( (lua_State*)L, ((eastl::string*)volue)->c_str() );
		SAFE_DELETE( volue );
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetScriptResultPushClass( void* L, void* volue )
	{
		if ( NULL == ((BohgeEngine::ScriptValue*)volue)->GetObject() )//·µ»Ø¿ÕÖµ
		{
			lua_remove( (lua_State*)L, -1 );
			lua_pushnil( (lua_State*)L );
		}
		else
		{
			lua_getglobal( (lua_State*)L, ((BohgeEngine::ScriptValue*)volue)->GetObject()->GetTypeName() );
			lua_setmetatable( (lua_State*)L, -2 );
		}
		return NULL;
	}
}