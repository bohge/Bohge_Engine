#include "LessThread.h"
#include "IJob.h"

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
	IJob* LessThread::_DoPopJob()
	{
		JobLessMap::iterator it = m_JobLessMap.begin();
		IJob* job = it->second;
		m_JobLessMap.erase( it );
		return job;
	}
	//------------------------------------------------------------------------------------------------------
	void LessThread::_DoPushJob( IJob* job )
	{
		m_JobLessMap.insert( std::make_pair( job->GetPriority(), job ) );
	}




}