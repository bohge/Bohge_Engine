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
#include "Framework/FunctionRegister.h"
#include "Framework/ScriptUtility.h"
#include "Framework/ClassRegister.h"
#include "Framework/ScriptValue.h"
#include "Framework/Object.hpp"


extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lobject.h"
}


namespace LuaScriptSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	static const BohgeEngine::Rtti* ObjectDefinedHelper( lua_State* L, int index )//找到对应的参数类型
	{
		switch( lua_type( L, index ) )
		{
		case LUA_TNUMBER: return &BohgeEngine::Number::RTTI();
		case LUA_TBOOLEAN: return &BohgeEngine::Bool::RTTI();
		case LUA_TSTRING: return &BohgeEngine::String::RTTI();
		case LUA_TFUNCTION: return &BohgeEngine::Function::RTTI();
		case LUA_TUSERDATA: return &((BohgeEngine::ScriptValue*)lua_touserdata(L,index))->GetObject()->GetRtti();
		default:ASSERT(false);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	static int ClassDestruct(lua_State *L)//析构函数
	{
		BohgeEngine::ScriptValue* data = (BohgeEngine::ScriptValue*)lua_touserdata(L,1);
		data->~ScriptValue();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int SingleClassConstructStatic(lua_State *L)//构造函数和静态函数
	{
		BohgeEngine::ClassRegister* sc = 
			(BohgeEngine::ClassRegister*)lua_touserdata(L, lua_upvalueindex(1));
		int index =	lua_tonumber(L, lua_upvalueindex(2));
		BohgeEngine::FunctionRegister*func = sc->GetFunction(index);

		uint ac = func->GetArgumentCount();
		ASSERT( func->hasReturn() );//必须有返回至
		void* resdata = func->GetScriptResultValue( L, NULL );
		switch ( ac )
		{
		case 0:func->Invoke( resdata );break;
		case 1:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 )
				   );break;
		case 2:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 ),//参数
				   func->GetScriptParameter( 1, L, (void*)3 )
				   );break;
		case 3:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 ),//参数
				   func->GetScriptParameter( 1, L, (void*)3 ),
				   func->GetScriptParameter( 2, L, (void*)4 )
				   );break;
		case 4:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 ),//参数
				   func->GetScriptParameter( 1, L, (void*)3 ),
				   func->GetScriptParameter( 2, L, (void*)4 ),
				   func->GetScriptParameter( 3, L, (void*)5 )
				   );break;
		case 5:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 ),//参数
				   func->GetScriptParameter( 1, L, (void*)3 ),
				   func->GetScriptParameter( 2, L, (void*)4 ),
				   func->GetScriptParameter( 3, L, (void*)5 ),
				   func->GetScriptParameter( 4, L, (void*)6 )
				   );break;
		case 6:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 ),//参数
				   func->GetScriptParameter( 1, L, (void*)3 ),
				   func->GetScriptParameter( 2, L, (void*)4 ),
				   func->GetScriptParameter( 3, L, (void*)5 ),
				   func->GetScriptParameter( 4, L, (void*)6 ),
				   func->GetScriptParameter( 5, L, (void*)7 )
				   );break;
		default:ASSERT(false);
		}
		func->PushScriptReslut( L, resdata );
		return 1;
	}

	//-------------------------------------------------------------------------------------------------------
	static int OverrideClassConstructStatic(lua_State *L)//重载的函数
	{
		BohgeEngine::ClassRegister* sc = 
			(BohgeEngine::ClassRegister*)lua_touserdata(L, lua_upvalueindex(1));
		int index =	lua_tonumber(L, lua_upvalueindex(2));
		uint ac = lua_gettop(L) - 1;//得到参数数量，减一的原因是不需要传入self
		BohgeEngine::FunctionRegister* func = NULL;
		ASSERT( 0 != ac );
		switch ( ac )
		{
		case 1:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 )
				   );break;
		case 2:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 ),
				   ObjectDefinedHelper( L, 3 )
				   );break;
		case 3:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 ),
				   ObjectDefinedHelper( L, 3 ),
				   ObjectDefinedHelper( L, 4 )
				   );break;
		case 4:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 ),
				   ObjectDefinedHelper( L, 3 ),
				   ObjectDefinedHelper( L, 4 ),
				   ObjectDefinedHelper( L, 5 )
				   );break;
		case 5:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 ),
				   ObjectDefinedHelper( L, 3 ),
				   ObjectDefinedHelper( L, 4 ),
				   ObjectDefinedHelper( L, 5 ),
				   ObjectDefinedHelper( L, 6 )
				   );break;
		case 6:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 ),
				   ObjectDefinedHelper( L, 3 ),
				   ObjectDefinedHelper( L, 4 ),
				   ObjectDefinedHelper( L, 5 ),
				   ObjectDefinedHelper( L, 6 ),
				   ObjectDefinedHelper( L, 7 )
				   );break;
		default:ASSERT(false);
		}

		ASSERT( func->hasReturn() );
		void* resdata = func->GetScriptResultValue( L, NULL );
		switch ( ac )
		{
		case 0:func->Invoke( resdata );break;
		case 1:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 )
				   );break;
		case 2:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 ),//参数
				   func->GetScriptParameter( 1, L, (void*)3 )
				   );break;
		case 3:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 ),//参数
				   func->GetScriptParameter( 1, L, (void*)3 ),
				   func->GetScriptParameter( 2, L, (void*)4 )
				   );break;
		case 4:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 ),//参数
				   func->GetScriptParameter( 1, L, (void*)3 ),
				   func->GetScriptParameter( 2, L, (void*)4 ),
				   func->GetScriptParameter( 3, L, (void*)5 )
				   );break;
		case 5:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 ),//参数
				   func->GetScriptParameter( 1, L, (void*)3 ),
				   func->GetScriptParameter( 2, L, (void*)4 ),
				   func->GetScriptParameter( 3, L, (void*)5 ),
				   func->GetScriptParameter( 4, L, (void*)6 )
				   );break;
		case 6:func->Invoke( resdata,
				   func->GetScriptParameter( 0, L, (void*)2 ),//参数
				   func->GetScriptParameter( 1, L, (void*)3 ),
				   func->GetScriptParameter( 2, L, (void*)4 ),
				   func->GetScriptParameter( 3, L, (void*)5 ),
				   func->GetScriptParameter( 4, L, (void*)6 ),
				   func->GetScriptParameter( 5, L, (void*)7 )
				   );break;
		default:ASSERT(false);
		}
		func->PushScriptReslut( L, resdata );
		return 1;
	}

	//-------------------------------------------------------------------------------------------------------
	static int SingleClassFunction(lua_State *L)//只有一个函数的同名函数
	{
		BohgeEngine::ClassRegister* sc = 
			(BohgeEngine::ClassRegister*)lua_touserdata(L, lua_upvalueindex(1));
		int index =	lua_tonumber(L, lua_upvalueindex(2));
		BohgeEngine::FunctionRegister*func = sc->GetFunction(index);
		uint ac = func->GetArgumentCount();
		bool hasRes = func->hasReturn();
		if ( hasRes )
		{//如果有返回值
			void* resdata = func->GetScriptResultValue( L, NULL );
			switch ( ac )
			{
			case 0:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject()//成员函数第一个永远是this指针
					   );break;
			case 1:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 )
					   );break;
			case 2:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 )
					   );break;
			case 3:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 )
					   );break;
			case 4:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 )
					   );break;
			case 5:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 ),
					   func->GetScriptParameter( 4, L, (void*)6 )
					   );break;
			case 6:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 ),
					   func->GetScriptParameter( 4, L, (void*)6 ),
					   func->GetScriptParameter( 5, L, (void*)7 )
					   );break;
			default:ASSERT(false);
			}
			func->PushScriptReslut( L, resdata );
		}
		else
		{
			switch ( ac )
			{
			case 0:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject()
					   );break;
			case 1:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 )//参数
					   );break;
			case 2:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 )
					   );break;
			case 3:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 )
					   );break;
			case 4:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 )
					   );break;
			case 5:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 2, L, (void*)3 ),
					   func->GetScriptParameter( 3, L, (void*)4 ),
					   func->GetScriptParameter( 4, L, (void*)5 ),
					   func->GetScriptParameter( 5, L, (void*)6 )
					   );break;
			case 6:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 ),
					   func->GetScriptParameter( 4, L, (void*)6 ),
					   func->GetScriptParameter( 5, L, (void*)7 )
					   );break;
			case 7:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 ),
					   func->GetScriptParameter( 4, L, (void*)6 ),
					   func->GetScriptParameter( 5, L, (void*)7 ),
					   func->GetScriptParameter( 6, L, (void*)8 )
					   );break;
			default:ASSERT(false);
			}
		}
		return hasRes ? 1 : 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int OverrideClassFunction(lua_State *L)//重载的函数
	{
		BohgeEngine::ClassRegister* sc = 
			(BohgeEngine::ClassRegister*)lua_touserdata(L, lua_upvalueindex(1));
		int index =	lua_tonumber(L, lua_upvalueindex(2));
		uint ac = lua_gettop(L) - 1;//得到参数数量，减一的原因是不需要传入self
		BohgeEngine::FunctionRegister* func = NULL;
		switch ( ac )
		{
		case 1:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 )
				   );break;
		case 2:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 ),
				   ObjectDefinedHelper( L, 3 )
				   );break;
		case 3:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 ),
				   ObjectDefinedHelper( L, 3 ),
				   ObjectDefinedHelper( L, 4 )
				   );break;
		case 4:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 ),
				   ObjectDefinedHelper( L, 3 ),
				   ObjectDefinedHelper( L, 4 ),
				   ObjectDefinedHelper( L, 5 )
				   );break;
		case 5:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 ),
				   ObjectDefinedHelper( L, 3 ),
				   ObjectDefinedHelper( L, 4 ),
				   ObjectDefinedHelper( L, 5 ),
				   ObjectDefinedHelper( L, 6 )
				   );break;
		case 6:func = sc->GetFunction( index,
				   ObjectDefinedHelper( L, 2 ),
				   ObjectDefinedHelper( L, 3 ),
				   ObjectDefinedHelper( L, 4 ),
				   ObjectDefinedHelper( L, 5 ),
				   ObjectDefinedHelper( L, 6 ),
				   ObjectDefinedHelper( L, 7 )
				   );break;
		default:ASSERT(false);
		}

		bool hasRes = func->hasReturn();
		if ( hasRes )
		{//如果有返回值
			void* resdata = func->GetScriptResultValue( L, NULL );
			switch ( ac )
			{
			case 1:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 )
					   );break;
			case 2:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 )
					   );break;
			case 3:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 )
					   );break;
			case 4:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 )
					   );break;
			case 5:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 ),
					   func->GetScriptParameter( 4, L, (void*)6 )
					   );break;
			case 6:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 ),
					   func->GetScriptParameter( 4, L, (void*)6 ),
					   func->GetScriptParameter( 5, L, (void*)7 )
					   );break;
			case 7:func->Invoke( resdata,
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 ),
					   func->GetScriptParameter( 4, L, (void*)6 ),
					   func->GetScriptParameter( 5, L, (void*)7 ),
					   func->GetScriptParameter( 6, L, (void*)8 )
					   );break;
			default:ASSERT(false);
			}
			func->PushScriptReslut( L, resdata );
		}
		else
		{
			switch ( ac )
			{
			case 1:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 )//参数
					   );break;
			case 2:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 )
					   );break;
			case 3:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 )
					   );break;
			case 4:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 )
					   );break;
			case 5:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 ),
					   func->GetScriptParameter( 4, L, (void*)6 )
					   );break;
			case 6:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 ),
					   func->GetScriptParameter( 4, L, (void*)6 ),
					   func->GetScriptParameter( 5, L, (void*)7 )
					   );break;
			case 7:func->Invoke(
					   ((BohgeEngine::ScriptValue*)lua_touserdata(L,1))->GetObject(),//this指针
					   func->GetScriptParameter( 0, L, (void*)2 ),//参数
					   func->GetScriptParameter( 1, L, (void*)3 ),
					   func->GetScriptParameter( 2, L, (void*)4 ),
					   func->GetScriptParameter( 3, L, (void*)5 ),
					   func->GetScriptParameter( 4, L, (void*)6 ),
					   func->GetScriptParameter( 5, L, (void*)7 ),
					   func->GetScriptParameter( 6, L, (void*)8 )
					   );break;
			default:ASSERT(false);
			}
		}
		return hasRes ? 1 : 0;
	}
}