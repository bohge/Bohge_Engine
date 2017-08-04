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
#include "SystemSingleton.h"
#include "FrameworkEvent.h"
#include "TimeCounter.h"
#include "IPlugin.h"


namespace BohgeEngine
{
	class IThreadSpecific;
	class BOHGE_FRAMEWORK ITimeSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR( ITimeSystem );
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( ITimeSystem, IPlugin );


	private:
		IThreadSpecific*		m_pSpecific;
		TimeCounter				m_TimeArray[FrameworkEvent::GC_THREAD_COUNT];
		float					m_TimeRender;
		float					m_TimeLogic;
	public:
		ITimeSystem(void);
		virtual ~ITimeSystem(void);
	private:
		virtual float _Time() = 0;
	public:
		virtual void RegistEvent( Framework& framework ); //注册事件
	public:
		void OnRenderFrameworkEntry( float time, Framework& framework );
		void OnRenderFrameworkBeginRender( float time, Framework& framework );
		void OnRenderFrameworkExit( float time, Framework& framework );
		void OnLogicFrameworkEntry( float time, Framework& framework );
		void OnLogicFrameworkBeginUpdate( float time, Framework& framework );
	public:
		float GetTime();//得到系统启动到现在的时间
		float GetFrameSpan();//得到与上一针的间隔
		float GetFPS();
	};
}