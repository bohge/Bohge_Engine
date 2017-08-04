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
#include "IBaseHandle.h"


#include <EASTL/string.h>


namespace BohgeEngine
{
	class IJobDispatcher;
	class BOHGE_FRAMEWORK IDownloadHandle : public IBaseHandle
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IDownloadHandle, IBaseHandle )
	private:
		eastl::string							m_FilePath;
		eastl::string							m_FileCache;
	public:
		IDownloadHandle( IJobDispatcher* dispatcher ):IBaseHandle( IWebProperty::JT_DOWNLOAD, dispatcher ){};
		virtual ~IDownloadHandle(void){};
	private:
		virtual void _OnStart() = 0;
		virtual void _OnPause() = 0;
	public:
		virtual void SaveState() = 0;
		virtual void RestroeState( const eastl::string& path ) = 0;
	public:
		virtual eastl::string GetJobStateSavePath()
		{
			return m_FileCache;
		}
	public:
		BOHGE_FORCEINLINE void SetPath( const eastl::string& fullpath )
		{
			eastl::string cache = fullpath.substr( 0, fullpath.find_last_of( ".")  );
			m_FilePath = fullpath;
			m_FileCache = cache + IWebProperty::FILE_CACHE_EXT;
		}
	public:
		BOHGE_FORCEINLINE const eastl::string& GetCachePath()
		{
			return m_FileCache;
		}
		BOHGE_FORCEINLINE const eastl::string& GetFilePath()
		{
			return m_FilePath;
		}
	};
}
