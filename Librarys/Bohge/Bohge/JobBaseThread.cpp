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
			IJob* job = _DoPopJob();
			m_pMutex->Unlock();
			job->DoJob();
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void JobBaseThread::PushJob( IJob* job )
	{
		switch( job->GetJobType() )
		{
		case IJob::JT_SYNCHRONOUS: job->DoJob(); break;
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