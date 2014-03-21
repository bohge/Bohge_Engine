#pragma once
#include "Predefine.h"

#include <pthread.h>



namespace BohgeEngine
{
	class Threading//线程基础类
	{
	public:
		enum ThreadPriority
		{
			TP_LOW = 0,
			TP_NORMAL,
			TP_HIGH,
		};
	private:
		pthread_t		m_pThreadID;
		ThreadPriority	m_ePriority;
		bool			m_isRunning;
	public:
		Threading(void);
		virtual ~Threading(void);
	public:
		static void* ThreadProc( void* param );
	private:
		virtual void _OnBeforeStart() = 0;//启动线程前的事件
		virtual void _OnAfterStart() = 0;//启动线程后的事件
		virtual bool _StopWork() = 0;//停止线程工作，线程准备退出,返回是否成功退出，如果失败将强制退出
	public:
		virtual void* DoWork() = 0;//工作线程
	public:
		void Start();//开启线程
		void Stop();//关闭线程
		void SetPriority( ThreadPriority tp );
	public:
		BOHGE_FORCEINLINE bool isRunning() const
		{
			return m_isRunning;
		}
		BOHGE_FORCEINLINE ThreadPriority GetPriority() const
		{
			return m_ePriority;
		}
	};
}

