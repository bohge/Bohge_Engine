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
#include "IMonopolyResource.h"
#include "IScriptSystem.h"



#include <EASTL/map.h>

namespace BohgeEngine
{
	class IFunctionInvoker;
	class BOHGE_FRAMEWORK IScriptResource : public IMonopolyResource
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IScriptResource, IMonopolyResource )
	private:
		struct UserFunction
		{
			volatile int		m_User;
			IFunctionInvoker*	m_pInvoker;
			UserFunction( IFunctionInvoker* inv )
				:m_User(1),
				m_pInvoker(inv){}
		};
	private:
		typedef eastl::map< uint, UserFunction* > FuncMap;
	private:
		int				m_InstanceHandle;
		FuncMap			m_FunctionMap;
		uint			m_nHashCode;
		ScriptProperty::ScriptThread	m_eThread;
	public:
		IScriptResource(void);
		virtual ~IScriptResource(void);
	private:
		virtual bool _isFunctionExist( int h, const eastl::string& name ) = 0;
		virtual IFunctionInvoker* _DoSignFunciton( int h, const eastl::string& name, uint argCount ) = 0;
		virtual IFunctionInvoker* _DoSignFunciton( int h, int userdata, uint argCount ) = 0;
		virtual void _DoReleaseFunction( int h, BohgeEngine::IFunctionInvoker* func ) = 0;
		virtual void _DoSetMember( int h, const char* name, const Object* obj ) = 0;
		virtual const void* _DoGetMember( int h, const char* name ) = 0;
		virtual void _DoRelease( int h ) = 0;
	public:
		virtual void ReleaseResource();
	public:
		//签名一个函数
		IFunctionInvoker* SignFunciton( const eastl::string& name, uint argCount );
		IFunctionInvoker* SignFunciton( int userdata, uint argCount );
		void UnsignFunction( IFunctionInvoker* func );
		void ReleaseAll();
	public:
		BOHGE_FORCEINLINE void SetMember( const char* name, const Object* obj )
		{
			_DoSetMember( m_InstanceHandle, name, obj );
		}
		BOHGE_FORCEINLINE const void* GetMember( const char* name )
		{
			return _DoGetMember( m_InstanceHandle, name );
		}
		BOHGE_FORCEINLINE void SetInstanceHandle( int h )
		{
			m_InstanceHandle = h;
		}
		BOHGE_FORCEINLINE void SetScriptHash( uint hash )
		{
			m_nHashCode = hash;
		}
		BOHGE_FORCEINLINE uint GetScriptHash()
		{
			return m_nHashCode;
		}
		BOHGE_FORCEINLINE void SetScriptThread( ScriptProperty::ScriptThread st )
		{
			m_eThread = st;
		}
		BOHGE_FORCEINLINE ScriptProperty::ScriptThread GetScriptThread() const
		{
			return m_eThread;
		}
		BOHGE_FORCEINLINE int GetScriptInstance()
		{
			return m_InstanceHandle;
		}
	};
}

