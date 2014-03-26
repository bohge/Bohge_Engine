#include "GreaterThread.h"
#include "IJob.h"

namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	GreaterThread::GreaterThread(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	GreaterThread::~GreaterThread(void)
	{
		m_JobGreaterMap.clear();
	}
	//------------------------------------------------------------------------------------------------------
	bool GreaterThread::_isEmpty()
	{
		return m_JobGreaterMap.empty();
	}
	//------------------------------------------------------------------------------------------------------
	IJob* GreaterThread::_DoPopJob()
	{
		JobGreaterMap::iterator it = m_JobGreaterMap.begin();
		IJob* job = it->second;
		m_JobGreaterMap.erase( it );
		return job;
	}
	//------------------------------------------------------------------------------------------------------
	void GreaterThread::_DoPushJob( IJob* job )
	{
		m_JobGreaterMap.insert( std::make_pair( job->GetPriority(), job ) );
	}




}