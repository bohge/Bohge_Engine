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
#include "Framework/IJob.h"


#include <EASTL/string.h>

namespace CurlWebSystemPlugin
{
	class ICurlJob : public BohgeEngine::IJob
	{
	private:
		eastl::string	m_Url;
		eastl::string	m_Username;
		eastl::string	m_Password;
		void*			m_pCurl;
		uint			m_StatusCode;
		bool			m_isLiving;
		BohgeEngine::Delegate1<void,BohgeEngine::SmartPtr<BohgeEngine::IJob>&>		m_JobDoneCallback;
	public:
		ICurlJob(void);
		virtual ~ICurlJob(void);
	private:
		virtual void _DoWebJob( BohgeEngine::SmartPtr<BohgeEngine::IJob>& self ) = 0;
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob( BohgeEngine::SmartPtr<BohgeEngine::IJob>& self );
	private:
		static	int ProgressCallback(void *bar,double t,double d, double ultotal,double ulnow);
	public:
		virtual void Connect();
	public:
		void SetCallback( BohgeEngine::Delegate1<void,BohgeEngine::SmartPtr<BohgeEngine::IJob>&>::FunctionType* callback );
		void SetUrl( const eastl::string& url );
	protected:
		BOHGE_FORCEINLINE void* _GetCURL()
		{
			return m_pCurl;
		}
		BOHGE_FORCEINLINE const eastl::string& _GetUrl()
		{
			return m_Url;

		}
		BOHGE_FORCEINLINE const eastl::string& _GetUsername()
		{
			return m_Username;

		}
		BOHGE_FORCEINLINE const eastl::string& _GetPassword()
		{
			return m_Password;

		}
	public:
		BOHGE_FORCEINLINE void SetUsername( const eastl::string& name )
		{
			m_Username = name;
		}
		BOHGE_FORCEINLINE void SetPassword( const eastl::string& pass )
		{
			m_Password = pass;
		}
		BOHGE_FORCEINLINE uint GetStatusCode()
		{
			return m_StatusCode;
		}
	};
}