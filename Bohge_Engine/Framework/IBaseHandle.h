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
#include "ScriptCallback.h"
#include "IWebProperty.h"
#include "Delegate.hpp"
#include "SmartPtr.hpp"
#include "IPlugin.h"
#include "Object.hpp"


#include <EASTL/string.h>

namespace BohgeEngine
{
	class IJob;
	class Serializer;
	class IJobDispatcher;
	class BOHGE_FRAMEWORK IBaseHandle : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( IBaseHandle )
	private:
		Delegate1< void, IBaseHandle* >			m_OnStateChangeCallBack;
		Delegate1< void, IBaseHandle* >			m_OnFinishCallBack;
		eastl::string							m_Url;
		eastl::string							m_Username;
		eastl::string							m_Password;
		IJobDispatcher*							m_rpJobDispatcher;
		uint									m_nThreadCount;
		uint									m_StatusCode;
		IWebProperty::WebJobState				m_JobState;
		IWebProperty::WebJobType				m_JobType;
	private:
		ScriptCallbackList						m_OnFinishScriptCallback;
	public:
		IBaseHandle( IWebProperty::WebJobType jt, IJobDispatcher* dispatcher );
		virtual ~IBaseHandle(void);
	private:
		virtual void _OnStart() = 0;
		virtual void _OnPause() = 0;
	public:
		virtual void SaveState() = 0;
		virtual void RestroeState( const eastl::string& path ) = 0;
		virtual eastl::string GetJobStateSavePath() = 0;
	protected:
		void _SetStatusCode( uint code );
		void _SaveBaseHandleState( Serializer& writer );
		void _RestroeBaseHandleState( Serializer& reader );
		void _PushJob( const SmartPtr<IJob>& job );
		void _Finish( uint code );//全部任务结束
	public:
		void OnFinishScriptCallback( IBaseHandle* h );
	public:
		void SetStateChangeCallback( Delegate1< void, IBaseHandle* >::FunctionType* callback );
		void SetFinishCallback( Delegate1< void, IBaseHandle* >::FunctionType* callback );
		void SetFinishScriptCallback( IFunctionInvoker* invoker );
		void SetUsername( const eastl::string& name );
		void SetPassword( const eastl::string& pass );
		void SetUrl( const eastl::string& url );
		void SetThreadCount( uint c );
		void Start();
		void Pause();
		void Stop();
	public:
		BOHGE_FORCEINLINE uint GetThreadCount()
		{
			return m_nThreadCount;
		}
		BOHGE_FORCEINLINE const eastl::string& GetUsername()
		{
			return m_Username;
		}
		BOHGE_FORCEINLINE const eastl::string& GetPassword()
		{
			return m_Password;
		}
		BOHGE_FORCEINLINE const eastl::string& GetUrl()
		{
			return m_Url;
		}
		BOHGE_FORCEINLINE IWebProperty::WebJobState GetState()
		{
			return m_JobState;
		}
		BOHGE_FORCEINLINE IWebProperty::WebJobType GetJobType()
		{
			return m_JobType;
		}
		BOHGE_FORCEINLINE uint GetStatusCode()
		{
			return m_StatusCode;
		}
	};
}