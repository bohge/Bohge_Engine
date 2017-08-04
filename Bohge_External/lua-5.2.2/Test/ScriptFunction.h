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
#include "Predefine.h"
#include "VariableType.h"

#include <string>
#include <vector>

namespace BohgeEngine
{
	class Rtti;
	class ScriptFunction
	{
	private:
		typedef std::vector<const Rtti*> ArgumentArray;
	private:
		ArgumentArray		m_ArgumentRttis;
		std::string			m_FuncName;
		const Rtti*			m_pResult;
		void*				m_pFunction;
		uint				m_ID;
	public:
		ScriptFunction(void);
		~ScriptFunction(void);
	private:
		void _CaculateID();//计算函数标识符
	public:
		void Result( const Rtti* rtti );
		void Arguments( const Rtti* rtti1 );
		void Arguments( const Rtti* rtti1, const Rtti* rtti2 );
		void Arguments( const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3 );
		void Arguments( const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4 );
		void Arguments( const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5 );
		void Arguments( const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6 );
		void Arguments( const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7 );
		void Arguments( const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8 );
		void Arguments( const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9 );
		void Arguments( const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9, const Rtti* rtti10 );
		void Arguments( const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9, const Rtti* rtti10, const Rtti* rtti11 );
	public:
		template<typename FUNC>
		void Function( FUNC f, const std::string& name )
		{
			m_pFunction = (void*)f;
			m_FuncName = name;
		}
	public:
		BOHGE_FORCEINLINE const Rtti* GetArguments( int index )
		{
			return m_ArgumentRttis[index];
		}
		BOHGE_FORCEINLINE const Rtti* GetReturnType()
		{
			return m_pResult;
		}
		BOHGE_FORCEINLINE uint GetID() const
		{
			return m_ID;
		}
		BOHGE_FORCEINLINE const std::string& GetName() const
		{
			return m_FuncName;
		}
		BOHGE_FORCEINLINE bool hasReturn() const//是否有返回值
		{
			return NULL != m_pResult;
		}
		BOHGE_FORCEINLINE uint GetArgumentCount() const
		{
			return m_ArgumentRttis.size();
		}
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