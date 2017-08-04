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
#include "Framework/IDownloadHandle.h"
#include "Application/IApplication.h"
#include "Framework/IThreadSystem.h"
#include "Framework/IThreadMutex.h"
#include "Framework/IPostHandle.h"
#include "Framework/IWebSystem.h"
#include "Framework/JsonReader.h"
#include "Framework/JsonWriter.h"
#include "Framework/IIOSystem.h"
#include "Framework/Framework.h"
#include "DefualtResource.h"
#include "Framework/IFile.h"
#include "Framework/Log.h"
#include "Checking.h"
#include "BLiteapp.h"



BohgeApp::ILiteapp* CreateEnterPoint()
{
	BohgeHub::Checking* ret = NEW BohgeHub::Checking;
	ret->SetLiteappUrl( BohgeHub::DefaultResource::HUB_RESOURCE_WEBSITE );
	return ret;
}
void DestroyLiteapp( BohgeApp::ILiteapp* app )
{
	SAFE_DELETE( app );
}

using namespace eastl;
using namespace BohgeApp;
using namespace BohgeEngine;
namespace BohgeHub
{
	//---------------------------------------------------------------------------------------------------------
	Checking::Checking(void)
		:m_JobCount(0),
		m_pJsonWriter(NULL),
		m_pMutex(NULL),
		m_isFinish(false)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	Checking::~Checking(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void Checking::_OnBeginLaunch( Framework& framework )
	{
		//加载默认插件
		_PushPlugin( "IO",		"IOSystem" );
		_PushPlugin( "TIME",	"TimeSystem" );
		_PushPlugin( "THREAD",	"ThreadSystem" );
		_PushPlugin( "WEB",		"CurlWebSystem" );
	}
	//---------------------------------------------------------------------------------------------------------
	void Checking::_OnEndLaunch( Framework& framework )
	{
		IIOSystem::Instance()->SetResourcePath( framework.GetConfig().GetResourcePath() );
		m_pJsonWriter = NEW JsonWriter;
		m_pMutex = static_cast<IThreadMutex*>
			(IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_MUTEX ));
		IWebSystem::Instance()->SetCachePath( "docs:WebCache/" );
		IPostHandle* ch = 
			static_cast<IPostHandle*>( 
			IWebSystem::Instance()->WebFactory( IWebProperty::JT_POST ));
		ch->SetUrl( m_LiteappUrl );//检查默认的
		ch->SetPostContent( DefaultResource::REQUEST_APP_PLATFORM, DefaultResource::APP_PLATFORM );
		ch->SetPostContent( DefaultResource::REQUEST_APP_VERSION, DefaultResource::APP_VERSION );
		ch->SetFinishCallback( MEMBER_FUNC_PTR(&Checking::OnRequestResourceList) );
		ch->Start();
	}
	//---------------------------------------------------------------------------------------------------------
	void Checking::_OnExit( BohgeEngine::Framework& framework )
	{
		SAFE_DELETE( m_pJsonWriter );
		IThreadSystem::Instance()->RecycleBin( m_pMutex );
	}
	//---------------------------------------------------------------------------------------------------------
	void Checking::_AllResource( ResourceList& reslist, const JsonEntryInfo& info )
	{
		m_LocalResourceList.m_ResinfoArray.Resize( info.m_ResinfoArray.Getsize() );
		for ( int i = 0; i < info.m_ResinfoArray.Getsize(); ++ i )
		{
			const JsonResourceInfo& res = info.m_ResinfoArray.At<JsonResourceInfo>(i);
			JsonLocalResourceInfo& local = m_LocalResourceList.m_ResinfoArray.At<JsonLocalResourceInfo>(i);
			string url = info.m_BaseUrl + res.m_ResUrl;
			local.m_LoacalPath = res.m_LoacalPath;
			local.m_Version = 0;
			m_ResourceDownloadInfoMap.insert(
				eastl::make_pair(
				Utility::HashCode( url ),
				eastl::make_pair(
				&m_LocalResourceList.m_ResinfoArray.At<JsonLocalResourceInfo>(i),
				res.m_Version)));
			reslist.push_back( eastl::make_pair( url, m_LocalPath + res.m_LoacalPath ) );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Checking::_CompareResource( ResourceList& reslist, const JsonEntryInfo& info )
	{
		for ( int i = 0; i < info.m_ResinfoArray.Getsize() ; ++ i)
		{
			const JsonResourceInfo& webres = info.m_ResinfoArray.At<JsonResourceInfo>(i);
			const JsonLocalResourceInfo& locres = m_LocalResourceList.m_ResinfoArray.At<JsonLocalResourceInfo>(i);
			if ( webres.m_LoacalPath != locres.m_LoacalPath )//说明本地没有这个资源
			{
				string url = info.m_BaseUrl + webres.m_ResUrl;
				JsonLocalResourceInfo ni;
				ni.m_LoacalPath = webres.m_LoacalPath;
				ni.m_Version = 0;
				m_LocalResourceList.m_ResinfoArray.Insert( i, ni );
				m_ResourceDownloadInfoMap.insert(
					eastl::make_pair(
					Utility::HashCode( url ),
					eastl::make_pair(
					&m_LocalResourceList.m_ResinfoArray.At<JsonLocalResourceInfo>(i),
					webres.m_Version)));
				reslist.push_back( eastl::make_pair( url, m_LocalPath + webres.m_LoacalPath ) );	
			}
			else//说明本地有这个资源，检测版本是否一致
			{
				if ( webres.m_Version != locres.m_Version )//说明资源不匹配
				{
					string url = info.m_BaseUrl + webres.m_ResUrl;
					m_ResourceDownloadInfoMap.insert(
						eastl::make_pair(
						Utility::HashCode( url ),
						eastl::make_pair(
						&m_LocalResourceList.m_ResinfoArray.At<JsonLocalResourceInfo>(i),
						webres.m_Version)));
					reslist.push_back( eastl::make_pair( url, m_LocalPath + webres.m_LoacalPath ) );	
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Checking::OnRequestResourceList( BohgeEngine::IBaseHandle* h )
	{
		if ( IWebProperty::HS_OK != h->GetStatusCode() )
		{
			DEBUGLOG("Can not connect server!\r\n");
		}
		else
		{
			ResourceList Reslist;
			ASSERT( IWebProperty::JS_FINISH == h->GetState() );
			JsonReader webreader;
			webreader.Parse( static_cast<IPostHandle*>(h)->GetResult() );
			JsonEntryInfo webinfo;
			webreader.Read( webinfo );
			m_LocalPath = string( DefaultResource::RESOURCE_PATH ) + webinfo.m_Bundle + PLANTFORMDELIMITER;
			string infopath = m_LocalPath + DefaultResource::LITE_INFO_FILE;
			if ( webinfo.m_isValid )
			{
				IFile* file = IIOSystem::Instance()->FileFactory( infopath );
				file->OpenFile( IFile::AT_READ );
				if ( !file->isOpen() )//如果没打卡，则从新下载相应资源
				{
					_AllResource( Reslist, webinfo );
				}
				else//如果打开了则对比一下
				{
					eastl::string locallif;
					locallif.resize( file->GetSize() );
					file->ReadFile( &locallif[0], file->GetSize() );
					file->CloseFile();
					IIOSystem::Instance()->RecycleBin( file );
					JsonReader loacalreader;
					if( loacalreader.Parse( locallif ) )//可能本地json被损坏导致无法读取
					{
						loacalreader.Read( m_LocalResourceList );
						_CompareResource( Reslist, webinfo );
					}
					else
					{
						DEBUGLOG("Resource list is invaild");
						_AllResource( Reslist, webinfo );
					}
				}
			}
			else
			{
				ASSERT( false && "Application's version is not valiable!\n" );
			}
			if ( !Reslist.empty() )
			{
				DEBUGLOG("Need download resource\n");
				m_pJsonWriter->Open( DefaultResource::APP_VERSION_INT, infopath );
				_DownloadResources( Reslist );
			}
			else
			{
				m_pMutex->Lock();
				m_isFinish = true;
				m_ReleaseList.push_back( h );
				m_pMutex->Unlock();
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Checking::_DownloadResources( const ResourceList& reslist )
	{
		int downloading = DefaultResource::MAX_DOWNLOAD_JOBS;
		FOR_EACH_CONST( ResourceList, res, reslist )
		{
			++ m_JobCount;
			IDownloadHandle* h =
				static_cast<IDownloadHandle*>( 
				IWebSystem::Instance()->WebFactory( IWebProperty::JT_DOWNLOAD ));
			h->SetUrl( res->first );
			h->SetPath( res->second );
			h->SetFinishCallback( MEMBER_FUNC_PTR(&Checking::OnDownloadResource) );
			if ( downloading > 0  )
			{
				-- downloading;
				h->Start();
			}
			else
			{
				m_DelayDownloadList.push_back( h );
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Checking::OnDownloadResource( BohgeEngine::IBaseHandle* h )
	{
		m_pMutex->Lock();
		if ( IWebProperty::JS_FINISH == h->GetState() )
		{
			ResourceDownloadInfoMap::iterator it = m_ResourceDownloadInfoMap.find( Utility::HashCode( h->GetUrl() ) );
			it->second.first->m_Version = it->second.second;
			m_pJsonWriter->Write( m_LocalResourceList );
			if ( 0 == -- m_JobCount )
			{
				m_pJsonWriter->Close();
				m_isFinish = true;
			}
			else if( ! m_DelayDownloadList.empty() )//继续下载
			{
				IDownloadHandle* job = m_DelayDownloadList.front();
				m_DelayDownloadList.pop_front();
				job->Start();
			}
		}
		m_ReleaseList.push_back( h );
		m_pMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void Checking::_EntryLiteapp()
	{
		DEBUGLOG( "Entry Liteapp\n" );
		BLiteapp* app = NEW BLiteapp;
		app->OpenConfig( m_LocalPath, DefaultResource::LITEAPP_ENTRY );
		IApplication::Instance()->SwapLiteapp( app );
	}
	//---------------------------------------------------------------------------------------------------------
	void Checking::_OnUpdata( BohgeEngine::Framework& framework )
	{
		m_pMutex->Lock();
		FOR_EACH( IBaseHandleList, handle, m_ReleaseList )
		{
			(*handle)->Stop();
			IWebSystem::Instance()->RecycleBin( *handle );
		}
		m_ReleaseList.clear();
		if ( m_isFinish )
		{
			_EntryLiteapp();
		}
		m_pMutex->Unlock();
	}



}