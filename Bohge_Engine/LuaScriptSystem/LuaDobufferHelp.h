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
#include "Framework/Log.h"



extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lobject.h"
}

static void print_error(lua_State *L, const char* fmt, ...)
{
	char text[4096];

	va_list args;
	va_start(args, fmt);
	vsprintf(text, fmt, args);
	va_end(args);

	lua_getglobal(L, "_ALERT");
	if(lua_isfunction(L, -1))
	{
		lua_pushstring(L, text);
		lua_call(L, 1, 0);
	}
	else
	{
		DEBUGLOG("%s", text);
		lua_pop(L, 1);
	}
	DEBUGLOG("\n");
}

static void call_stack(lua_State* L, int n)
{
	lua_Debug ar;
	if(lua_getstack(L, n, &ar) == 1)
	{
		lua_getinfo(L, "nSlu", &ar);

		const char* indent;
		if(n == 0)
		{
			indent = "->\t";
			print_error(L, "\t<call stack>");
		}
		else
		{
			indent = "\t";
		}

		if(ar.name)
			print_error(L, "%s%s() : line %d [%s : line %d]", indent, ar.name, ar.currentline, ar.source, ar.linedefined);
		else
			print_error(L, "%sunknown : line %d [%s : line %d]", indent, ar.currentline, ar.source, ar.linedefined);

		call_stack(L, n+1);
	}
}

static int on_error(lua_State *L)
{
	print_error(L, "%s", lua_tostring(L, -1));

	call_stack(L, 0);

	return 0;	
}
static void DoBuffer( lua_State *L, const char *name,  const char* buff, size_t len )
{
	lua_pushcclosure(L, on_error, 0);
	int errfunc = lua_gettop(L);
	luaL_getsubtable( L, LUA_REGISTRYINDEX, "_LOADED" );
	lua_getfield(L, -1, name);  /* _LOADED[name] */
	ASSERT( !lua_toboolean(L, -1) );//必须是没有加载的
	if(luaL_loadbuffer( L, buff, len, name ) == 0)
	{
		lua_pushvalue( L, -1 );
		lua_setfield( L, -4, name );
		if(lua_pcall(L, 0, 0, errfunc) != 0)
		{
			lua_pop(L, 1);
		}
	}
	else
	{
		print_error(L, "%s", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	lua_pop(L, 3);
	ASSERT( 0 == lua_gettop( L ) );
}