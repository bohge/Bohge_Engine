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
#include "3DMath.h"


#include <EASTL/vector.h>

namespace Utility
{
	template< typename T >
	class T2Byte
	{
	private:
		union Data
		{
			T			m_T;
			byte		m_Byte[sizeof(T)];
		};
	private:
		Data		m_Data;
	public:
		T2Byte( T data )
		{
			m_Data.m_T = data;
		}
	public:
		BOHGE_FORCEINLINE byte GetByte( int i )
		{
			return m_Data.m_Byte[i];
		}
	};
	//------------------------------------------------------------------------------------------------------
	template< typename T > BOHGE_FORCEINLINE void Vector3T2Byte( const BohgeEngine::vector3<T>& vec, eastl::vector<byte>& into )
	{
		T2Byte<T> mx( vec.m_x );
		T2Byte<T> my( vec.m_y );
		T2Byte<T> mz( vec.m_z );
		for ( int i = 0; i < sizeof(T); ++ i )
		{
			into.push_back( mx.GetByte(i) );
			into.push_back( my.GetByte(i) );
			into.push_back( mz.GetByte(i) );
		}
	}
	//------------------------------------------------------------------------------------------------------
	template< typename T > BOHGE_FORCEINLINE void SingleT2Byte( T data, eastl::vector<byte>& into )
	{
		T2Byte<T> t( data );
		for ( int i = 0; i < sizeof(T); ++ i )
		{
			into.push_back( t.GetByte(i) );
		}
	}
}