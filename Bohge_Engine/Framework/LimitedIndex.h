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


namespace BohgeEngine
{
	class LimitedIndex
	{
	private:
		int			m_nMax;
		int			m_nIndex;
	public:
		LimitedIndex( int max )
			:m_nMax(max),
			m_nIndex(0)
		{
		}
		LimitedIndex()
			:m_nMax(0),m_nIndex(0)
		{
		}
		BOHGE_FORCEINLINE void SetCurrent(int c)
		{
			m_nIndex = c;
			if( m_nIndex >= m_nMax )
			{
				m_nIndex = m_nIndex - m_nMax;
			}
		}
		BOHGE_FORCEINLINE void SetMax(int max)
		{
			m_nMax = max;
		}
		BOHGE_FORCEINLINE int operator += ( int rhs )
		{
			m_nIndex += rhs;
			if( m_nIndex >= m_nMax )
			{
				m_nIndex = 0;
			}
			return m_nIndex;
		}
		BOHGE_FORCEINLINE int operator++ (int)
		{
			++ m_nIndex;
			if( m_nIndex >= m_nMax )
			{
				m_nIndex = 0;
			}
			return m_nIndex;
		}
		BOHGE_FORCEINLINE int Max()
		{
			return m_nMax;
		}
		BOHGE_FORCEINLINE operator int()
		{
			return m_nIndex;
		}
	};
}