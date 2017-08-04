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
#include "ScriptProperty.h"
#include "IScriptSystem.h"
#include "ScriptUtility.h"
#include "Object.hpp"

#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace BohgeEngine
{
	class Rtti;
	class BOHGE_FRAMEWORK FunctionRegister// : public Object
	{
		//RTTI_TYPE_DECLEAR( ScriptFunction )
	private:
		typedef eastl::vector<const Rtti*> ArgumentArray;
		typedef eastl::vector< IScriptSystem::ScriptFunction > GetArgumentFunctionArray;
	private:
		eastl::string					m_FuncName;
		ArgumentArray					m_ArgumentRttis;
		GetArgumentFunctionArray		m_ArgumentGetter;
		IScriptSystem::ScriptFunction	m_pResultGetter;
		IScriptSystem::ScriptFunction	m_pResultPusher;
		ScriptProperty::FunctionType	m_eFunctionType;
		const Rtti*						m_pResult;
		void*							m_pFunction;
		uint							m_ID;
	public:
		FunctionRegister(void);
		~FunctionRegister(void);
	private:
		void _CaculateID();//计算函数标识符
	public:
		void Result( const IScriptSystem& sys, const Rtti* rtti );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1 );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1, const Rtti* rtti2 );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3 );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4 );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5 );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6 );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7 );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8 );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9 );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9, const Rtti* rtti10 );
		void Arguments( const IScriptSystem& sys, const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9, const Rtti* rtti10, const Rtti* rtti11 );
	public:
		template<typename FUNC>
		void Function( FUNC f, const eastl::string& name )
		{
			m_pFunction = (void*)f;
			m_FuncName = name;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isMatchArguments( const Rtti* o1 )
		{
			ASSERT( 1 == GetArgumentCount() );
			return 
				CaculateFunctionID( o1->GetTypeID() ) == m_ID
				|| o1->isTypeOrDriverType( *GetArguments(0) );
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isMatchArguments( const Rtti* o1, const Rtti* o2 )
		{
			ASSERT( 2 == GetArgumentCount() );
			return 
				CaculateFunctionID(
				o1->GetTypeID(),
				o2->GetTypeID()
				) == m_ID
				|| ( 
				o1->isTypeOrDriverType( *GetArguments(0) )
				&& o2->isTypeOrDriverType( *GetArguments(1) )
				);
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isMatchArguments( const Rtti* o1, const Rtti* o2, const Rtti* o3 )
		{
			ASSERT( 3 == GetArgumentCount() );
			return 
				CaculateFunctionID(
				o1->GetTypeID(),
				o2->GetTypeID(),
				o3->GetTypeID()
				) == m_ID
				|| ( 
				o1->isTypeOrDriverType( *GetArguments(0) )
				&& o2->isTypeOrDriverType( *GetArguments(1) )
				&& o3->isTypeOrDriverType( *GetArguments(2) )
				);
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isMatchArguments( const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4 )
		{
			ASSERT( 4 == GetArgumentCount() );
			return 
				CaculateFunctionID(
				o1->GetTypeID(),
				o2->GetTypeID(),
				o3->GetTypeID(),
				o4->GetTypeID()
				) == m_ID
				|| ( 
				o1->isTypeOrDriverType( *GetArguments(0) )
				&& o2->isTypeOrDriverType( *GetArguments(1) )
				&& o3->isTypeOrDriverType( *GetArguments(2) )
				&& o4->isTypeOrDriverType( *GetArguments(3) )
				);
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isMatchArguments( const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4, const Rtti* o5 )
		{
			ASSERT( 5 == GetArgumentCount() );
			return 
				CaculateFunctionID(
				o1->GetTypeID(),
				o2->GetTypeID(),
				o3->GetTypeID(),
				o4->GetTypeID(),
				o5->GetTypeID()
				) == m_ID
				|| ( 
				o1->isTypeOrDriverType( *GetArguments(0) )
				&& o2->isTypeOrDriverType( *GetArguments(1) )
				&& o3->isTypeOrDriverType( *GetArguments(2) )
				&& o4->isTypeOrDriverType( *GetArguments(3) )
				&& o5->isTypeOrDriverType( *GetArguments(4) )
				);
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isMatchArguments( const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4, const Rtti* o5, const Rtti* o6 )
		{
			ASSERT( 6 == GetArgumentCount() );
			return 
				CaculateFunctionID(
				o1->GetTypeID(),
				o2->GetTypeID(),
				o3->GetTypeID(),
				o4->GetTypeID(),
				o5->GetTypeID(),
				o6->GetTypeID()
				) == m_ID
				|| ( 
				o1->isTypeOrDriverType( *GetArguments(0) )
				&& o2->isTypeOrDriverType( *GetArguments(1) )
				&& o3->isTypeOrDriverType( *GetArguments(2) )
				&& o4->isTypeOrDriverType( *GetArguments(3) )
				&& o5->isTypeOrDriverType( *GetArguments(4) )
				&& o5->isTypeOrDriverType( *GetArguments(5) )
				);
		}
	public:
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void* GetScriptResultValue( void* p1, void* p2 )
		{
			return m_pResultGetter( p1, p2 );
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void* PushScriptReslut( void* p1, void* p2 )
		{
			return m_pResultPusher( p1, p2 );
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void* GetScriptParameter( int index, void* l, void* stack )
		{
			return m_ArgumentGetter[index]( l, stack );
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const Rtti* GetArguments( int index )
		{
			return m_ArgumentRttis[index];
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const Rtti* GetReturnType()
		{
			return m_pResult;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const eastl::string& GetName() const
		{
			return m_FuncName;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool hasReturn() const//是否有返回值
		{
			return NULL != m_pResult;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetArgumentCount() const
		{
			return m_ArgumentRttis.size();
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE ScriptProperty::FunctionType GetFunctionType() const
		{
			return m_eFunctionType;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetFunctionType( ScriptProperty::FunctionType ft )
		{
			m_eFunctionType = ft;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void Invoke()
		{
			((void(*)(void))m_pFunction)();
		}
		BOHGE_FORCEINLINE void Invoke(void* p1)
		{
			((void(*)(void*))m_pFunction)( p1 );
		}
		BOHGE_FORCEINLINE void Invoke(void* p1, void* p2)
		{
			((void(*)(void*, void*))m_pFunction)( p1, p2 );
		}
		BOHGE_FORCEINLINE void Invoke(void* p1, void* p2, void* p3)
		{
			((void(*)(void*, void*, void*))m_pFunction)( p1, p2, p3 );
		}
		BOHGE_FORCEINLINE void Invoke(void* p1, void* p2, void* p3, void* p4)
		{
			((void(*)(void*, void*, void*, void*))m_pFunction)( p1, p2, p3, p4 );
		}
		BOHGE_FORCEINLINE void Invoke(void* p1, void* p2, void* p3, void* p4, void* p5)
		{
			((void(*)(void*, void*, void*, void*, void*))m_pFunction)( p1, p2, p3, p4, p5 );
		}
		BOHGE_FORCEINLINE void Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6)
		{
			((void(*)(void*, void*, void*, void*, void*, void*))m_pFunction)( p1, p2, p3, p4, p5, p6 );
		}
		BOHGE_FORCEINLINE void Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7)
		{
			((void(*)(void*, void*, void*, void*, void*, void*, void*))m_pFunction)( p1, p2, p3, p4, p5, p6, p7 );
		}
		BOHGE_FORCEINLINE void Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8)
		{
			((void(*)(void*, void*, void*, void*, void*, void*, void*, void*))m_pFunction)( p1, p2, p3, p4, p5, p6, p7, p8 );
		}
		BOHGE_FORCEINLINE void Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9)
		{
			((void(*)(void*, void*, void*, void*, void*, void*, void*, void*, void*))m_pFunction)( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
		}
		BOHGE_FORCEINLINE void Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10)
		{
			((void(*)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*))m_pFunction)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
		}
		BOHGE_FORCEINLINE void Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10, void* p11)
		{
			((void(*)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*))m_pFunction)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11 );
		}
	};
}