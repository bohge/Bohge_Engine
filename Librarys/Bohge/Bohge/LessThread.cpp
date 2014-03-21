#include "LessThread.h"
#include "IAsynJob.h"

namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	LessThread::LessThread(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	LessThread::~LessThread(void)
	{
		m_JobLessMap.clear();
	}
	//------------------------------------------------------------------------------------------------------
	bool LessThread::_isEmpty()
	{
		return m_JobLessMap.empty();
	}
	//------------------------------------------------------------------------------------------------------
	IAsynJob* LessThread::_DoPopJob()
	{
		JobLessMap::iterator it = m_JobLessMap.begin();
		IAsynJob* job = it->second;
		m_JobLessMap.erase( it );
		return job;
	}
	//------------------------------------------------------------------------------------------------------
	void LessThread::_DoPushJob( IAsynJob* job )
	{
		m_JobLessMap.insert( std::make_pair( job->GetPriority(), job ) );
	}




}