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
#include "ICurlJob.h"


#include <curl\curl.h>

using namespace BohgeEngine;

namespace CurlWebSystemPlugin
{
	
	//------------------------------------------------------------------------------
	ICurlJob::ICurlJob( void )
		:m_isLiving(true),
		m_StatusCode(0)
	{
		m_pCurl = curl_easy_init();
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, FALSE);
	}
	//------------------------------------------------------------------------------
	ICurlJob::~ICurlJob( void )
	{		
		curl_easy_cleanup( m_pCurl );
	}
	//------------------------------------------------------------------------------
	int ICurlJob::ProgressCallback(void *job,double t,double d, double ultotal,double ulnow)
	{
		bool* living = (bool*)job;
		return living ? 0 : 1;
	}
	//------------------------------------------------------------------------------
	void ICurlJob::Connect()
	{
		/* specify URL to get */ 
		curl_easy_setopt(_GetCURL(), CURLOPT_URL, m_Url.c_str());
		curl_easy_setopt(_GetCURL(), CURLOPT_USERNAME, m_Username.c_str() );
		curl_easy_setopt(_GetCURL(), CURLOPT_PASSWORD, m_Password.c_str() );	
		curl_easy_setopt(_GetCURL(), CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(_GetCURL(), CURLOPT_PROGRESSFUNCTION, ProgressCallback);
		curl_easy_setopt(_GetCURL(), CURLOPT_PROGRESSDATA, &m_isLiving);
		curl_easy_setopt(_GetCURL(), CURLOPT_FOLLOWLOCATION, 1L);
		//curl_easy_setopt(_GetCURL(), CURLOPT_PROXY, "127.0.0.1:8888");
// 		curl_easy_setopt(_GetCURL(), CURLOPT_LOW_SPEED_LIMIT,1);
// 		curl_easy_setopt(_GetCURL(), CURLOPT_LOW_SPEED_TIME,5);
	}
	//------------------------------------------------------------------------------
	void ICurlJob::_DoJob( SmartPtr<IJob>& self )
	{
		_DoWebJob( self );
		if ( m_isLiving )
		{
			curl_easy_getinfo (m_pCurl, CURLINFO_RESPONSE_CODE, &m_StatusCode);
			m_JobDoneCallback.Multicast( self );
		}
	}
	//------------------------------------------------------------------------------
	void ICurlJob::SetCallback( Delegate1<void,SmartPtr<IJob>&>::FunctionType* callback )
	{
		m_JobDoneCallback.Connect( callback  );
	}
	//------------------------------------------------------------------------------
	void ICurlJob::SetUrl( const eastl::string& url )
	{
		m_Url = url;		
	}
	//------------------------------------------------------------------------------
	void ICurlJob::_DoYieldJob()
	{
		m_isLiving = false;
	}

}