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
#include "Framework/SmartPtr.hpp"
#include "Framework/Delegate.hpp"
#include "Threading.h"
#include "ThreadMutex.h"
#include "ThreadEvent.h"



namespace BohgeEngine{ class IJob; }
namespace ThreadSystemPlugin
{
	//异步线程队列
	class JobThread : public Threading//这个队列不支持移除工作
	{
	private:
		BohgeEngine::SmartPtr<BohgeEngine::IJob>		m_Job;
		BohgeEngine::Unidelegate1<void,JobThread*>		m_JobFinishCallBack;
		ThreadMutex*									m_pMutex;
		ThreadEvent*									m_pCondition;
		ThreadEvent*									m_pStopEvent;
		volatile bool									m_isLiveing;
		volatile bool									m_isWorking;
	public:
		JobThread(void);
		~JobThread(void);
	private:
		virtual void _OnReset();
		virtual void _OnStart();
		virtual bool _StopWork();
	public:
		virtual void* DoWork();
	private:
		void _SendSignal();
	public:
		void SetJob( BohgeEngine::SmartPtr<BohgeEngine::IJob>& job );
	public:
		BOHGE_FORCEINLINE void SetCallBack( BohgeEngine::Unidelegate1<void,JobThread*>::FunctionType* fptr )
		{
			m_JobFinishCallBack.Connect( fptr );
		}
	};
}
