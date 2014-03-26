#pragma once
#include "JobBaseThread.h"


#include <queue>


namespace BohgeEngine
{
	//异步线程队列
	class IJob;
	class QueueThread : public JobBaseThread
	{
	private:
		typedef std::queue< IJob* > JobQueue;
	private:
		JobQueue		m_JobQueue;//数据队列
	public:
		QueueThread(void);
		~QueueThread(void);
	private:
		virtual bool _isEmpty();//数据是否为空
		virtual IJob* _DoPopJob();//弹出一个数据
		virtual void _DoPushJob( IJob* job );//写入一个数据	
	};
}
