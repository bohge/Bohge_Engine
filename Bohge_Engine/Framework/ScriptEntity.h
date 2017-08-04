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
#include "IScriptResource.h"
#include "ScriptSource.h"
#include "IAsyncEntity.h"


namespace BohgeEngine
{
	class BOHGE_FRAMEWORK ScriptEntity : public IAsyncEntity< IScriptResource, ScriptSource >
	{
		RTTI_DRIVER_TYPE_DECLEAR( ScriptEntity, IAsyncEntity );
	public:
		ScriptEntity(void);
		~ScriptEntity(void);
	private:
		virtual void _OnCreateResource();//准备加载资源的时候调用
		virtual void _OnResourceCreated();//资源加载完毕的回掉
	public:
		BOHGE_FORCEINLINE void AsLogicScript();//注册到系统上，系统负责调用循环
	public:
		BOHGE_FORCEINLINE IFunctionInvoker* SignFunciton( const eastl::string& name, uint argCount )
		{
			return _GetResourcePtr()->SignFunciton( name, argCount );
		}
		BOHGE_FORCEINLINE IFunctionInvoker* SignFunciton( int userdata, uint argCount )
		{
			return _GetResourcePtr()->SignFunciton( userdata, argCount );
		}
		BOHGE_FORCEINLINE void UnsignFunction( IFunctionInvoker* func )
		{
			_GetResourcePtr()->UnsignFunction( func );
		}
		BOHGE_FORCEINLINE void SetMember( const char* name, const Object* obj )
		{
			_GetResourcePtr()->SetMember( name, obj );
		}
		BOHGE_FORCEINLINE const void* GetMember( const char* name )
		{
			return _GetResourcePtr()->GetMember( name );
		}
		BOHGE_FORCEINLINE uint GetScriptHash()
		{
			return _GetResourcePtr()->GetScriptHash();
		}
		BOHGE_FORCEINLINE int GetScriptInstance()
		{
			return _GetResourcePtr()->GetScriptInstance();
		}
	};
}