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
#include "ITimeSystem.h"
#include "LogicJob.h"



namespace BohgeEngine
{
	//---------------------------------------------------------------------------------------------------------
	LogicJob::LogicJob(void)
		:m_isLiveing( false ),		
		m_rpFramework(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LogicJob::~LogicJob(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void LogicJob::_DoJob( SmartPtr<IJob>& self )
	{
		m_LogicThreadDelegateArray[FrameworkEvent::ET_LOGIC_ON_ENTRY].Multicast( 0.0f, *m_rpFramework );
		m_isLiveing = true;
		while( m_isLiveing )
		{
			float timespan = ITimeSystem::Instance()->GetFrameSpan();
			for ( int i = FrameworkEvent::ET_LOGIC_BEGIN_UPDATE;
				i <= FrameworkEvent::ET_LOGIC_END_UPDATE;
				i ++ )
			{
				m_LogicThreadDelegateArray[i].Multicast( timespan, *m_rpFramework );
			}
		}
		m_LogicThreadDelegateArray[FrameworkEvent::ET_LOGIC_ON_EXIT].RMulticast( 0.0f, *m_rpFramework );
	}
	//---------------------------------------------------------------------------------------------------------
	void LogicJob::_DoYieldJob()
	{
		m_isLiveing = false;
	}
	//---------------------------------------------------------------------------------------------------------
	void LogicJob::RegisterEventListener( FrameworkEvent::EventType et, FrameworkDelegate::FunctionType* func )
	{
		m_LogicThreadDelegateArray[ et ].Connect( func );
	}
	//---------------------------------------------------------------------------------------------------------
	void LogicJob::OnLanuch( Framework* framework )
	{
		m_rpFramework = framework;
	}
	//---------------------------------------------------------------------------------------------------------
	void LogicJob::OnExit()
	{
		for ( int i = FrameworkEvent::ET_LOGIC_ON_ENTRY;
			i < FrameworkEvent::GC_LOGIC_DELEGATE_COUNT;
			i ++ )
		{
			m_LogicThreadDelegateArray[i].ClearConnect();
		}
	}
}