#pragma once
#include "JobBaseThread.h"


#include <map>


namespace BohgeEngine
{
	//异步线程队列
	class IAsynJob;
	class GreaterThread : public JobBaseThread
	{
	private:
		typedef std::multimap< int, IAsynJob*, std::greater<int> > JobGreaterMap;
	private:
		JobGreaterMap		m_JobGreaterMap;//数据队列
	public:
		GreaterThread(void);
		~GreaterThread(void);
	private:
		virtual bool _isEmpty();//数据是否为空
		virtual IAsynJob* _DoPopJob();//弹出一个数据
		virtual void _DoPushJob( IAsynJob* job );//写入一个数据	
	};
}
