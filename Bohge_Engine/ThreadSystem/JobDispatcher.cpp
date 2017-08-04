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
#include "GreaterContainer.hpp"
#include "LessContaniner.hpp"
#include "QueueContainer.hpp"
#include "Framework/IJob.h"
#include "Framework/Log.h"
#include "JobDispatcher.h"
#include "IJobContainer.h"
#include "JobThreadPool.h"
#include "ThreadMutex.h"
#include "JobThread.h"




using namespace BohgeEngine;

namespace ThreadSystemPlugin
{
	//---------------------------------------------------------------------------------------------------------
	JobDispatcher::JobDispatcher(void)
		:m_pJobContainer(NULL),
		m_pMutex( NEW ThreadMutex )
	{
	}
	//---------------------------------------------------------------------------------------------------------
	JobDispatcher::~JobDispatcher(void)
	{
		ASSERT( 0 == m_ThreadList.size() );//必须停止
		SAFE_DELETE( m_pMutex );
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::Start()
	{
		ASSERT( NULL == m_pJobContainer );
		switch( GetSortType() )
		{
		case IJobDispatcher::ST_UNSORT: m_pJobContainer = NEW QueueContainer; break;
		case IJobDispatcher::ST_GREATER: m_pJobContainer = NEW GreaterContainer; break;
		case IJobDispatcher::ST_LESS: m_pJobContainer = NEW LessContaniner; break;
		default: ASSERT( false ); break;
		}
		for ( int i = 0; i < GetThreadCount(); ++ i )
		{
			JobThread* jt = JobThreadPool::Instance()->GetJobThread();
			jt->SetCallBack( MEMBER_FUNC_PTR( &JobDispatcher::OnThreadWorkDone ) );
			jt->Start();
			m_ThreadList.push_back( jt );
			m_IdleThreads.push_back( jt );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::Stop()
	{
		//m_pMutex->Lock();
		m_pJobContainer->Clear();
		FOR_EACH( ThreadList, it, m_ThreadList )
		{
			(*it)->Reset();
			JobThreadPool::Instance()->ReleaseJobThread( *it );
			//(*it)->Stop();
			//SAFE_DELETE( *it );
		}
		SAFE_DELETE( m_pJobContainer );
		m_ThreadList.clear();
		m_IdleThreads.clear();
		//m_pMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::PushJob( SmartPtr<IJob> job )
	{
		switch( job->GetJobType() )
		{
		case IJob::JT_SYNCHRONOUS: job->DoJob( job ); break;
		case IJob::JT_ASYNCHRONOUS: _TryDispatchJob( job ); break;
		default: ASSERT(false);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::_TryDispatchJob( SmartPtr<IJob>& job )//尝试去分发工作
	{
		m_pMutex->Lock();
		if ( m_IdleThreads.empty() )//如果没有空闲线程
		{
			m_pJobContainer->PushJob( job );
		}
		else
		{
			m_IdleThreads.front()->SetJob( job );
			m_IdleThreads.pop_front();
		}
		m_pMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::OnThreadWorkDone( JobThread* thread )
	{
		m_pMutex->Lock();
		if ( !m_pJobContainer->isEmpty() )
		{
			thread->SetJob( m_pJobContainer->PopJob() );
		}
		else
		{
			m_IdleThreads.push_back( thread );
		}
		m_pMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::Pasue()
	{
		DEBUGLOG("Empty JobDispatcher Pasue !\n");
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::Continue()
	{
		DEBUGLOG("Empty JobDispatcher Continue !\n");
	}

}