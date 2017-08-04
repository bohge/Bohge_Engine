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
#include "IThreadSpecific.h"
#include "IThreadSystem.h"
#include "ITimeSystem.h"
#include "Framework.h"
#include "Log.h"


namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( ITimeSystem );
	SYSTEM_SINGLETON_IMPLEMENT( ITimeSystem );
	//---------------------------------------------------------------------------------------------------------
	ITimeSystem::ITimeSystem( void )
		:m_pSpecific(NULL),
		m_TimeRender(0.0f),
		m_TimeLogic(0.0f)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	ITimeSystem::~ITimeSystem( void )
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void ITimeSystem::RegistEvent( BohgeEngine::Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_ENTRY, MEMBER_FUNC_PTR( &ITimeSystem::OnRenderFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_BEGIN_RENDER, MEMBER_FUNC_PTR( &ITimeSystem::OnRenderFrameworkBeginRender ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_EXIT, MEMBER_FUNC_PTR( &ITimeSystem::OnRenderFrameworkExit ) );

		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &ITimeSystem::OnLogicFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_BEGIN_UPDATE, MEMBER_FUNC_PTR( &ITimeSystem::OnLogicFrameworkBeginUpdate ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void ITimeSystem::OnRenderFrameworkEntry( float time, Framework& framework )
	{
		m_pSpecific =
			static_cast< IThreadSpecific* >
			( IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_THREAD_SPECIFIC ) );
		m_pSpecific->Initialize();

		m_pSpecific->SetSpecific( &m_TimeArray[FrameworkEvent::GC_RENDER_THREAD_INDEX] );
		m_TimeArray[FrameworkEvent::GC_RENDER_THREAD_INDEX].SetTime( _Time() );
	}
	//---------------------------------------------------------------------------------------------------------
	void ITimeSystem::OnRenderFrameworkBeginRender( float time, Framework& framework )
	{
		m_TimeArray[FrameworkEvent::GC_RENDER_THREAD_INDEX].SetTime( _Time() );
		m_TimeRender += m_TimeArray[FrameworkEvent::GC_RENDER_THREAD_INDEX].GetFrameSpan();
		if ( m_TimeRender >= 5.0f )
		{
			m_TimeRender = 0.0f;
			DEBUGLOG("Render FPS %f\n",m_TimeArray[FrameworkEvent::GC_RENDER_THREAD_INDEX].GetFPS() );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ITimeSystem::OnRenderFrameworkExit( float time, Framework& framework )
	{
		IThreadSystem::Instance()->RecycleBin( m_pSpecific );
	}
	//---------------------------------------------------------------------------------------------------------
	void ITimeSystem::OnLogicFrameworkEntry( float time, Framework& framework )
	{
		m_pSpecific->SetSpecific( &m_TimeArray[FrameworkEvent::GC_LOGIC_THREAD_INDEX] );
		m_TimeArray[FrameworkEvent::GC_LOGIC_THREAD_INDEX].SetTime( _Time() );
	}
	//---------------------------------------------------------------------------------------------------------
	void ITimeSystem::OnLogicFrameworkBeginUpdate( float time, Framework& framework )
	{
		m_TimeArray[FrameworkEvent::GC_LOGIC_THREAD_INDEX].SetTime( _Time() );
		m_TimeLogic += m_TimeArray[FrameworkEvent::GC_LOGIC_THREAD_INDEX].GetFrameSpan();
		if ( m_TimeLogic >= 5.0f )
		{
			m_TimeLogic = 0.0f;
			DEBUGLOG("Logic FPS %f\n",m_TimeArray[FrameworkEvent::GC_LOGIC_THREAD_INDEX].GetFPS() );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	float ITimeSystem::GetTime()
	{
		return m_pSpecific->GetSpecific<TimeCounter>()->GetTime();
	}
	//---------------------------------------------------------------------------------------------------------
	float ITimeSystem::GetFrameSpan()
	{
		return m_pSpecific->GetSpecific<TimeCounter>()->GetFrameSpan();
	}
	//---------------------------------------------------------------------------------------------------------
	float ITimeSystem::GetFPS()
	{
		return m_pSpecific->GetSpecific<TimeCounter>()->GetFPS();
	}

}