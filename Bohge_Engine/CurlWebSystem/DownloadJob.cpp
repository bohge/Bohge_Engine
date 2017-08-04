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
#include "Framework/Log.h"
#include "DownloadJob.h"


#include <curl\curl.h>
#include <sstream>

using namespace eastl;
using namespace BohgeEngine;

namespace CurlWebSystemPlugin
{

	//------------------------------------------------------------------------------
	DownloadJob::DownloadJob(void)
		:m_BeginPos(0),
		m_EndPos(0)	
	{
	}
	//------------------------------------------------------------------------------
	DownloadJob::~DownloadJob(void)
	{
	}
	//------------------------------------------------------------------------------
	void DownloadJob::Connect()
	{
		ICurlJob::Connect();

		curl_easy_setopt(_GetCURL(), CURLOPT_WRITEDATA, &m_Buffer);
		curl_easy_setopt(_GetCURL(), CURLOPT_WRITEFUNCTION, DownloadJob::onWriteData);
	}
	//------------------------------------------------------------------------------
	void DownloadJob::_DoWebJob( SmartPtr<IJob>& self )
	{
		std::stringstream ss;
		ss<<m_BeginPos<<"-"<<m_EndPos;
		CURLcode code = curl_easy_setopt(_GetCURL(), CURLOPT_RANGE, ss.str().c_str() );
		code = curl_easy_perform(_GetCURL());		
		if(code!=CURLE_OK)
		{
			DEBUGLOG("download cancelled  %d\n",code);
		}
	}
	//------------------------------------------------------------------------------
	void DownloadJob::SetJobState( uint64 begin_pos, uint64 end_pos )
	{
		m_BeginPos = begin_pos;
		m_EndPos = end_pos;
		m_Buffer.Reset(end_pos - begin_pos + 1);
	}
	//------------------------------------------------------------------------------
	size_t DownloadJob::onWriteData( char *ptr, size_t size, size_t nmemb, void *userdata )
	{
		size_t byte_size = size * nmemb;
		DownloadJob::Buffer* buffer = (DownloadJob::Buffer*)userdata;
		buffer->CopyBuffer( ptr, byte_size );
		return byte_size;
	}



	


	

}