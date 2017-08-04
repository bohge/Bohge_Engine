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
#include "ScriptFunction.h"
#include "ScriptUtility.h"
#include "ScriptClass.h"
#include "RttiObject.hpp"
#include "ScriptValue.h"

extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lobject.h"
}


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static int ClassDestruct(lua_State *L)//析构函数
	{
		ScriptValue* data = (ScriptValue*)lua_touserdata(L,1);
		data->~ScriptValue();
		 return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int SingleClassConstruct(lua_State *L)//构造函数
	{
		ScriptClass* sc = 
			(ScriptClass*)lua_touserdata(L, lua_upvalueindex(1));
		int index =	lua_tonumber(L, lua_upvalueindex(2));
		ScriptFunction*func = sc->GetFunction(index);

		uint ac = func->GetArgumentCount();
		bool hasRes = func->hasReturn();
		if ( hasRes )
		{//如果有返回值
			ScriptValue* resdata = (ScriptValue*)lua_newuserdata(L, sizeof(ScriptValue) );
			switch ( ac )
			{
			case 0:func->Invoke( resdata );break;
			case 1:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject() 
					   );break;
			case 2:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject() 
					   );break;
			case 3:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject() 
					   );break;
			case 4:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject() 
					   );break;
			case 5:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject() 
					   );break;
			case 6:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,6))->GetObject() 
					   );break;
			default:ASSERT(false);
			}
			lua_getglobal( L, resdata->GetObject()->GetTypeName().c_str() );
			lua_setmetatable(L, -2);
		}
		else
		{
			switch ( ac )
			{
			case 0:func->Invoke( );break;
			case 1:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject() 
					   );break;
			case 2:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject() 
					   );break;
			case 3:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject() 
					   );break;
			case 4:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject() 
					   );break;
			case 5:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,6))->GetObject() 
					   );break;
			case 6:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,6))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,7))->GetObject() 
					   );break;
			default:ASSERT(false);
			}
		}
		return hasRes ? 1 : 0;
	}

	//-------------------------------------------------------------------------------------------------------
	static int OverrideClassConstruct(lua_State *L)//重载的函数
	{
		ScriptClass* sc = 
			(ScriptClass*)lua_touserdata(L, lua_upvalueindex(1));
		int index =	lua_tonumber(L, lua_upvalueindex(2));
		uint ac = lua_gettop(L) - 1;//得到参数数量，减一的原因是不需要传入self
		ScriptFunction* func = NULL;
		switch ( ac )
		{
		case 0:func = sc->GetFunction( index, CaculateFunctionID() );break;
		case 1:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID()
				   ) );break;
		case 2:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,3))->GetObject()->GetTypeID()
				   ) );break;
		case 3:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,3))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,4))->GetObject()->GetTypeID()
				   ) );break;
		case 4:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,3))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,4))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,5))->GetObject()->GetTypeID()
				   ) );break;
		case 5:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,3))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,4))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,5))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,6))->GetObject()->GetTypeID()
				   ) );break;
		case 6:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,3))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,4))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,5))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,6))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,7))->GetObject()->GetTypeID()
				   ) );break;
		default:ASSERT(false);
		}

		bool hasRes = func->hasReturn();
		if ( hasRes )
		{//如果有返回值
			ScriptValue* resdata = (ScriptValue*)lua_newuserdata(L, sizeof(ScriptValue) );
			switch ( ac )
			{
			case 0:func->Invoke( resdata );break;
			case 1:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject() 
					   );break;
			case 2:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject() 
					   );break;
			case 3:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject() 
					   );break;
			case 4:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject() 
					   );break;
			case 5:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject() 
					   );break;
			case 6:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,6))->GetObject() 
					   );break;
			default:ASSERT(false);
			}
			lua_getglobal( L, resdata->GetObject()->GetTypeName().c_str() );
			lua_setmetatable(L, -2);
		}
		else
		{
			switch ( ac )
			{
			case 0:func->Invoke( );break;
			case 1:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject() 
					   );break;
			case 2:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject() 
					   );break;
			case 3:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject() 
					   );break;
			case 4:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject() 
					   );break;
			case 5:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,6))->GetObject() 
					   );break;
			case 6:func->Invoke( 
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,6))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,7))->GetObject() 
					   );break;
			default:ASSERT(false);
			}
		}
		return hasRes ? 1 : 0;
	}

	//-------------------------------------------------------------------------------------------------------
	static int SingleClassFunction(lua_State *L)//只有一个函数的同名函数
	{
		ScriptClass* sc = 
			(ScriptClass*)lua_touserdata(L, lua_upvalueindex(1));
		int index =	lua_tonumber(L, lua_upvalueindex(2));
		ScriptFunction*func = sc->GetFunction(index);
		uint ac = func->GetArgumentCount();
		bool hasRes = func->hasReturn();
		if ( hasRes )
		{//如果有返回值
			ScriptValue* resdata = (ScriptValue*)lua_newuserdata(L, sizeof(ScriptValue) );
			switch ( ac )
			{
			case 0:func->Invoke( resdata );break;
			case 1:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject() 
					   );break;
			case 2:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject() 
					   );break;
			case 3:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject() 
					   );break;
			case 4:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject() 
					   );break;
			case 5:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject() 
					   );break;
			case 6:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,6))->GetObject() 
					   );break;
			default:ASSERT(false);
			}
			lua_getglobal( L, resdata->GetObject()->GetTypeName().c_str() );
			lua_setmetatable(L, -2);
		}
		else
		{
			switch ( ac )
			{
			case 0:func->Invoke( );break;
			case 1:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject()
					   );break;
			case 2:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject()
					   );break;
			case 3:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject()
					   );break;
			case 4:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject()
					   );break;
			case 5:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject()
					   );break;
			case 6:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,6))->GetObject()
					   );break;
			default:ASSERT(false);
			}
		}
		return hasRes ? 1 : 0;
	}

	//-------------------------------------------------------------------------------------------------------
	static int OverrideClassFunction(lua_State *L)//重载的函数
	{
		ScriptClass* sc = 
			(ScriptClass*)lua_touserdata(L, lua_upvalueindex(1));
		int index =	lua_tonumber(L, lua_upvalueindex(2));
		uint ac = lua_gettop(L);//得到参数数量
		ScriptFunction* func = NULL;
		RttiObject* r1 = ((RttiObject*)((ScriptValue*)lua_touserdata(L,1))->GetObject());
		RttiObject* r2 = ((RttiObject*)((ScriptValue*)lua_touserdata(L,2))->GetObject());
		switch ( ac )
		{
		case 0:func = sc->GetFunction( index, CaculateFunctionID() );break;
		case 1:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID()
				   ) );break;
		case 2:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,3))->GetObject()->GetTypeID()
				   ) );break;
		case 3:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,3))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,4))->GetObject()->GetTypeID()
				   ) );break;
		case 4:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,3))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,4))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,5))->GetObject()->GetTypeID()
				   ) );break;
		case 5:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,3))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,4))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,5))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,6))->GetObject()->GetTypeID()
				   ) );break;
		case 6:func = sc->GetFunction( index, CaculateFunctionID(
				   ((ScriptValue*)lua_touserdata(L,2))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,3))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,4))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,5))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,6))->GetObject()->GetTypeID(),
				   ((ScriptValue*)lua_touserdata(L,7))->GetObject()->GetTypeID()
				   ) );break;
		default:ASSERT(false);
		}

		bool hasRes = func->hasReturn();
		if ( hasRes )
		{//如果有返回值
			ScriptValue* resdata = (ScriptValue*)lua_newuserdata(L, sizeof(ScriptValue) );
			switch ( ac )
			{
			case 0:func->Invoke( resdata );break;
			case 1:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject() 
					   );break;
			case 2:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject() 
					   );break;
			case 3:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject() 
					   );break;
			case 4:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject() 
					   );break;
			case 5:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject() 
					   );break;
			case 6:func->Invoke( resdata,
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,6))->GetObject() 
					   );break;
			default:ASSERT(false);
			}
			lua_getglobal( L, resdata->GetObject()->GetTypeName().c_str() );
			lua_setmetatable(L, -2);
		}
		else
		{
			switch ( ac )
			{
			case 0:func->Invoke( );break;
			case 1:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject()
					   );break;
			case 2:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject()
					   );break;
			case 3:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject()
					   );break;
			case 4:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject()
					   );break;
			case 5:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject()
					   );break;
			case 6:func->Invoke(
					   ((ScriptValue*)lua_touserdata(L,1))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,2))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,3))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,4))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,5))->GetObject(),
					   ((ScriptValue*)lua_touserdata(L,6))->GetObject()
					   );break;
			default:ASSERT(false);
			}
		}
		return hasRes ? 1 : 0;
	}
}