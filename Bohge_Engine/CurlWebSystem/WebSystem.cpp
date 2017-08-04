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
#include "Framework/IJobDispatcher.h"
#include "Framework/IThreadSystem.h"
#include "Framework/IThreadMutex.h"
#include "Framework/IBaseHandle.h"
#include "Framework/Serializer.h"
#include "Framework/Framework.h"
#include "Framework/IIOSystem.h"
#include "DownloadHandle.h"
#include "PostHandle.h"
#include "WebSystem.h"


#include <curl\curl.h>


using namespace eastl;
using namespace BohgeEngine;
namespace CurlWebSystemPlugin
{
	//------------------------------------------------------------------------------
	WebSystem::WebSystem(void)
		:m_pCachePath(NULL),
		m_pDonwloadInfoMap(NULL),
		m_pHandleMap(NULL),
		m_DonwloadSize(0),
		m_pJobDispatcher(NULL)
	{
	}
	//------------------------------------------------------------------------------
	WebSystem::~WebSystem(void)
	{
	}
	//------------------------------------------------------------------------------
	void WebSystem::OnFrameworkEntry( float time, BohgeEngine::Framework& framework )
	{
		m_pDonwloadInfoMap = NEW DonwloadInfoMap;
		m_pHandleMap = NEW HandleMap;
		m_pCachePath = NEW string;
		m_pJobDispatcher = static_cast< IJobDispatcher* >(
			IThreadSystem::Instance()->	ThreadEntityFactory( ThreadProperty::ET_JOB_DISPATCHER ));
		m_pJobDispatcher->SetThreadCount( IWebProperty::WC_MAX_THREAD_COUNT );
		m_pJobDispatcher->Start();
		m_pMapMutex = static_cast<IThreadMutex*>
			(IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_MUTEX ));
		curl_global_init(CURL_GLOBAL_ALL);
	}
	//------------------------------------------------------------------------------
	void WebSystem::OnFrameworkExit( float time, BohgeEngine::Framework& framework )
	{
		_CacheDownloadInfo();
		FOR_EACH( HandleMap, it, *m_pHandleMap )
		{
			it->second->Stop();
		}
		FOR_EACH( DonwloadInfoMap, it, *m_pDonwloadInfoMap )
		{
			SAFE_DELETE( it->second );
		}
		SAFE_DELETE( m_pHandleMap );
		SAFE_DELETE( m_pCachePath );
		SAFE_DELETE( m_pDonwloadInfoMap );
		m_pJobDispatcher->Stop();
		IThreadSystem::Instance()->RecycleBin( m_pJobDispatcher );
		IThreadSystem::Instance()->RecycleBin( m_pMapMutex );
	}
	//------------------------------------------------------------------------------
	eastl::vector< BohgeEngine::IBaseHandle* > WebSystem::_PraseJobState()
	{
		string filepath = *m_pCachePath + IWebProperty::WEB_SYSTEM_STATE;
		eastl::vector< IBaseHandle* > res;
		Serializer reader( filepath );
		string cachepath;
		reader.Open( IFile::AT_READ );
		if ( reader.isOpen() )
		{//如果已经有未完成的任务，这里可以继续下载
			uint pathsize = 0;
			reader>>pathsize;
			IBaseHandle* ptr = NULL;
			string cachepath;
			for( int i = 0 ; i < pathsize ; ++ i)
			{
				uint jt;
				reader>>jt;
				switch( jt )
				{
				case IWebProperty::JT_DOWNLOAD:
					{
						ptr = NEW DownloadHandle(this, m_pJobDispatcher);
						reader>>cachepath;
					}break;
				case IWebProperty::JT_POST: continue;
				default:ASSERT(false);
				}
				ptr->RestroeState( cachepath );
				ptr->SetStateChangeCallback( MEMBER_FUNC_PTR( &WebSystem::OnWebJobStateChange ) );
				res.push_back(ptr);
			}
		}
		return res;
	}
	//------------------------------------------------------------------------------
	void WebSystem::_CacheJobStates()
	{
		//记录正在下载的连接
		string filepath = *m_pCachePath + IWebProperty::WEB_SYSTEM_STATE;
		Serializer writer( filepath );
		uint downloadCount = 0;
		FOR_EACH( HandleMap, it, *m_pHandleMap )
		{
			switch( it->second->GetJobType() )
			{
			case IWebProperty::JT_DOWNLOAD: ++downloadCount; break;
			case IWebProperty::JT_POST: continue;
			default:ASSERT(false);
			}
		}
		if ( downloadCount > 0  )
		{
			writer<<downloadCount;
			FOR_EACH( HandleMap, it, *m_pHandleMap )
			{
				switch( it->second->GetJobType() )
				{
				case IWebProperty::JT_DOWNLOAD:
					{
						writer<<it->second->GetJobType()
							<<it->second->GetJobStateSavePath();
					}break;
				case IWebProperty::JT_POST: continue;
				default:ASSERT(false);
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	void WebSystem::_PraseDownloadInfo()
	{
		string filepath = *m_pCachePath + IWebProperty::WEB_SYSTEM_CACHE;
		Serializer reader( filepath );
		reader.Open( IFile::AT_READ );
		if ( reader.isOpen() )
		{
			uint size;
			reader>>size;
			multimap< uint, DownloadInfo* > dataordemap;
			for ( int i = 0 ; i < size ; ++ i )
			{
				DownloadInfo* info = NEW DownloadInfo;
				reader
					>>info->m_Size
					>>info->m_Date
					>>info->m_Url
					>>info->m_Path;
				dataordemap.insert( make_pair( info->m_Date, info ) );
				if ( i >= WC_CACHE_DOWNLOAD_INFO_MAX_SIZE )
				{
					dataordemap.erase( dataordemap.begin() );
				}
			}
			for( multimap< uint, DownloadInfo* >::iterator it = dataordemap.begin();
				it != dataordemap.end();
				++ it )
			{
				m_pDonwloadInfoMap->insert( make_pair( Utility::HashCode( it->second->m_Url + it->second->m_Path ), it->second ) );
			}
		}
	}
	//------------------------------------------------------------------------------
	void WebSystem::_CacheDownloadInfo()
	{
		//记录正在下载的连接
		string filepath = *m_pCachePath + IWebProperty::WEB_SYSTEM_CACHE;
		Serializer writer( filepath );
		writer<<m_pDonwloadInfoMap->size();
		FOR_EACH( DonwloadInfoMap, it, *m_pDonwloadInfoMap )
		{
			writer
				<<it->second->m_Size
				<<it->second->m_Date
				<<it->second->m_Url
				<<it->second->m_Path;
		}
	}
	//------------------------------------------------------------------------------
	void WebSystem::_InsertDonwloadInfo( DownloadHandle* h )
	{
		DownloadInfo* info = NULL;
		uint hash = Utility::HashCode( h->GetUrl() + h->GetFilePath() );
		DonwloadInfoMap::iterator it = m_pDonwloadInfoMap->find( hash );
		if ( m_pDonwloadInfoMap->end() != it )//如果找到了，则更新
		{
			info = it->second;
		}
		else
		{
			info = NEW DownloadInfo;
			m_pDonwloadInfoMap->insert( make_pair( hash, info ) );
		}
		info->m_Url = h->GetUrl();
		info->m_Path = h->GetFilePath();
		info->m_Date = h->GetFileTime();
		info->m_Size = h->GetFileSize();		
		m_DonwloadSize += h->GetFileSize();
		if ( m_DonwloadSize >= WC_CACHE_EACH_DOWNLOAD_SIZE )
		{
			m_DonwloadSize = 0;
			_CacheDownloadInfo();//保存
		}
	}
	//------------------------------------------------------------------------------
	bool WebSystem::isCaching( uint date, uint64 size, const eastl::string& url, const eastl::string& path ) const
	{
		DonwloadInfoMap::const_iterator cit = m_pDonwloadInfoMap->find( Utility::HashCode( url+path ) );
		if ( m_pDonwloadInfoMap->end() == cit )
		{
			return false;
		}
		else if ( date != cit->second->m_Date
			|| size != cit->second->m_Size
			|| !IIOSystem::Instance()->isFileExist( path ) )
		{
			return false;
		}
		return true;
	}
	//------------------------------------------------------------------------------
	eastl::vector< IBaseHandle* > WebSystem::SetCachePath( const eastl::string& path )
	{
		*m_pCachePath = path;
		_PraseDownloadInfo();
		return vector< IBaseHandle* >();//_PraseJobState();
	}
	//------------------------------------------------------------------------------
	IBaseHandle* WebSystem::WebFactory( IWebProperty::WebJobType jt )
	{
		IBaseHandle* res;
		switch( jt )
		{
		case IWebProperty::JT_DOWNLOAD:
			{
				res = NEW DownloadHandle(this, m_pJobDispatcher);
			}break;
		case IWebProperty::JT_POST:
			{
				res = NEW PostHandle(m_pJobDispatcher);
			}break;
		default: ASSERT(false);
		}
		res->SetStateChangeCallback( MEMBER_FUNC_PTR( &WebSystem::OnWebJobStateChange ) );
		m_pMapMutex->Lock();
		m_pHandleMap->insert( make_pair( res->GetObjectID(), res ) );
		m_pMapMutex->Unlock();
		return res;
	}
	//------------------------------------------------------------------------------
	void WebSystem::RecycleBin( BohgeEngine::IBaseHandle* h )
	{
		m_pMapMutex->Lock();
		m_pHandleMap->erase( h->GetObjectID() );
		//_CacheJobStates();
		m_pMapMutex->Unlock();
		SAFE_DELETE( h );
	}
	//------------------------------------------------------------------------------
	void WebSystem::OnWebJobStateChange( IBaseHandle* ptr )
	{
		m_pMapMutex->Lock();
		switch( ptr->GetState() )
		{
		case IWebProperty::JS_ACTIVE:break;
		case IWebProperty::JS_FINISH:
			{
				m_pHandleMap->erase( ptr->GetObjectID() );
				if ( IWebProperty::JT_DOWNLOAD == ptr->GetJobType()
					&& IWebProperty::HS_OK == ptr->GetStatusCode() )
				{
					_InsertDonwloadInfo( static_cast<DownloadHandle*>(ptr) );
				}
			}break;
		case IWebProperty::JS_INACTIVE:break;
		default:ASSERT(false);
		}
		//_CacheJobStates();
		m_pMapMutex->Unlock();
	}
	//------------------------------------------------------------------------------
	void WebSystem::RegistEvent( BohgeEngine::Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &WebSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &WebSystem::OnFrameworkExit ) );
	}
}