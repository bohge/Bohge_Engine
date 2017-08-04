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
#include "Framework/IThreadSystem.h"
#include "Framework/IThreadMutex.h"
#include "Framework/IWebProperty.h"
#include "Framework/Serializer.h"
#include "Framework/Log.h"
#include "DownloadHandle.h"
#include "DownloadJob.h"
#include "FileSlices.h"
#include "WebSystem.h"
#include "InfoJob.h"



using namespace eastl;
using namespace BohgeEngine;

namespace CurlWebSystemPlugin
{
	//------------------------------------------------------------------------------
	DownloadHandle::DownloadHandle( const WebSystem* host, BohgeEngine::IJobDispatcher* dispatcher  )
		:IDownloadHandle( dispatcher ),
		m_isRestore(false),
		m_rpHostWebSystem(host),
		m_FileSize(0),
		m_FileData(0),
		m_pFileWirter(NULL)
	{
		m_pWirterMutex = static_cast<IThreadMutex*>
			(IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_MUTEX ));
		m_pFileSlices = NEW FileSlices( IWebProperty::DPC_BUFFE_SIZE );
	}
	//------------------------------------------------------------------------------
	DownloadHandle::~DownloadHandle(void)
	{
		//m_FileWirter.close();
		if( m_pFileWirter )
		{
			m_pFileWirter->CloseFile();
			IIOSystem::Instance()->RecycleBin( m_pFileWirter );
		}
		IThreadSystem::Instance()->RecycleBin( m_pWirterMutex );
		m_pWirterMutex = NULL;
		SAFE_DELETE( m_pFileSlices );
	}
	//------------------------------------------------------------------------------
	void DownloadHandle::InfoCallBack( SmartPtr<IJob>& job )
	{
		if ( IWebProperty::JS_ACTIVE == GetState() )
		{
			SmartPtr<InfoJob> info = SmartPtr_Static_Cast<InfoJob>(job);
			if ( IWebProperty::HS_OK != info->GetStatusCode() )
			{
				_Finish( info->GetStatusCode() );
				//IIOSystem::Instance()->DeleteFile( GetCachePath() );
			}
			else
			{
				m_FileSize = info->GetFileSize();
				m_FileData = info->GetFileTime();
				if ( m_rpHostWebSystem->isCaching( m_FileData, m_FileSize, GetUrl(), GetFilePath() ) )
				{
					_Finish( IWebProperty::HS_CACHED );
					DEBUGLOG("In cache file %s\n", GetFilePath().c_str() );
				}
				else
				{
					SaveState();
					//m_FileWirter.open( IIOSystem::Instance()->PathAssembly( GetFilePath() ).c_str(), m_isRestore ? std::ofstream::binary | std::ofstream::in : std::ofstream::binary );
					m_pFileWirter = IIOSystem::Instance()->FileFactory( GetFilePath() );
					m_pFileWirter->OpenFile( IFile::AT_WRITE );
					m_pFileSlices->InitSlices( m_FileSize );
					_PushDownloadJob();
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	void DownloadHandle::DownloadCallBack( SmartPtr<IJob>& job )
	{
		if ( IWebProperty::JS_ACTIVE == GetState() )
		{
			m_pWirterMutex->Lock();//这个地方可能有性能问题，有优化空间，优化思路，写文件在独立线程中操作，这里只是把buffer交换给对方
			SmartPtr<DownloadJob> download = SmartPtr_Static_Cast<DownloadJob>(job);
			//m_FileWirter.seekp( download->GetRangeBegin(), std::ios_base::beg );
			//m_FileWirter.write( download->GetBuffer(), download->GetBufferSize() );
			//m_FileWirter.flush();
			m_pFileWirter->Seek( download->GetRangeBegin(), SEEK_SET );
			m_pFileWirter->WriteFile( download->GetBuffer(), download->GetBufferSize() );
			m_pFileSlices->JobSlicesFinish( download->GetRangeBegin(), download->GetRangeEnd() );
			if ( m_pFileSlices->isFinish() )
			{
				//m_FileWirter.close();
				m_pFileWirter->CloseFile();
				IIOSystem::Instance()->RecycleBin( m_pFileWirter );
				m_pFileWirter = NULL;
				//IIOSystem::Instance()->DeleteFile( GetCachePath() );
				_Finish( IWebProperty::HS_OK );//download->GetStatusCode() );
				DEBUGLOG("Finish download file %s\n", GetFilePath().c_str() );
				m_pWirterMutex->Unlock();
			}
			else
			{
				if ( m_pFileSlices->isAnyJob() )
				{
					uint64 begin = 0;
					uint64 end = 0;
					m_pFileSlices->GetJobSlices( begin, end );
					download->SetJobState( begin, end );
					_PushJob( job );
				}
				m_pWirterMutex->Unlock();
			}
		}
	}
	//------------------------------------------------------------------------------
	void DownloadHandle::_OnStart()
	{
		if ( !m_pFileSlices->isAnyJob() )
		{
			SmartPtr<InfoJob> info( NEW InfoJob );
			info->SetUrl( GetUrl() );
			info->SetUsername( GetUsername() );
			info->SetPassword( GetPassword() );
			info->Connect();
			info->SetCallback( MEMBER_FUNC_PTR( &DownloadHandle::InfoCallBack ) );
			DEBUGLOG("Get file infomation on url %s\n", GetUrl().c_str() );
			_PushJob( info );
		}
		else
		{
			_PushDownloadJob();
		}
	}
	//------------------------------------------------------------------------------
	void DownloadHandle::_OnPause()
	{
		m_pWirterMutex->Lock();
		//m_FileWirter.close();
		m_pFileWirter->CloseFile();
		SaveState();
		m_pWirterMutex->Unlock();
	}
	//------------------------------------------------------------------------------
	void DownloadHandle::SaveState()
	{
		//Serializer writer( GetCachePath() );
		//uint size = 0;
		//_SaveBaseHandleState( writer );
		//writer<<GetFilePath();
		//m_pFileSlices->Save( writer );
	}
	//------------------------------------------------------------------------------
	void DownloadHandle::RestroeState(  const eastl::string& path  )
	{
		//Serializer reader( path );
		//string filepath;
		//m_isRestore = true;
		//_RestroeBaseHandleState( reader );
		//reader>>filepath;
		//SetPath( filepath );
		//m_pFileSlices->Load( reader );
	}
	//------------------------------------------------------------------------------
	void DownloadHandle::_PushDownloadJob()
	{
 		for ( int i = 0 ; i < GetThreadCount() ; ++ i )
		{
			if ( m_pFileSlices->isAnyJob() )
			{
				uint64 begin = 0;
				uint64 end = 0;
				m_pFileSlices->GetJobSlices( begin, end );
				SmartPtr<DownloadJob> download( NEW DownloadJob );
				download->SetUrl( GetUrl() );
				download->SetUsername( GetUsername() );
				download->SetPassword( GetPassword() );
				download->Connect();
				download->SetJobState( begin, end );
				download->SetCallback( MEMBER_FUNC_PTR( &DownloadHandle::DownloadCallBack ) );
				_PushJob( download );
			}
		}
	}
}