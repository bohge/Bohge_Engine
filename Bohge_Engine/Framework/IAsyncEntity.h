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
#include "IResourceSystem.h"
#include "ScriptCallback.h"
#include "SmartPtr.hpp"
#include "Delegate.hpp"
#include "Object.hpp"
#include "IJob.h"

#include <EASTL/string.h>

namespace BohgeEngine
{
	class IMetadata;
	class IAsyncSource;
	class IAsyncResource;
	template<typename RES, typename SOU>
	class IAsyncEntity : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( IAsyncEntity );
	private:
		Delegate1<void,const SmartPtr<IAsyncSource>& >*	m_pOnLoadedEvent;
		Delegate1<void,const SmartPtr<IAsyncSource>& >*	m_pOnPushedEvent;
		SmartPtr<SOU>					m_spSource;//CPU资源
		SmartPtr<IAsyncSource>			m_spRootPtr;
		RES*							m_pResource;//GPU资源
		uint							m_HashCode;
		bool							m_isPushIntoQueue;
		bool							m_isReady;
	private:
		ScriptCallbackList*				m_pOnLoadedCallback;
	public:
		//------------------------------------------------------------------------------------------------------
		IAsyncEntity( )
			:m_pResource(NULL),
			m_pOnLoadedCallback( NEW ScriptCallbackList ),
			m_pOnLoadedEvent( NEW Delegate1<void, const SmartPtr<IAsyncSource>& > ),
			m_pOnPushedEvent( NEW Delegate1<void, const SmartPtr<IAsyncSource>& > ),
			m_isReady(false),
			m_isPushIntoQueue(false),
			m_spSource( NEW SOU )
		{
			m_spRootPtr = SmartPtr_Static_Cast< IAsyncSource >( m_spSource );
			m_spSource->SetSelfReference( &m_spRootPtr );
			m_spSource->SetListener( MEMBER_FUNC_PTR(&IAsyncEntity::ResourceCallBack) );
		}
		//------------------------------------------------------------------------------------------------------
		IAsyncEntity( const IAsyncEntity& rhs )
		{
			_DoCopy(rhs);
		}
		//------------------------------------------------------------------------------------------------------
		virtual ~IAsyncEntity(void)
		{
			switch( m_spSource->GetSourceType() )
			{
			case IResourceProperty::CST_SHARED: IResourceSystem::Instance()->UnloadResource( m_spSource ); break;
			case IResourceProperty::CST_MONOPOLY:
				{
					m_pResource->ReleaseResource();
					if ( m_spSource->isHostingFree() )
					{
						SAFE_DELETE( m_pResource );//删除资源
					}
				}break;
			default: ASSERT(false);
			}
			SAFE_DELETE( m_pOnLoadedEvent );
			SAFE_DELETE( m_pOnPushedEvent );
			SAFE_DELETE( m_pOnLoadedCallback );
		}
	private:
		virtual void _OnCreateResource() = 0;//准备加载资源的时候调用
		virtual void _OnResourceCreated() = 0;//资源加载完毕的回掉
	private:
		//------------------------------------------------------------------------------------------------------
		void _DoCopy( const IAsyncEntity& rhs )
		{
			m_isReady = rhs.m_isReady;
			m_HashCode = rhs.m_HashCode;
			m_pResource = rhs.m_pResource;
			m_spSource = rhs.m_spSource;
		}
	protected:
		//------------------------------------------------------------------------------------------------------
		void _Recreate()//重新加载
		{
			ASSERT( NULL != m_pResource );//必须是加载后才能resize
			IResourceSystem::Instance()->UnloadResource( m_spSource );//释放旧的
			IResourceSystem::Instance()->LoadResource( m_spSource );//加载新的
		}
	public:
		//------------------------------------------------------------------------------------------------------
		const IAsyncEntity& operator = ( const IAsyncEntity& rhs )
		{
			_DoCopy( rhs );
			return *this;
		}
		//------------------------------------------------------------------------------------------------------
		void ResourceCallBack( IAsyncResource* res )//资源准备好后的回掉
		{
			m_HashCode = m_spSource->GetHashCode();
			m_pResource = static_cast<RES*>(res);
			m_isReady = true;//资源准备就绪
			_OnResourceCreated();
			m_pOnLoadedEvent->Multicast(m_spSource);
			m_pOnLoadedEvent->ClearConnect();//只有一次有效性
			SAFE_DELETE( m_pOnLoadedEvent );
			FOR_EACH( ScriptCallbackList, invoker, *m_pOnLoadedCallback )//脚本回调
			{
				(*invoker)->Invoke( this );
			}
			SAFE_DELETE( m_pOnLoadedCallback );
		}
		//------------------------------------------------------------------------------------------------------
		void CreateResource( )//开始加载资源
		{
			if ( !m_isPushIntoQueue )
			{
				m_isPushIntoQueue = true;
				_OnCreateResource();
				IResourceSystem::Instance()->LoadResource( m_spSource );
				m_pOnPushedEvent->Multicast(m_spSource);
				m_pOnPushedEvent->ClearConnect();//只有一次有效性
				SAFE_DELETE( m_pOnPushedEvent );
			}
		}
	protected:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE SmartPtr<SOU>& _GetSourcePtr()
		{
			return m_spSource;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const SmartPtr<SOU>& _GetSourcePtr() const
		{
			return m_spSource;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE RES* _GetResourcePtr()
		{
			return m_pResource;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const RES* _GetResourcePtr() const
		{
			return m_pResource;
		}
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetPriority( uint p )
		{
			m_spSource->SetPriority( p );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetPriority()
		{
			return m_spSource->GetPriority();
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetJobType( IJob::JobType jt )
		{
			m_spSource->SetJobType( jt );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE IJob::JobType GetJobType()
		{
			return m_spSource->GetJobType();
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void PushMetadata( const IMetadata& meta )
		{
			m_spSource->PushSourceMetadata( meta );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetHashCode() const
		{
			return m_HashCode;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isReady() const
		{
			return m_isReady;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void ConnectPushedEvent( Delegate1<void, const SmartPtr<IAsyncSource>& >::FunctionType* ptr )//只有一次有效性
		{
			m_pOnPushedEvent->Connect( ptr );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void ConnectLoadedEvent( Delegate1<void, const SmartPtr<IAsyncSource>& >::FunctionType* ptr )//只有一次有效性
		{
			m_pOnLoadedEvent->Connect( ptr );
		}
		//------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetScriptLoadedEvent( IFunctionInvoker* invoker )
		{
			m_pOnLoadedCallback->push_back( invoker );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void DependenceSource( IAsyncSource* source )
		{
			SetPriority( source->GetPriority() );//设置为同级权限优先加载
			SetJobType( source->GetJobType() );
			ConnectPushedEvent( source->DependencePushed() );//增加一个依赖项目
			ConnectLoadedEvent( source->DependenceLoaded() );//增加一个依赖项目
		}
	};
	//全部的IAsyncEntity都是同一个ID，做脚本的时候不做他们基础类
	template<typename RES, typename SOU> Rtti 
		IAsyncEntity<RES, SOU>::s_Rtti
		( sizeof(IAsyncEntity<RES, SOU>), NULL, "IAsyncEntity" );
}
