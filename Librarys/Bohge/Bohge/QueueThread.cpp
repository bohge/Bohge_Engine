#include "QueueThread.h"



namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	QueueThread::QueueThread(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	QueueThread::~QueueThread(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	bool QueueThread::_isEmpty()
	{
		return m_JobQueue.empty();
	}
	//------------------------------------------------------------------------------------------------------
	IAsynJob* QueueThread::_DoPopJob()
	{
		IAsynJob* job = m_JobQueue.front();
		m_JobQueue.pop();
		return job;
	}
	//------------------------------------------------------------------------------------------------------
	void QueueThread::_DoPushJob( IAsynJob* job )
	{
		m_JobQueue.push( job );
	}
}
