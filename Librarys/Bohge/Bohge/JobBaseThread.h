#pragma once
#include "Threading.h"



namespace BohgeEngine
{
	//异步线程队列
	class IAsynJob;
	class ThreadMutex;
	class ThreadCondition;
	class JobBaseThread : public Threading
	{
	private:
		ThreadMutex*		m_pMutex;
		ThreadCondition*	m_pCondition;
		bool				m_isQuitQueue;
		bool				m_isWorking;
	public:
		JobBaseThread(void);
		virtual ~JobBaseThread(void);
	private:
		void _SendSignal();
	private:
		virtual bool _isEmpty() = 0;//数据是否为空
		virtual IAsynJob* _DoPopJob() = 0;//弹出一个数据
		virtual void _DoPushJob( IAsynJob* job ) = 0;//写入一个数据
	private:
		virtual void _OnBeforeStart();//启动线程前的事件
		virtual void _OnAfterStart();//启动线程后的事件
		virtual bool _StopWork();//停止线程工作，线程准备退出,返回是否成功退出，如果失败将强制退出
	public:
		virtual void* DoWork();//工作线程
	public:
		void PushJob( IAsynJob* job );//写入一个工作
	};
}
