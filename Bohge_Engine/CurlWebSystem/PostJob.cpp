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
#include "PostJob.h"


#include <curl\curl.h>


namespace CurlWebSystemPlugin
{
	PostJob::PostJob()
		:m_pPostContent(NULL)
	{

	}

	PostJob::~PostJob()
	{

	}

	void PostJob::Connect()
	{
		ICurlJob::Connect();
		curl_easy_setopt(_GetCURL(), CURLOPT_POST, 1L);
      

		curl_easy_setopt(_GetCURL(),CURLOPT_POSTFIELDS,m_pPostContent);    
		curl_easy_setopt(_GetCURL(), CURLOPT_POSTFIELDSIZE,strlen(m_pPostContent));    
		curl_easy_setopt(_GetCURL(),CURLOPT_VERBOSE,1); 


		curl_easy_setopt(_GetCURL(), CURLOPT_WRITEDATA, &m_Buffer);
		curl_easy_setopt(_GetCURL(), CURLOPT_FOLLOWLOCATION, 1L); 
		curl_easy_setopt(_GetCURL(), CURLOPT_WRITEFUNCTION, PostJob::onWriteData);
	}

	void PostJob::_DoWebJob( BohgeEngine::SmartPtr<BohgeEngine::IJob>& self )
	{
		CURLcode code;
		code = curl_easy_perform(_GetCURL());
	}

	void PostJob::SetPostContent( const char* pContent )
	{
		m_pPostContent = pContent;
	}

	size_t PostJob::onWriteData( char *ptr, size_t size, size_t nmemb, void *userdata )
	{
		eastl::string* pbuffer = (eastl::string*)userdata;

		pbuffer->append(ptr);
		return size*nmemb;
	}

}