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

#include "JobBaseThread.h"
#include "ThreadMutex.h"
#include "ThreadRWLock.h"
#include "ThreadCondition.h"
#include "IJob.h"


namespace BohgeEngine
{
	JobBaseThread::JobBaseThread(void)
		:m_isWorking(false),
		m_isQuitQueue(false)
	{
		m_pMutex = NEW ThreadMutex();
		m_pCondition = NEW ThreadCondition();

	}
	//------------------------------------------------------------------------------------------------------
	JobBaseThread::~JobBaseThread(void)
	{
		SAFE_DELETE( m_pMutex );
		SAFE_DELETE( m_pCondition );
	}
	//------------------------------------------------------------------------------------------------------
	void JobBaseThread::_OnBeforeStart()
	{
		m_pMutex->Lock();//上锁等待线程启动
	}
	//------------------------------------------------------------------------------------------------------
	void JobBaseThread::_OnAfterStart()
	{
		m_pMutex->Lock();
		if ( !_isEmpty() )
		{
			_SendSignal();
		}
		m_pMutex->Unlock();
	}
	//------------------------------------------------------------------------------------------------------
	bool JobBaseThread::_StopWork()
	{
		m_pMutex->Lock();
		m_isQuitQueue = true;
		_SendSignal();
		m_pMutex->Unlock();
		return true;
	}
	//------------------------------------------------------------------------------------------------------
	void* JobBaseThread::DoWork()
	{
		m_pMutex->Unlock();
		while( true )
		{
			m_pMutex->Lock();
			bool isContinue = _isEmpty() ? false : true;
			if ( isContinue )
			{
				m_isWorking = true;
				m_pMutex->Unlock();
			}
			else
			{
				m_isWorking = false;
				m_pMutex->Unlock();
				m_pCondition->Wait();
			}
			if ( m_isQuitQueue )
			{
				m_isQuitQueue = false;
				m_isWorking = false;
				return NULL;
			}
			m_pMutex->Lock();
			SmartPtr<IJob> job = _DoPopJob();
			m_pMutex->Unlock();
			job->DoJob( job );
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void JobBaseThread::PushJob( SmartPtr<IJob>& job )
	{
		switch( job->GetJobType() )
		{
		case IJob::JT_SYNCHRONOUS: job->DoJob( job ); break;
		case IJob::JT_ASYNCHRONOUS:
			{
				ASSERT( isRunning() );
				m_pMutex->Lock();
				_DoPushJob( job );
				_SendSignal();
				m_pMutex->Unlock();
			}break;
		default: ASSERT(false);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void JobBaseThread::_SendSignal()
	{
		//是否发送信号启动
		if ( !m_isWorking )
		{
			m_pCondition->Signal();
		}
	}

}