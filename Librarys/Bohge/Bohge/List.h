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
//		链表	   //
/////////////////////

#pragma once


#include "Predefine.h"
#include "VariableType.h"

namespace BohgeEngine
{
	template<typename T>
	class List
	{
	private:
		class sList
		{
		public:
			T			m_pData;
			sList*		m_pPrevious;
			sList*		m_pNext;
		public:
			sList()
				:m_pPrevious(NULL),
				m_pNext(NULL)
			{
			}
		};
	public:
		class iterator//迭代器
		{
			friend class List;
		private:
			sList*		m_pPointer;
		private:
			iterator(sList* input)
			{
				m_pPointer = input;
			}
		public:
			iterator()
				:m_pPointer(NULL)
			{
			}
			iterator(const iterator& input)
			{
				m_pPointer = input.m_pPointer;
			}
			BOHGE_FORCEINLINE void operator = (const iterator& input)
			{
				m_pPointer = input.m_pPointer;
			}
			BOHGE_FORCEINLINE bool operator != (const iterator& input)
			{
				return m_pPointer != input.m_pPointer ;
			}
			BOHGE_FORCEINLINE bool operator == (const iterator& input)
			{
				return m_pPointer == input.m_pPointer ;
			}
			BOHGE_FORCEINLINE T& operator*() const
			{
				return m_pPointer->m_pData;
			}
			BOHGE_FORCEINLINE iterator& operator++(int)
			{
				m_pPointer = m_pPointer->m_pNext;
				return *this;
			}
		};
	private:
		uint			m_uSize;//当前存入的数量
		sList*			m_pFirst;
		sList*			m_pLast;
	public:
		List()
			:m_uSize(0),
			m_pFirst(NULL),
			m_pLast(NULL)
		{
		}
		~List()
		{
			clear();
		}
		bool empty()
		{
			return 0 == m_uSize;
		}
		void clear()
		{
			for( sList* temp = m_pFirst; temp != NULL ;  )
			{
				sList* toDelet = temp;
				temp = temp->m_pNext;
				SAFE_DELETE(toDelet);
			}
			m_pFirst = NULL ; 
			m_pLast = NULL;
		}
		void push_back(T data)
		{
			if( NULL == m_pFirst )
			{
				m_pFirst = NEW sList();
				m_pLast = m_pFirst;
			}
			else
			{
				m_pLast->m_pNext = NEW sList();
				m_pLast->m_pNext->m_pPrevious = m_pLast;
				m_pLast = m_pLast->m_pNext;
			}
			m_pLast->m_pData = data;
			m_uSize++;
		}
		void push_front(T data)
		{
			if( NULL == m_pFirst )
			{
				m_pFirst = NEW sList();
				m_pLast = m_pFirst;
			}
			else
			{
				sList* temp = m_pFirst;
				m_pFirst = NEW sList();
				m_pFirst->m_pNext = temp;
				temp->m_pPrevious = m_pFirst;
			}
			m_pFirst->m_pData = data;
			m_uSize++;
		}
		void pop_back()
		{
			sList* temp = m_pLast;
			m_pLast = m_pLast->m_pPrevious;
			m_pLast->m_pNext = NULL;
			SAFE_DELETE( temp );
		}
		iterator erase(iterator& input)
		{
			ASSERT( NULL != input.m_pPointer );
			sList* toDelete = input.m_pPointer;
			if( input.m_pPointer != m_pFirst )
			{
				toDelete->m_pPrevious->m_pNext = toDelete->m_pNext;
			}
			else
			{
				m_pFirst = m_pFirst->m_pNext;
			}
			if( input.m_pPointer != m_pLast )
			{
				toDelete->m_pNext->m_pPrevious = toDelete->m_pPrevious;
			}
			else
			{
				m_pLast = m_pLast->m_pPrevious;
			}
			SAFE_DELETE(toDelete);
			m_uSize--;
			return input;
		}
		iterator begin() const
		{
			return iterator(m_pFirst);
		}
		iterator end() const
		{
			return iterator(NULL);
		}
		uint size()
		{
			return m_uSize;
		}
	};



#include <list>
	class Rect2D;
	typedef std::list<Rect2D*>	Rect2DPointList;//2D可绘制矩形链表

	class ShapeBase;
	//typedef List<ShapeBase*>	ShapePointList;
	typedef std::list<ShapeBase*>	ShapePointList;

}