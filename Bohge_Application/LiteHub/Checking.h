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
#include "Application/ILiteapp.h"
#include "JsonResourceList.h"


namespace BohgeEngine
{	
	class JsonWriter;
	class IBaseHandle;
	class IThreadMutex;
	class IDownloadHandle;
}
namespace BohgeHub
{
	//检查更新Hub脚本，为将来检查更新dll做准备
	class Checking : public BohgeApp::ILiteapp
	{
	private:
		typedef eastl::map< uint, eastl::pair< JsonLocalResourceInfo*, uint > > ResourceDownloadInfoMap;
		typedef eastl::list< BohgeEngine::IDownloadHandle* > DownloadList;
		typedef eastl::list< eastl::pair< eastl::string, eastl::string > > ResourceList;
		typedef eastl::list< BohgeEngine::IBaseHandle* > IBaseHandleList;
	private:
		IBaseHandleList				m_ReleaseList;//准备析构的网络链接
		DownloadList				m_DelayDownloadList;//同时最多下载N个文件，其他的放在这个list中准备下载
		ResourceDownloadInfoMap		m_ResourceDownloadInfoMap;//现在更新的资源信息
		JsonLocalResourceList		m_LocalResourceList;//本地数据资料
		eastl::string				m_LocalPath;
		eastl::string				m_LiteappUrl;
		BohgeEngine::JsonWriter*	m_pJsonWriter;
		BohgeEngine::IThreadMutex*	m_pMutex;
		uint						m_JobCount;
		bool						m_isFinish;
	public:
		Checking(void);
		~Checking(void);
	private:
		void _DownloadResources( const ResourceList& reslist );
		void _EntryLiteapp();
		void _AllResource( ResourceList& reslist, const JsonEntryInfo& info );
		void _CompareResource( ResourceList& reslist, const JsonEntryInfo& info );
	public:
		void OnRequestResourceList( BohgeEngine::IBaseHandle* h );//得到对应版本的资源列表
		void OnDownloadResource( BohgeEngine::IBaseHandle* h );//得到对应版本的资源列表
	private:
		virtual void _OnBeginLaunch( BohgeEngine::Framework& framework );
		virtual void _OnEndLaunch( BohgeEngine::Framework& framework );
		virtual void _OnUpdata( BohgeEngine::Framework& framework );
		virtual void _OnExit( BohgeEngine::Framework& framework );
	public:
		BOHGE_FORCEINLINE void SetLiteappUrl( const eastl::string& url )
		{
			m_LiteappUrl = url;
		}
	};
}