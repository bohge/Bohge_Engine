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
#include "IJobDispatcher.h"
#include "Serializer.h"
#include "IBaseHandle.h"


using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT(IBaseHandle)
	//------------------------------------------------------------------------------
	IBaseHandle::IBaseHandle( IWebProperty::WebJobType jt, IJobDispatcher* dispatcher )
		:m_rpJobDispatcher(dispatcher),
		m_nThreadCount(1),
		m_JobType( jt ),
		m_JobState( IWebProperty::JS_INACTIVE ),
		m_StatusCode(0)
	{
		SetFinishCallback( MEMBER_FUNC_PTR(&IBaseHandle::OnFinishScriptCallback) );
	}
	//------------------------------------------------------------------------------
	IBaseHandle::~IBaseHandle( void )
	{
		Stop();
		ASSERT( IWebProperty::JS_INACTIVE == m_JobState );
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::SetStateChangeCallback( Delegate1< void, IBaseHandle* >::FunctionType* callback )
	{
		m_OnStateChangeCallBack.Connect( callback );
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::SetFinishCallback( Delegate1< void, IBaseHandle* >::FunctionType* callback )
	{
		m_OnFinishCallBack.Connect( callback );
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::SetUrl( const eastl::string& url )
	{
		m_Url = url;
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::SetUsername( const eastl::string& name )
	{
		m_Username = name;
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::SetPassword( const eastl::string& pass )
	{
		m_Password = pass;
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::SetThreadCount( uint c )
	{
		m_nThreadCount = c > IWebProperty::WC_MAX_THREAD_COUNT ? IWebProperty::WC_MAX_THREAD_COUNT : c;
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::_PushJob( const SmartPtr<IJob>& job )
	{
		m_rpJobDispatcher->PushJob( job );
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::_SetStatusCode( uint code )
	{
		m_StatusCode = code;
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::_Finish( uint code )
	{
		_SetStatusCode( code );
		m_JobState = IWebProperty::JS_FINISH;
		m_OnStateChangeCallBack.Multicast( this );
		m_OnFinishCallBack.Multicast( this );
		//m_pJobDispatcher->Stop();不能在这里停止应为可能会在dowork中调用这个函数
		//IThreadSystem::Instance()->RecycleBin( m_pJobDispatcher );
		//m_pJobDispatcher = NULL;
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::_SaveBaseHandleState( Serializer& writer )
	{
		writer<<m_nThreadCount
			<<m_Url
			<<m_Username[0]
			<<m_Password[0];
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::_RestroeBaseHandleState( Serializer& reader )
	{
		reader>>m_nThreadCount
			>>m_Url
			>>m_Username[0]
			>>m_Password[0];
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::Start()
	{
		if ( IWebProperty::JS_INACTIVE == m_JobState )
		{
			m_JobState = IWebProperty::JS_ACTIVE;
			m_OnStateChangeCallBack.Multicast( this );
			_OnStart();
		}
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::Pause()
	{
		if ( IWebProperty::JS_ACTIVE == m_JobState )
		{
			m_JobState = IWebProperty::JS_INACTIVE;
			m_OnStateChangeCallBack.Multicast( this );
			_OnPause();
		}
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::Stop()
	{
		if ( IWebProperty::JS_INACTIVE != m_JobState)
		{
			m_JobState = IWebProperty::JS_INACTIVE;
		}
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::OnFinishScriptCallback( IBaseHandle* h )
	{
		FOR_EACH( ScriptCallbackList, invoker, m_OnFinishScriptCallback )
		{
			(*invoker)->Invoke( h );
		}
	}
	//------------------------------------------------------------------------------
	void IBaseHandle::SetFinishScriptCallback( IFunctionInvoker* invoker )
	{
		m_OnFinishScriptCallback.push_back( invoker );
	}

}