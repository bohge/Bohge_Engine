#pragma once
#include "JobBaseThread.h"


#include <queue>


namespace BohgeEngine
{
	//异步线程队列
	class IAsynJob;
	class QueueThread : public JobBaseThread
	{
	private:
		typedef std::queue< IAsynJob* > JobQueue;
	private:
		JobQueue		m_JobQueue;//数据队列
	public:
		QueueThread(void);
		~QueueThread(void);
	private:
		virtual bool _isEmpty();//数据是否为空
		virtual IAsynJob* _DoPopJob();//弹出一个数据
		virtual void _DoPushJob( IAsynJob* job );//写入一个数据	
	};
}
