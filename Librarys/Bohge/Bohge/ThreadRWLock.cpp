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

#include "ThreadRWLock.h"
#include <pthread.h>


namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	ThreadRWLock::ThreadRWLock(void)
	{
		m_pRWLock = NEW pthread_rwlock_t;
		pthread_rwlock_init( static_cast<pthread_rwlock_t*>(m_pRWLock), NULL );
	}
	//------------------------------------------------------------------------------------------------------
	ThreadRWLock::~ThreadRWLock(void)
	{
		pthread_rwlock_destroy( static_cast<pthread_rwlock_t*>(m_pRWLock) );
		SAFE_DELETE( m_pRWLock );
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadRWLock::ReadLock()//共享方式的读取锁
	{
		pthread_rwlock_rdlock( static_cast<pthread_rwlock_t*>(m_pRWLock) );
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadRWLock::WriteLock()//独占的写入锁
	{
		pthread_rwlock_wrlock( static_cast<pthread_rwlock_t*>(m_pRWLock) );
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadRWLock::Unlock()//解锁
	{
		pthread_rwlock_unlock( static_cast<pthread_rwlock_t*>(m_pRWLock) );
	}
}