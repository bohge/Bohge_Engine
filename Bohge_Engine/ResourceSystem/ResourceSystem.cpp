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
#include "Framework/IMonopolyResource.h"
#include "Framework/IMonopolySource.h"
#include "Framework/ISharedResource.h"
#include "Framework/IJobDispatcher.h"
#include "Framework/ISharedSource.h"
#include "Framework/IThreadSystem.h"
#include "Framework/IThreadMutex.h"
#include "Framework/Framework.h"
#include "ResourceSystem.h"
#include "SkeletonLoader.h"
#include "TextureLoader.h"
#include "MeshLoader.h"


using namespace eastl;
using namespace BohgeEngine;

namespace ResourceSystemPlugin
{
	//------------------------------------------------------------------------------------------------------
	ResourceSystem::ResourceSystem()
	{
	}
	//------------------------------------------------------------------------------------------------------
	ResourceSystem::~ResourceSystem()
	{
		ASSERT( 0 == m_SharedRenderLoadedSource.size() );
		ASSERT( 0 == m_SharedLogicLoadedSource.size() );
		ASSERT( 0 == m_MonopolyRenderLoadedSource.size() );
		ASSERT( 0 == m_MonopolyLogicLoadedSource.size() );
		ASSERT( 0 == m_RenderReleaseResource.size() );
		ASSERT( 0 == m_LogicReleaseResource.size() );
		for( int i = 0; i < BohgeEngine::IResourceProperty::SST_COUNT ; ++ i )
		{
			//FOR_EACH( IResourcePairMap, it, m_IResourcePairMap[i] )
			//{
			//	DEBUGLOG( "unreleased resource id is %d, hash is %d\r\n", i, it->second->m_RootSource->GetHashCode() );
			//}
			ASSERT( 0 == m_IResourcePairMap[i].size() );
		}
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::RegistEvent( BohgeEngine::Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_ENTRY, MEMBER_FUNC_PTR( &ResourceSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_EXIT, MEMBER_FUNC_PTR( &ResourceSystem::OnFrameworkRenderExit ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_BEGIN_RENDER, MEMBER_FUNC_PTR( &ResourceSystem::OnFrameworkBeginRender ) );

		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_BEGIN_UPDATE, MEMBER_FUNC_PTR( &ResourceSystem::OnFrameworkBeginUpdata ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &ResourceSystem::OnFrameworkLogicExit ) );
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::OnFrameworkEntry( float time, Framework& framework )
	{
		m_pMutex = static_cast<IThreadMutex*>
			(IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_MUTEX ));
		m_pIJobDispatcher  = static_cast<IJobDispatcher*>
			(IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_JOB_DISPATCHER ));
		m_pIJobDispatcher->SetSortType( IJobDispatcher::ST_GREATER );
		m_pIJobDispatcher->Start();
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::OnFrameworkRenderExit( float time, Framework& framework )
	{
		_DoReleaseResource( m_RenderReleaseResource );
		m_pIJobDispatcher->Stop();
		IThreadSystem::Instance()->RecycleBin( m_pMutex );
		IThreadSystem::Instance()->RecycleBin( m_pIJobDispatcher );
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::OnFrameworkLogicExit( float time, BohgeEngine::Framework& framework )
	{
		_DoReleaseResource( m_LogicReleaseResource );
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::OnFrameworkBeginUpdata( float time, Framework& framework )
	{
		_ProcessSharedSource( m_SharedLogicLoadedSource );
		_ProcessMonopolySource( m_MonopolyLogicLoadedSource );
		_DoReleaseResource( m_LogicReleaseResource );
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::OnFrameworkBeginRender( float time, BohgeEngine::Framework& framework )
	{
		_ProcessSharedSource( m_SharedRenderLoadedSource );
		_ProcessMonopolySource( m_MonopolyRenderLoadedSource );
		_DoReleaseResource( m_RenderReleaseResource );
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::_ProcessSharedSource( ISourceList& que )
	{
		while( !que.empty() )
		{
			m_pMutex->Lock();
			SmartPtr<ISharedSource> source = que.front();
			IResourcePairMap& irpmTar = m_IResourcePairMap[source->GetGPUSourceType()];
			que.pop_front();
			m_pMutex->Unlock();
			IResourcePairMap::iterator pair = irpmTar.find( source->GetHashCode() );
			ASSERT( irpmTar.end() != pair );
			if ( 0 == pair->second->m_NeederMap.size() )
			{//没有需求就白加载了，直接删除资源
				switch( pair->second->m_RootSource->GetDispatchThread() )
				{
				case IResourceProperty::DT_RENDER_THREAD: m_RenderReleaseResource.push_back( pair->second );break;
				case IResourceProperty::DT_LOGIC_THREAD: m_LogicReleaseResource.push_back( pair->second );break;
				default:ASSERT(false);
				}
				irpmTar.erase( pair );//删除资源队列中的数据
			}
			else
			{
				if ( NULL == pair->second->Resource )//资源还没做
				{
					if ( source->isJustDepends() )
					{
						pair->second->m_WaitingMap.insert( make_pair( source->GetObjectID(), source ) );
						source->ReleaseUnnecessaryDataAfterLoaded();//释放内存数据
						return;//不走之后的流程
					}
					else
					{
						ISharedResource* res = source->MakeResource();//制作数据
						pair->second->Resource = res;
						FOR_EACH( SourceMap, it, pair->second->m_WaitingMap )
						{
							pair->second->Resource->Increment();
							//it->second->GetListener()->Invoker( res );
							it->second->CastDoneEvent( res );
						}
						pair->second->m_WaitingMap.clear();
					}
				}
				SourceMap::iterator needer =
					pair->second->m_NeederMap.find( source->GetObjectID() );
				if ( pair->second->m_NeederMap.end() != needer )
				{
					pair->second->Resource->Increment();
					//source->GetListener()->Invoker( pair->second->Resource );
					source->CastDoneEvent( pair->second->Resource );
				}
				source->ReleaseUnnecessaryDataAfterLoaded();//释放内存数据
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::_ProcessMonopolySource( ISourceList& que )
	{
		while( !que.empty() )
		{
			m_pMutex->Lock();
			SmartPtr<IMonopolySource> source = que.front();
			que.pop_front();
			m_pMutex->Unlock();
			source->CastDoneEvent( source->MakeResource() );
			source->ReleaseUnnecessaryDataAfterLoaded();//释放内存数据
		}
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::_MonopolySourcrLoadedCallBacke( const BohgeEngine::SmartPtr<BohgeEngine::IMonopolySource>& source )
	{
		m_pMutex->Lock();
		switch( source->GetDispatchThread() )
		{
		case IResourceProperty::DT_LOGIC_THREAD: m_MonopolyLogicLoadedSource.push_back( source );break;
		case IResourceProperty::DT_RENDER_THREAD: m_MonopolyRenderLoadedSource.push_back( source );break;
		default:ASSERT(false);
		}
		m_pMutex->Unlock();
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::_SharedSourcrLoadedCallBacke( const BohgeEngine::SmartPtr<BohgeEngine::ISharedSource>& source )
	{
		//现在的结构同步加载只能在加载完之后在下一帧正常使用
		//当数据在异步线程加载完毕后，会调用这个函数将数据push到队列中，等待在主线程制作成资源
		m_pMutex->Lock();
		IResourcePairMap& irpmTar = m_IResourcePairMap[source->GetGPUSourceType()];
		IResourcePairMap::iterator res = irpmTar.find( source->GetHashCode() );
		if ( irpmTar.end() != res )
		{
			SourceMap::iterator it = res->second->m_NeederMap.find( source->GetObjectID() );
			switch( res->second->m_RootSource->GetDispatchThread() )
			{
			case IResourceProperty::DT_LOGIC_THREAD: m_SharedLogicLoadedSource.push_back( it->second );break;
			case IResourceProperty::DT_RENDER_THREAD: m_SharedRenderLoadedSource.push_back( it->second );break;
			default:ASSERT(false);
			}
		}
		m_pMutex->Unlock();
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::OnSourceLoadedCallBack( const SmartPtr<IAsyncSource>& source )
	{
		switch( source->GetSourceType() )
		{
		case IResourceProperty::CST_SHARED:		_SharedSourcrLoadedCallBacke( source ); break;
		case IResourceProperty::CST_MONOPOLY:	_MonopolySourcrLoadedCallBacke( source ); break;
		default: ASSERT(false);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::_LoadSharedSource( BohgeEngine::SmartPtr<BohgeEngine::ISharedSource> source )
	{
		m_pMutex->Lock();//这里也需要加锁，在异步线程可能调用这里，push新的资源进来
		IResourcePairMap& irpmTar = m_IResourcePairMap[source->GetGPUSourceType()];
		IResourcePairMap::iterator res = irpmTar.find( source->GetHashCode() );
		if ( irpmTar.end() != res )//找到这个资源就直接插入一个需求者
		{
			source->SetJustDepends();
			res->second->m_NeederMap.insert( make_pair( source->GetObjectID(), source ) );
		}
		else
		{
			//如果都没找到，就添加到加载队列中
			SharedResourcePair* pair = NEW SharedResourcePair();
			pair->m_RootSource = source;
			pair->m_NeederMap.insert( make_pair( source->GetObjectID(), source ) );
			irpmTar.insert( make_pair( source->GetHashCode(), pair ) );
		}
		source->SetLoadedCallBacke( MEMBER_FUNC_PTR( &ResourceSystem::OnSourceLoadedCallBack ) );
		m_pMutex->Unlock();
		m_pIJobDispatcher->PushJob( source );//全部的资源都需要走加载流程，加载依赖项
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::_LoadMonoPolySource( BohgeEngine::SmartPtr<BohgeEngine::IMonopolySource> source )
	{
		source->SetLoadedCallBacke( MEMBER_FUNC_PTR( &ResourceSystem::OnSourceLoadedCallBack ) );
		m_pIJobDispatcher->PushJob( source );//全部的资源都需要走加载流程，加载依赖项
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::LoadResource( SmartPtr<IAsyncSource> source )
	{
		switch( source->GetSourceType() )
		{
		case IResourceProperty::CST_SHARED:		_LoadSharedSource( source ); break;
		case IResourceProperty::CST_MONOPOLY:	_LoadMonoPolySource( source ); break;
		default: ASSERT(false);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::_UnloadSharedSource( BohgeEngine::SmartPtr<BohgeEngine::ISharedSource> source )
	{
		m_pMutex->Lock();
		IResourcePairMap& irpmTar = m_IResourcePairMap[source->GetGPUSourceType()];
		IResourcePairMap::iterator pair = irpmTar.find( source->GetHashCode() );
		ASSERT( irpmTar.end() != pair );
		if ( NULL != pair->second->Resource )//找到这个资源就直接减少引用计数
		{
			if( 0 == pair->second->Resource->Decrement() )//减少引用计数
			{
				//pair->second->Resource->ReleaseResource();//释放资源
				//SAFE_DELETE( pair->second->Resource );//删除资源
				//SAFE_DELETE( pair->second );//删除资源对
				switch( pair->second->m_RootSource->GetDispatchThread() )
				{
				case IResourceProperty::DT_RENDER_THREAD: m_RenderReleaseResource.push_back( pair->second );break;
				case IResourceProperty::DT_LOGIC_THREAD: m_LogicReleaseResource.push_back( pair->second );break;
				default:ASSERT(false);
				}
				irpmTar.erase( pair );//删除资源队列中的数据
			}
		}
		else
		{
			pair->second->m_NeederMap.erase( source->GetObjectID() );//如果在加载队列中就去掉需求者
			pair->second->m_WaitingMap.erase( source->GetObjectID() );
		}
		m_pMutex->Unlock();
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::UnloadResource( SmartPtr<IAsyncSource> source )
	{
		switch( source->GetSourceType() )
		{
		case IResourceProperty::CST_SHARED: _UnloadSharedSource( source ); break;
		case IResourceProperty::CST_MONOPOLY: break;//cpu资源不涉及共享，这里直接退出
		default: ASSERT(false);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::_DoReleaseResource( ResourcePairList& que )
	{
		while( !que.empty() )
		{
			m_pMutex->Lock();
			SharedResourcePair* ptr = que.front();
			que.pop_front();
			m_pMutex->Unlock();
			ptr->Resource->ReleaseResource();
			if ( ptr->m_RootSource->isHostingFree() )
			{
				SAFE_DELETE( ptr->Resource );//删除资源
			}
			SAFE_DELETE( ptr );
		}
	}
	//------------------------------------------------------------------------------------------------------
	ILoader* ResourceSystem::LoaderFactory( IResourceProperty::SourceLoaderType slt )
	{
		switch( slt )
		{
		case IResourceProperty::SLT_TEXTURE: return NEW TextureLoader;
		case IResourceProperty::SLT_SKELETON: return NEW SkeletonLoader;
		case IResourceProperty::SLT_MESH: return NEW MeshLoader;
		default: ASSERT( false );
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceSystem::RecycleBin( ILoader* ptr )
	{
		SAFE_DELETE( ptr );
	}
}