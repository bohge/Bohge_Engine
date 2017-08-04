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
#include "ThreadSafeOprator.h"





namespace BohgeEngine
{
	template<typename T>
	class SmartPtr
	{
	private:
		T*					m_pPointer;
		volatile int*		m_pReferences;
	public:
		//------------------------------------------------------------------------------------------------------
		SmartPtr()
			:m_pPointer(NULL),
			m_pReferences(NULL)
		{
		}
		//------------------------------------------------------------------------------------------------------
		explicit SmartPtr( T* ptr )
			:m_pPointer(NULL),
			m_pReferences(NULL)
		{
			_Copy( ptr );
		}
		//------------------------------------------------------------------------------------------------------
		SmartPtr( T* ptr, volatile int* ref )
			:m_pPointer(NULL),
			m_pReferences(NULL)
		{
			_Copy( ptr, ref );
		}
		//------------------------------------------------------------------------------------------------------
		SmartPtr(const SmartPtr<T>& sp)
			:m_pPointer(NULL),
			m_pReferences(NULL)
		{
			_Copy( sp );
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		explicit SmartPtr( U* ptr )
			:m_pPointer(NULL),
			m_pReferences(NULL)
		{
			_Copy( ptr );
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		SmartPtr( U* ptr, volatile int* ref )
			:m_pPointer(NULL),
			m_pReferences(NULL)
		{
			_Copy( ptr, ref );
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		SmartPtr(const SmartPtr<U>& sp)
			:m_pPointer(NULL),
			m_pReferences(NULL)
		{
			_Copy( sp );
		}
		//------------------------------------------------------------------------------------------------------
		~SmartPtr()
		{
			_TryRelease();
		}
	private:
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		BOHGE_FORCEINLINE void _Copy( U* ptr )
		{
			T* ptrT = reinterpret_cast<T*>(ptr);
			if ( m_pPointer != ptrT )
			{
				_TryRelease();
				m_pPointer = ptrT;
				m_pReferences = NULL == ptrT ? NULL : new int(1);
			}
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		BOHGE_FORCEINLINE void _Copy( U* ptr, volatile int* ref )
		{
			T* ptrT = reinterpret_cast<T*>(ptr);
			if ( m_pPointer != ptrT )
			{
				_TryRelease();
				m_pPointer = ptrT;
				m_pReferences = ref;
				if(m_pReferences)ThreadSafeOprator::Increment( *m_pReferences );
			}
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		BOHGE_FORCEINLINE void _Copy( const SmartPtr<U>& sp )//拷贝
		{
			T* ptr = reinterpret_cast<T*>(sp.GetUnsafePointer());
			if ( m_pPointer != ptr )
			{
				_TryRelease();
				m_pPointer = ptr;
				m_pReferences = sp.GetUnsafeReferences();
				if(m_pReferences)ThreadSafeOprator::Increment( *m_pReferences );
			}
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void _TryRelease()
		{
			if ( NULL != m_pReferences //短路求值
				&& 0 == ThreadSafeOprator::Decrement( *m_pReferences ) )
			{
				SAFE_DELETE( m_pPointer );
				SAFE_DELETE( m_pReferences );
			}
		}
	public:
		//类内使用friend使其成为一个non-member函数
		//------------------------------------------------------------------------------------------------------
		friend BOHGE_FORCEINLINE bool operator == ( const SmartPtr<T>& sp, const T* const ptr )
		{
			return sp.m_pPointer == ptr;
		}
		//------------------------------------------------------------------------------------------------------
		friend BOHGE_FORCEINLINE bool operator == ( const T* const ptr, const SmartPtr<T>& sp )
		{
			return sp.m_pPointer == ptr;
		}
		//------------------------------------------------------------------------------------------------------
		friend BOHGE_FORCEINLINE bool operator != ( const SmartPtr<T>& sp, const T* const ptr )
		{
			return sp.m_pPointer != ptr;
		}
		//------------------------------------------------------------------------------------------------------
		friend BOHGE_FORCEINLINE bool operator != ( const T* const ptr, const SmartPtr<T>& sp )
		{
			return sp.m_pPointer != ptr;
		}
		//------------------------------------------------------------------------------------------------------
		template<typename CASTTO>
		friend BOHGE_FORCEINLINE SmartPtr<CASTTO> SmartPtr_Static_Cast( const SmartPtr<T>& rhs )
		{
			return SmartPtr<CASTTO>( static_cast<CASTTO*>(rhs.m_pPointer), rhs.m_pReferences );
		}
		//------------------------------------------------------------------------------------------------------
		template<typename CASTTO>
		friend BOHGE_FORCEINLINE SmartPtr<CASTTO> SmartPtr_Dynamic_Cast( const SmartPtr<T>& rhs )
		{
			return SmartPtr<CASTTO>( dynamic_cast<CASTTO*>(rhs.m_pPointer), rhs.m_pReferences );
		}
	public:
		BOHGE_FORCEINLINE T* GetUnsafePointer() const
		{
			return m_pPointer;
		}
		BOHGE_FORCEINLINE volatile int* GetUnsafeReferences() const
		{
			return m_pReferences;
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		BOHGE_FORCEINLINE const SmartPtr<T>& operator = ( const SmartPtr<U>& sp )
		{
			_Copy( sp );
			return *this;
		}
		//------------------------------------------------------------------------------------------------------
		//template<typename U>//这个函数太危险了
		//BOHGE_FORCEINLINE const SmartPtr<T>& operator = ( U* ptr )
		//{
		//	_Copy( ptr );
		//	return *this;
		//}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const SmartPtr<T>& operator = ( const SmartPtr<T>& sp )
		{
			_Copy( sp );
			return *this;
		}
		//------------------------------------------------------------------------------------------------------
		//BOHGE_FORCEINLINE const SmartPtr<T>& operator = ( T* ptr )//这个函数太危险了
		//{
		//	_Copy( ptr );
		//	return *this;
		//}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE T* operator-> () const
		{
			return m_pPointer;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE T& operator* () const
		{
			return *m_pPointer;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool operator == ( const SmartPtr<T>& rhs ) const
		{
			return m_pPointer == rhs->m_pPointer;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool operator != ( const SmartPtr<T>& rhs ) const
		{
			return m_pPointer != rhs->m_pPointer;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE intptr_t AsIntPtr () const
		{
			return (intptr_t)m_pPointer;
		}
		//------------------------------------------------------------------------------------------------------
//		template<typename CASTTO> BOHGE_FORCEINLINE operator SmartPtr<CASTTO>& ()
//		{
//#ifdef _DEBUG
//			CASTTO* ptr = this->m_pPointer;//检测指针转换是否是继承关系
//#endif
//			return *reinterpret_cast< SmartPtr<CASTTO>* >(this);
//		}
	};
}