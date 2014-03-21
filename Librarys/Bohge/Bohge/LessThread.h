#pragma once
#include "JobBaseThread.h"


#include <map>


namespace BohgeEngine
{
	//异步线程队列
	class IAsynJob;
	class LessThread : public JobBaseThread
	{
	private:
		typedef std::multimap< int, IAsynJob* > JobLessMap;
	private:
		JobLessMap		m_JobLessMap;//数据队列
	public:
		LessThread(void);
		~LessThread(void);
	private:
		virtual bool _isEmpty();//数据是否为空
		virtual IAsynJob* _DoPopJob();//弹出一个数据
		virtual void _DoPushJob( IAsynJob* job );//写入一个数据	
	};
}
