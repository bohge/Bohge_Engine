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



//====================================================================================
//
//	具体实施西沟的类，客户端退出后，这个clientcontext需要保存一段时间，防止异步操作报错
//
//====================================================================================
#pragma once
#include "ISocketContext.h"
#include "SmartPtr.hpp"
#include <list>


class ISocketContext;
class Releaser
{
private:
	enum
	{
		MAX_KEEP = 100,
	};
private:
	typedef std::list< SmartPtr<ISocketContext>* > ReleaseList;//
private:
	CRITICAL_SECTION	m_csList;// 用于Worker线程同步的互斥量
	bool				m_isReleasing;//是否正在释放
	ReleaseList			m_ReleaseList;//准备释放的指针
public:
	Releaser()
		:m_isReleasing(false)
	{
		// 初始化线程互斥量
		InitializeCriticalSection(&m_csList);
	}
	~Releaser()
	{
		// 删除客户端列表的互斥量
		DeleteCriticalSection(&m_csList);
		for ( ReleaseList::iterator it = m_ReleaseList.begin();
			it != m_ReleaseList.end();
			it ++ )
		{
			SAFE_DELETE( *it );
		}
		m_ReleaseList.clear();
	}
	inline bool isNeedRelease()
	{
		if( false == m_isReleasing
			&& m_ReleaseList.size() > MAX_KEEP )
		{
			return true;
		}
		return false;
	}
	inline void DoRelease()
	{
		EnterCriticalSection(&m_csList);
		m_isReleasing = true;
		if ( m_ReleaseList.size() > MAX_KEEP )
		{
			const int releCount = MAX_KEEP / 2;
			int i = 0;
			for ( ReleaseList::iterator it = m_ReleaseList.begin();
				it != m_ReleaseList.end();
				it ++ )
			{
				if( i == releCount )
				{
					m_ReleaseList.erase( m_ReleaseList.begin(), it );
					break;
				}
				SAFE_DELETE( *it );
				i++;
			}
		}
		m_isReleasing = false;
		LeaveCriticalSection(&m_csList);
	}
	inline void AddToReleaseList( SmartPtr<ISocketContext>* p)
	{
		EnterCriticalSection(&m_csList);
		m_ReleaseList.push_back( p );
		LeaveCriticalSection(&m_csList);
	}
};