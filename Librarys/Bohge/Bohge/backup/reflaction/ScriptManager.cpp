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



/////////////////////////////
//		Lua脚本管理	   //
/////////////////////////////
#include "ScriptManager.h"
#include "lua.hpp"
#include "FileSystem.h"
#include "Log.h"
#include <string>

using namespace std;


//--------------------------------------------------------------------------------------------------------------------------------------
ScriptManager::ScriptManager(void)
{
	m_Lua = luaL_newstate();
	luaL_openlibs (m_Lua);//打开全部的库
	//luaopen_base( m_Lua );//基础库
	//luaopen_coroutine( m_Lua );//线程库
	//luaopen_table( m_Lua );//表格库
	//luaopen_io( m_Lua );//io库
	//luaopen_os( m_Lua );//操作系统相关的，时间啊等等
	//luaopen_string( m_Lua );//字符串
	//luaopen_bit32( m_Lua );//32位
	//luaopen_math( m_Lua );//数学
	//luaopen_debug( m_Lua );//除错
	//luaopen_package( m_Lua );//封装库
	/*{
		int check = luaL_loadfile(m_Lua, File::Instance().MakeFilePath("Test.lua").c_str() );
		if ( check == 0 )  
		{  
			check = lua_pcall(m_Lua, 0, LUA_MULTRET, 0);  
			if (check != 0)  
			{  
				_PrintLuaError(check);  
			} 
		}  
		else  
		{  
			_PrintLuaError(check);  
		} 

		check = luaL_loadfile(m_Lua, File::Instance().MakeFilePath("Test2.lua").c_str() );
		if ( check == 0 )  
		{  
			check = lua_pcall(m_Lua, 0, LUA_MULTRET, 0);  
			if (check != 0)  
			{  
				_PrintLuaError(check);  
			} 
		}  
		else  
		{  
			_PrintLuaError(check);  
		}

		Add(10, 15);
	} */
}
//--------------------------------------------------------------------------------------------------------------------------------------
ScriptManager::~ScriptManager(void)
{
	lua_close( m_Lua );
}

int ScriptManager::Add( int x, int y )
{
	int sum;
	lua_getglobal(m_Lua, "add");
	lua_pushnumber(m_Lua, x);
	lua_pushnumber(m_Lua, y);
	lua_call(m_Lua, 2, 1);
	sum = (int)lua_tonumber(m_Lua, -1);
	lua_pop(m_Lua, 1);
	return sum;
}

//--------------------------------------------------------------------------------------------------------------------------------------
void ScriptManager::_PrintLuaError( int sErr )
{
	if (sErr==0)  
	{  
		return;  
	}  
	const char* error;  
    string sErrorType;
	switch(sErr)  
	{  
	case LUA_ERRSYNTAX://编译时错误  
		/*const char *buf = "mylib.myfun()2222";类似这行语句可以引起编译时错误*/  
		sErrorType += string("Syntax error during pre-compilation\n");  
		break;  
	case LUA_ERRMEM://内存错误  
		sErrorType += string("Memory allocation error\n");  
		break;  
	case LUA_ERRRUN://运行时错误  
		/*const char *buf = "my222lib.myfun()";类似这行语句可以引起运行时错误，my222lib实际上不存在这样的库，返回的值是nil*/  
		sErrorType += string("A runtime error\n");  
		break;  
	case LUA_YIELD://线程被挂起错误  
		sErrorType += string("Thread has Suspended\n");  
		break;  
	case LUA_ERRERR://在进行错误处理时发生错误  
		sErrorType += string("Error while running the error handler function\n");  
		break;  
	default:  
		break;  
	}  
	error = lua_tostring(m_Lua, -1);//打印错误结果  
	DEBUGLOG("%s:%s\n",sErrorType.c_str(),error);  
	lua_pop( m_Lua, 1);   
}
