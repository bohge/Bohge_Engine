#include "JobBaseThread.h"
#include "ThreadMutex.h"
#include "ThreadRWLock.h"
#include "ThreadCondition.h"
#include "IAsynJob.h"


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
			if ( _isEmpty() )
			{
				m_isWorking = false;
				m_pCondition->Wait();
			}
			m_isWorking = true;
			if ( m_isQuitQueue )
			{
				m_isQuitQueue = false;
				m_isWorking = false;
				return NULL;
			}
			m_pMutex->Lock();
			IAsynJob* job = _DoPopJob();
			m_pMutex->Unlock();
			job->AsyncDoJob();
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void JobBaseThread::PushJob( IAsynJob* job )
	{
		ASSERT( isRunning() );
		m_pMutex->Lock();
		_DoPushJob( job );
		_SendSignal();
		m_pMutex->Unlock();
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