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



/////////////////////
//		数组类	   //
/////////////////////
#pragma once

#include <string>

#include "Predefine.h"
#include "VariableType.h"


namespace BohgeEngine
{
	template<typename T>
	class Vector
	{
	private:
		uint		m_nCount;//全部数据的个数
		uint		m_nCurrentIndex;
		T*			m_data;	//data数组指针
	public:
		Vector()
			:m_data(NULL),
			m_nCount(1000),
			m_nCurrentIndex(0)
		{
			m_data = NEW T[m_nCount];
			memset(m_data, 0, m_nCount);
			ASSERT(m_data!=NULL);
		}
		Vector(const uint count)
			:m_data(NULL),
			m_nCount(count),
			m_nCurrentIndex(0)
		{
			m_data = NEW T[m_nCount];
			memset(m_data, 0, m_nCount);
			ASSERT(m_data!=NULL);
		}
		Vector( const Vector& input )
			:m_nCount(input.m_nCount),
			m_nCurrentIndex(input.m_nCurrentIndex)
		{
			m_data = NEW T[m_nCount];
			for( uint i =0 ; i < m_nCurrentIndex ; i ++ )
			{
				m_data[i] = input.m_data[i];
			}
		}
		~Vector()
		{
			SAFE_DELETE_ARRAY(m_data);
		}
		BOHGE_FORCEINLINE void push_back(const T& input)
		{
			m_data[m_nCurrentIndex] = input;
			m_nCurrentIndex++;
		}
		BOHGE_FORCEINLINE T* GetDataArray() const
		{
			return m_data;
		}
		BOHGE_FORCEINLINE bool empty()
		{
			return 0 == m_nCurrentIndex;
		}
		BOHGE_FORCEINLINE void clear()
		{
			m_nCurrentIndex = 0;
		}
		//得到当前以添加的数据个数
		BOHGE_FORCEINLINE uint size()
		{
			return m_nCurrentIndex;
		}
		BOHGE_FORCEINLINE T operator[] (uint index)
		{
			return m_data[index];
		}
		BOHGE_FORCEINLINE uint BitSize()
		{
			return sizeof(T) * m_nCurrentIndex;
		}
		BOHGE_FORCEINLINE uint MaxBitSize()
		{
			return sizeof(T) * m_nCount;
		}
	};
}