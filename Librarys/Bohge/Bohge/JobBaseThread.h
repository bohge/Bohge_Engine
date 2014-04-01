//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Threading.h"



namespace BohgeEngine
{
	//异步线程队列
	class IJob;
	class ThreadMutex;
	class ThreadCondition;
	class JobBaseThread : public Threading//这个队列不支持移除工作
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
		virtual IJob* _DoPopJob() = 0;//弹出一个数据
		virtual void _DoPushJob( IJob* job ) = 0;//写入一个数据
	private:
		virtual void _OnBeforeStart();//启动线程前的事件
		virtual void _OnAfterStart();//启动线程后的事件
		virtual bool _StopWork();//停止线程工作，线程准备退出,返回是否成功退出，如果失败将强制退出
	public:
		virtual void* DoWork();//工作线程
	public:
		void PushJob( IJob* job );//写入一个工作
	};
}
