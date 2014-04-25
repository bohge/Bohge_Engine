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



/////////////////////////
//		资源读取	   //
/////////////////////////
#include "ResourceManager.h"
#include "Model.h"
#include "Texture.h"
#include "Device.h"
#include "Engine.h"
#include "Log.h"
#include "TextureData.h"
#include "Utility.h"
#include "ISource.h"
#include "IResource.h"
#include "GreaterThread.h"
#include "ThreadMutex.h"


using namespace std;


namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	ResourceManager::ResourceManager()
	{
		m_pLoadingThread = NEW GreaterThread();
		m_pLoadingThread->SetPriority( Threading::TP_HIGH );
		m_pLoadingThread->Start();

		m_pMutex = NEW ThreadMutex();
	}
	//------------------------------------------------------------------------------------------------------
	ResourceManager::~ResourceManager()
	{
		m_pLoadingThread->Stop();
		SAFE_DELETE( m_pLoadingThread );
		SAFE_DELETE( m_pMutex );
	}
	//------------------------------------------------------------------------------------------------------
	const TextureData& ResourceManager::LoadTexture2D( Device::PixelFormat pf, const std::string& filename)
	{
		TextureReferenceMap::iterator it = m_TextureMap.find( Utility::HashCode(filename + static_cast<char>( pf+1 )) );
		if( it != m_TextureMap.end()  )
		{
			it->second.second++;
			return *(it->second.first);
		}
		TextureData* pdata = NEW TextureData();
		if( Engine::Instance().GetDevice()->LoadTextureFromFile( pf, *pdata, filename) )
		{
			m_TextureMap.insert( std::make_pair( Utility::HashCode(filename + static_cast<char>( pdata->m_eFormat+1 )), std::make_pair( pdata, 1 ) ) );
			return *pdata;
		}
		ASSERT(false);
		return *pdata;
	}
	//------------------------------------------------------------------------------------------------------
	const TextureData& ResourceManager::LoadTextureCube( Device::PixelFormat pf, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r )
	{
		std::string filename = f+b+u+d+l+r;
		TextureReferenceMap::iterator it = m_TextureMap.find( Utility::HashCode(filename + static_cast<char>( pf+1 )) );
		if( it != m_TextureMap.end()  )
		{
			it->second.second++;
			return *(it->second.first);
		}

		TextureData* pdata = NEW TextureData();
		if( Engine::Instance().GetDevice()->LoadCubeTextureFromFile( pf, *pdata, f, b, u, d, l, r ) )
		{
			m_TextureMap.insert( std::make_pair( Utility::HashCode(filename + static_cast<char>( pdata->m_eFormat+1 )), std::make_pair( pdata, 1 ) ) );
			return *pdata;
		}
		ASSERT(false);
		return *pdata;
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceManager::ReleaseTexture(Device::PixelFormat pf, const std::string& name)
	{
		TextureReferenceMap::iterator it = m_TextureMap.find( Utility::HashCode(name + static_cast<char>( pf+1 )) );
		if( it != m_TextureMap.end()  )
		{
			it->second.second--;
			if ( 0 == it->second.second )
			{
				//DEBUGLOG("Release %s\n", name.c_str() );
				Engine::Instance().GetDevice()->DeleteTexture( it->second.first->m_hTextureHandle );
				SAFE_DELETE( it->second.first );
				m_TextureMap.erase( it++ );
			}
			return;
		}
		//如果还是找不到
		ASSERT(false);
	}
	//------------------------------------------------------------------------------------------------------
	const ModelData& ResourceManager::LoadModel( const std::string& name )
	{
		ModelData* data;
		ModelsReferenceMap::iterator it = m_ModelMap.find( Utility::HashCode(name) );
		if ( it == m_ModelMap.end() )
		{
			data = NEW ModelData();
			data->LoadModel( name );
			m_ModelMap.insert( make_pair( Utility::HashCode(name), make_pair( data, 1 ) ) );
		}
		else
		{
			data = it->second.first;
		}
		return *data;
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceManager::ReleaseModel( const std::string& name )
	{
		ModelsReferenceMap::iterator it = m_ModelMap.find( Utility::HashCode(name) );
		if( m_ModelMap.end() != it )
		{
			it->second.second--;
			if( 0 == it->second.second )//释放
			{
				SAFE_DELETE( (it->second.first) );
				m_ModelMap.erase( it );
			}
		}
	}



	//------------------------------------------------------------------------------------------------------
	void ResourceManager::OnSourceLoaded( SmartPtr<ISource>& source )
	{
		//当数据在异步线程加载完毕后，会调用这个函数将数据push到队列中，等待在主线程制作成资源
		m_pMutex->Lock();
		m_LoadedSource.push( source );
		m_pMutex->Unlock();
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceManager::LoadResource( SmartPtr<ISource>& source )
	{
		m_pMutex->Lock();//这里也需要加锁，在异步线程可能调用这里，push新的资源进来
		IResourcePairMap::iterator res = m_IResourcePairMap.find( source->GetHashCode() );
		if ( m_IResourcePairMap.end() != res )//找到这个资源就直接通知拷贝,或者添加到等待队列中
		{
			if ( NULL != res->second->Resource )
			{
				res->second->Resource->Increment();//增加引用计数
				Event<bool,IResource*>::FunctionType* ptr = source->GetListener();
				res->second->User->Connect( ptr );
				ptr->Invoker( res->second->Resource );//通知客户资源已经准备好了
			}
			else//资源还在加载中
			{
				Event<bool,IResource*>::FunctionType* ptr = source->GetListener();
				res->second->User->Connect( ptr );
			}
		}
		else
		{
				//如果都没找到，就添加到加载队列中
				Event<bool,IResource*>* eventPtr = NEW Event<bool,IResource*>();
				eventPtr->Connect( source->GetListener() );
				ResourcePair* pair = NEW ResourcePair();
				pair->User = eventPtr;
				m_IResourcePairMap.insert( make_pair( source->GetHashCode(), pair ) );
				m_pLoadingThread->PushJob( source );
		}
		m_pMutex->Unlock();
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceManager::Update()
	{
		while( !m_LoadedSource.empty() )
		{
			m_pMutex->Lock();
			SmartPtr<ISource> ptr = m_LoadedSource.front();
			m_LoadedSource.pop();
			m_pMutex->Unlock();
			IResourcePairMap::iterator pair = m_IResourcePairMap.find( ptr->GetHashCode() );
			uint neederCount = pair->second->User->GetListenerCount();
			if ( 0 != neederCount )//如果还有需求者
			{
				IResource* res = ptr->DoMakeResource();//制作数据
				pair->second->Resource = res;
				pair->second->User->Multicast( res );//通知客户，资源已经准备完毕
				res->Increment( neederCount );//添加引用计数
			}
			else//没有需求就白加载了，直接删除资源
			{
				SAFE_DELETE( pair->second->User );//删除事件
				SAFE_DELETE( pair->second );//删除资源对
				m_IResourcePairMap.erase( pair );//删除资源队列中的数据
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceManager::UnloadResource( SmartPtr<ISource>& source )
	{
		IResourcePairMap::iterator pair = m_IResourcePairMap.find( source->GetHashCode() );
		if ( NULL != pair->second->Resource )//找到这个资源就直接减少引用计数
		{
			if( 0 == pair->second->Resource->Decrement() )//减少引用计数
			{
				pair->second->Resource->ReleaseResource();//释放资源
				SAFE_DELETE( pair->second->Resource );//删除资源
				SAFE_DELETE( pair->second->User );//删除事件
				SAFE_DELETE( pair->second );//删除资源对
				m_IResourcePairMap.erase( pair );//删除资源队列中的数据
			}
		}
		else
		{
			pair->second->User->Remove( source->GetListener() );//如果在加载队列中就去掉事件监听者
		}
	}

}