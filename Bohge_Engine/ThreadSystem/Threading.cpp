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
#include "Threading.h"


#include <pthread.h>

using namespace BohgeEngine;
namespace ThreadSystemPlugin
{
	//------------------------------------------------------------------------------------------------------
	Threading::Threading(void)
		:m_isRunning(false),
		m_ePriority(ThreadProperty::TP_NORMAL)
	{
		m_pThreadID = NEW pthread_t;
		m_pThreadAttr = NEW pthread_attr_t;
		pthread_attr_init( static_cast<pthread_attr_t*>(m_pThreadAttr) );
	}
	//------------------------------------------------------------------------------------------------------
	Threading::~Threading(void)
	{
		pthread_attr_destroy( static_cast<pthread_attr_t*>(m_pThreadAttr) );
		delete( static_cast<pthread_attr_t*>( m_pThreadAttr ) );
		delete static_cast<pthread_t*>( m_pThreadID );
	}
	//------------------------------------------------------------------------------------------------------
	void* Threading::ThreadProc( void* param )
	{
		Threading* ptr = static_cast<Threading*>( param );
		return ptr->DoWork();
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::Start()
	{
		//ASSERT( false == m_isRunning );		
		if (!m_isRunning)//可能是reset之后的线程，这里不需要初始化
		{//初始化线程参数
			m_isRunning = true;
			pthread_attr_setschedpolicy( static_cast<pthread_attr_t*>(m_pThreadAttr), SCHED_FIFO );
			int policy = -1;
			pthread_attr_getschedpolicy( static_cast<pthread_attr_t*>(m_pThreadAttr), & policy);
			int max = sched_get_priority_max( policy );
			int min = sched_get_priority_min( policy );
			int half = min + ( max - min ) / 2;
			int pri = 0;
			switch( m_ePriority )
			{
			case ThreadProperty::TP_LOW: pri = min; break;
			case ThreadProperty::TP_NORMAL: pri = half; break;
			case ThreadProperty::TP_HIGH: pri = max; break;
			default:ASSERT(false);
			}
			sched_param sched;
			sched.sched_priority = pri;
			pthread_attr_setscope( static_cast<pthread_attr_t*>(m_pThreadAttr), PTHREAD_SCOPE_SYSTEM );
			pthread_attr_setschedparam( static_cast<pthread_attr_t*>(m_pThreadAttr), &sched );
			pthread_create( static_cast<pthread_t*>(m_pThreadID), static_cast<pthread_attr_t*>(m_pThreadAttr), ThreadProc, (void*)this );
		}
		_OnStart();
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::Reset()
	{
		ASSERT( m_isRunning );
		_OnReset();
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::Stop()
	{
		ASSERT( m_isRunning );
		_StopWork( );
		//if( !_StopWork( ) )
		{
			void* ptr = 0;
			pthread_join( *static_cast<pthread_t*>(m_pThreadID), &ptr );
			if ( NULL != ptr )
			{
				pthread_exit( ptr );
			}
		}
		m_isRunning = false;
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::SetPriority( ThreadProperty::ThreadPriority tp )
	{
		m_ePriority = tp;
	}
}