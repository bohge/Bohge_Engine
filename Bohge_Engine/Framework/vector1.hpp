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



//////////////////////////////////////////////
//	3DMath的拆分实现，请不要单独引用该头文件	//
//////////////////////////////////////////////
#ifndef VECTOR1
#define VECTOR1
#else
#error( "Do not include this file in other files" )
#endif
#include "Volume.hpp"



namespace BohgeEngine
{
	template<typename T>
	class vector1 : public Volume
	{
		RTTI_TYPE_DECLEAR( vector1 )
	public:
		typedef T value_type;
	public:
		union
		{
			T	m_x;
			T	m_Data[1];
		};
	public:
		BOHGE_FORCEINLINE vector1():m_x(0){}
		BOHGE_FORCEINLINE vector1(T x):m_x(x){}
		template< typename R >
		BOHGE_FORCEINLINE vector1(const vector1<R>& input)
		{
			m_x = static_cast<T>( input.m_x );
		}
		template< typename R >
		BOHGE_FORCEINLINE vector1& operator= (const vector1<R>& input)
		{
			m_x = static_cast<T>( input.m_x );
			return *this;
		}
		BOHGE_FORCEINLINE vector1& operator= (const vector1& input)
		{
			m_x = input.m_x;
			return *this;
		}
		//vector1
		BOHGE_FORCEINLINE vector1& operator+= (const vector1& input)
		{
			m_x+=input.m_x;
			return *this;
		}
		BOHGE_FORCEINLINE vector1& operator-= (const vector1& input)
		{
			m_x-=input.m_x;
			return *this;
		}
		BOHGE_FORCEINLINE vector1& operator*= (const vector1& input)
		{
			m_x*=input.m_x;
			return *this;
		}
		BOHGE_FORCEINLINE vector1& operator/= (const vector1& input)
		{
			m_x/=input.m_x;
			return *this;
		}
		BOHGE_FORCEINLINE vector1 operator* (const vector1& input) const
		{
			return vector1( m_x*input.m_x );
		}
		BOHGE_FORCEINLINE vector1 operator+ (const vector1& input) const
		{
			return vector1( m_x+input.m_x );
		}
		BOHGE_FORCEINLINE vector1 operator- (const vector1& input) const
		{
			return vector1( m_x-input.m_x );
		}
		BOHGE_FORCEINLINE vector1 operator/ (const vector1& input) const
		{
			return vector1( m_x/input.m_x );
		}
		BOHGE_FORCEINLINE vector1 operator- () const
		{
			return vector1( -m_x );
		}
		//单个的数字
		BOHGE_FORCEINLINE vector1 operator/ (T num) const
		{
			return vector1( m_x/num );
		}
		friend BOHGE_FORCEINLINE vector1<T> operator* ( T num, const vector1<T>& rhs )
		{
			return vector1<T>( rhs.m_x*num );
		}
		friend BOHGE_FORCEINLINE vector1<T> operator* ( const vector1<T>& rhs, T num )
		{
			return vector1<T>( rhs.m_x*num );
		}
		BOHGE_FORCEINLINE vector1 operator- (T num) const
		{
			return vector1( m_x-num );
		}
		BOHGE_FORCEINLINE vector1 operator+ (T num) const
		{
			return vector1( m_x+num );
		}
		BOHGE_FORCEINLINE vector1& operator*= (T num)
		{
			m_x*=num;
			return *this;
		}
		BOHGE_FORCEINLINE vector1& operator/= (T num)
		{
			m_x/=num;
			return *this;
		}
		BOHGE_FORCEINLINE vector1& operator+= (T num)
		{
			m_x+=num;
			return *this;
		}
		BOHGE_FORCEINLINE vector1& operator-= (T num)
		{
			m_x-=num;
			return *this;
		}
		//逻辑
		BOHGE_FORCEINLINE bool operator == (const vector1& rhs) const
		{
			return Math::isEqual( m_x, rhs.m_x);
		}
		BOHGE_FORCEINLINE bool operator != (const vector1& rhs) const
		{
			return !( *this == rhs );
		}

		//取值
		BOHGE_FORCEINLINE operator T()
		{
			return static_cast<T>( m_x );
		}
		BOHGE_FORCEINLINE operator const T() const
		{
			return static_cast<T>( m_x );
		}

		BOHGE_FORCEINLINE T& operator[] ( int i )
		{
			return m_Data[i];
		}
		BOHGE_FORCEINLINE const T& operator[] ( int i ) const
		{
			return m_Data[i];
		}
	public:
		virtual void* Data()
		{
			return m_Data;
		}
		virtual const void* Data() const
		{
			return m_Data;
		}
		virtual uint Count() const
		{
			return 1;
		}
		static uint Size()
		{
			return 1;
		}
		static uint BitSize()
		{
			return sizeof(T);
		}
	};
}