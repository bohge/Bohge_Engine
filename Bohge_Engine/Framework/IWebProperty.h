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
#include "Object.hpp"

#include <EASTL/string.h>

namespace BohgeEngine
{
	class BOHGE_FRAMEWORK IWebProperty : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( IWebProperty );
	private:
		IWebProperty(void){}
		~IWebProperty(void){}
	public:
		static const char* WEB_SYSTEM_STATE;
		static const char* WEB_SYSTEM_CACHE;
		static const char* FILE_CACHE_EXT;
		enum Constant
		{
			WC_MAX_THREAD_COUNT = 5,//网络最多开5个线程
		};
		enum WebJobType
		{
			JT_DOWNLOAD = 0,
			JT_POST = 1,
		};
		enum WebJobState
		{
			JS_INACTIVE = 0,
			JS_ACTIVE,
			JS_FINISH,
		};
		enum Const
		{
			DPC_BUFFE_SIZE = 32 * 1024,//缓存buffer的大小
		};
		
		enum HttpStatusCode
		{
			HS_CONTINUE			= 100,
			HS_SWITCH_PROTOCOLS	= 101,
			HS_OK				= 200,
			HS_CREATED			= 201,
			HS_ACCEPTED			= 202,
			HS_NO_CONTENT		= 204,
			HS_RESET_CONTENT	= 205,
			HS_PARTIAL_CONTENT	= 206,
			HS_MULTIPLE_CHOICES	= 300,
			HS_MOVED_PERMANENTLY= 301,
			HS_MOVE_TEMPORARILY	= 302,
			HS_SEE_OTHER		= 303,
			HS_NOT_MODIFIED		= 304,
			HS_USE_PROXY		= 305,
			HS_SWITCH_PROXY		= 306,
			HS_TEMP_REDIRECT	= 307,
			HS_BAD_REQUEST		= 400,
			HS_UNAUTHORIZED		= 401,
			HS_PAYMENT_REQUIRED	= 402,
			HS_FORBIDDEN		= 403,
			HS_NOT_FOUND		= 404,
			HS_NOT_ALLOWED		= 405,
			HS_NOT_ACCEPTABLE	= 406,
			HS_AUTHENTICATION	= 407,
			HS_REQUEST_TIMEOUT	= 408,
			HS_CONFLICT			= 409,
			HS_GONE				= 410,
			HS_LENGTH_REQUIRED	= 411,
			HS_PRECONDITION_FAILED	= 412,
			HS_INTERNAL_SERVER_ERROR= 500,
			HS_NOT_IMPLEMENTED		= 501,
			HS_BAD_GATEWAY			= 502,
			HS_SERVICE_UNAVAILABLE	= 503,
			HS_GATEWAY_TIMEOUT		= 504,
			HS_UNPARSEABLE_RESPONSE_HEADERS	= 600,

			HS_CACHED				= 10000001,//自定义响应
		};
	};
}