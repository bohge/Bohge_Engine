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
#include "JobThread.h"
#include "Framework/IJob.h"

using namespace BohgeEngine;

namespace ThreadSystemPlugin
{
	//------------------------------------------------------------------------------------------------------
	JobThread::JobThread(void)
		:m_isWorking(false),
		m_isLiveing(true)
	{
		m_pMutex = NEW ThreadMutex();
		m_pCondition = NEW ThreadEvent();
		m_pStopEvent = NEW ThreadEvent();
	}
	//------------------------------------------------------------------------------------------------------
	JobThread::~JobThread(void)
	{
		SAFE_DELETE( m_pMutex );
		SAFE_DELETE( m_pCondition );
		SAFE_DELETE( m_pStopEvent );
	}
	//------------------------------------------------------------------------------------------------------
	void JobThread::_OnStart()
	{
		m_pMutex->Lock();//上锁等待线程启动
	}
	//------------------------------------------------------------------------------------------------------
	bool JobThread::_StopWork()
	{
		m_pMutex->Lock();
		m_isLiveing = false;
		if ( m_isWorking )
		{
			m_Job->YieldJob();
			m_isWorking = false;
		}
		else
		{
			//这个地方存在问题，当work线程已经退出道m_pStopEvent->Signal();
			//而由于竞态导致代码走到这里，会导致死锁
			m_pCondition->Signal();
		}
		m_pStopEvent->Wait();
		m_pMutex->Unlock();
		return true;
	}
	//------------------------------------------------------------------------------------------------------
	void JobThread::_SendSignal()
	{
		//是否发送信号启动
		if ( !m_isWorking )
		{
			m_isWorking = true;
			m_pCondition->Signal();
		}
	}
	//------------------------------------------------------------------------------------------------------
	void JobThread::_OnReset()
	{
		m_pMutex->Lock();
		if ( m_isWorking )
		{
			m_isWorking = false;
			m_JobFinishCallBack.ClearConnect();
			m_Job->YieldJob();
		}
		m_pMutex->Unlock();		
	}
	//------------------------------------------------------------------------------------------------------
	void* JobThread::DoWork()
	{
		m_pMutex->Unlock();
		while( m_isLiveing )
		{
			m_pCondition->Wait();
			while( m_isWorking )
			{
				m_Job->DoJob( m_Job );
				m_pMutex->Lock();
				m_Job = SmartPtr<IJob>( NULL );
				if ( m_isLiveing && m_isWorking )
				{
					m_JobFinishCallBack.Unicast( this );
					m_isWorking = NULL != m_Job;
				}
				m_pMutex->Unlock();
			}
		}
		m_pStopEvent->Signal();
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void JobThread::SetJob( BohgeEngine::SmartPtr<BohgeEngine::IJob>& job )
	{
		ASSERT( isRunning() );
		//m_pMutex->Lock();//由于目前只有jobdispatch调用此处，而dis是从dowork中调用，所以把上锁放入dowork中
		m_Job = job;
		_SendSignal();
		//m_pMutex->Unlock();
	}
}