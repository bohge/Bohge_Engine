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
#include "ScriptEntity.h"
#include "IComponent.h"
#include "Utility.hpp"
#include "IJob.h"



#include <EASTL/string.h>

namespace BohgeEngine
{
	class IMetadata;
	class Framework;
	class ScriptValue;
	class IAsyncSource;
	class IFunctionInvoker;
	class BOHGE_FRAMEWORK ScriptComponent : public IComponent
	{
		RTTI_DRIVER_TYPE_DECLEAR( ScriptComponent, IComponent )
	private:
		ScriptEntity*				m_pScriptEntity;
	public:
		ScriptComponent(void);
		virtual ~ScriptComponent(void);
	private:
		virtual void _DoOnAttachNode( );//当挂接在node上
	public:
		virtual void OnRootNodeChange( );//当所属node的根节点变化时
		virtual void Release( );
	public:
		void OnScriptLoaded( const SmartPtr<IAsyncSource>& source );
	public:
		void SetScriptPath( const eastl::string& path );
		void SetJobType( IJob::JobType jt );
		IFunctionInvoker* SignFunciton( const eastl::string& name, uint argCount );//多次注册安全，底层需查询
		IFunctionInvoker* SignFunciton( int userdata, uint argCount );//多次注册安全，底层需查询
		void UnsignFunction( IFunctionInvoker* func );
		void SetMember( const char* name, const Object* obj );
		const void* GetMember( const char* name );
		void DependenceSource( IAsyncSource* source );
		void ConnectLoadedEvent( Delegate1<void, const SmartPtr<IAsyncSource>& >::FunctionType* ptr );
	public:
		BOHGE_FORCEINLINE void CreateResource()
		{
			m_pScriptEntity->CreateResource();
		}
		BOHGE_FORCEINLINE int GetScriptInstance()
		{
			return m_pScriptEntity->GetScriptInstance();
		}
	};
}