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
#include "Framework/3DMath.h"
#include "ThreadEvent.h"


#include <pthread.h>

using namespace BohgeEngine;
namespace ThreadSystemPlugin
{
	//------------------------------------------------------------------------------------------------------
	ThreadEvent::ThreadEvent(void)
	{
		m_pCondition = NEW pthread_cond_t;
		m_pMutex = NEW pthread_mutex_t;
		pthread_cond_init( static_cast<pthread_cond_t*>(m_pCondition), NULL );
		pthread_mutex_init( static_cast<pthread_mutex_t*>(m_pMutex), NULL );

		//上锁，直到wait自动打开
		pthread_mutex_lock( static_cast<pthread_mutex_t*>(m_pMutex) );
	}
	//------------------------------------------------------------------------------------------------------
	ThreadEvent::~ThreadEvent(void)
	{
		pthread_cond_destroy( static_cast<pthread_cond_t*>(m_pCondition) );
		pthread_mutex_destroy( static_cast<pthread_mutex_t*>(m_pMutex) );
		delete static_cast<pthread_cond_t*>(m_pCondition);
		delete static_cast<pthread_mutex_t*>(m_pMutex);
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadEvent::Wait()
	{
		//wait函数在等待时会把mutex打开，当等待完成后给mutex上锁
		pthread_cond_wait( static_cast<pthread_cond_t*>(m_pCondition), static_cast<pthread_mutex_t*>(m_pMutex) );
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadEvent::Signal()
	{
		pthread_mutex_lock( static_cast<pthread_mutex_t*>(m_pMutex) );
		pthread_cond_signal( static_cast<pthread_cond_t*>(m_pCondition) );
		pthread_mutex_unlock( static_cast<pthread_mutex_t*>(m_pMutex) );
	}
}