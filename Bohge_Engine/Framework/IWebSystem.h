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
#include "IWebProperty.h"
#include "IPlugin.h"


#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/map.h>



namespace BohgeEngine
{
	class IBaseHandle;
	class BOHGE_FRAMEWORK IWebSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR( IWebSystem );
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IWebSystem, IPlugin )
	public:
		IWebSystem(void){SYSTEM_SINGLETON_INITIALIZE;};
		virtual ~IWebSystem(void){SYSTEM_SINGLETON_DESTROY;};
	public:
		virtual eastl::vector< IBaseHandle* > SetCachePath( const eastl::string& path ) = 0;//设置缓存文件目录,如果目录下已有缓存，则解析缓存，判断是否有未完成的任务
		virtual IBaseHandle* WebFactory( IWebProperty::WebJobType jt ) = 0;
		virtual void RecycleBin( IBaseHandle* h ) = 0;
	};
}
