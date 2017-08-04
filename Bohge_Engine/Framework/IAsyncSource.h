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
#include "IResourceProperty.h"
#include "Delegate.hpp"
#include "IMetadata.h"
#include "IJob.h"


namespace BohgeEngine
{
	class IAsyncResource;
	class BOHGE_FRAMEWORK IAsyncSource : public IJob //资源的原始数据
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IAsyncSource, IJob )
	private:
		SmartPtr<IAsyncSource>*							m_pSelfReference;
		ISourceMetadataList*							m_pISourceMetadataList;
		Delegate1<void,const SmartPtr<IAsyncSource>& >*	m_pSourceLoadedCallBack;
		Delegate1<void,const SmartPtr<IAsyncSource>& >*	m_pSourceLoadedDelayDependsCallBack;//存在延迟加载的资源，但是原始资源已经加载完毕
		Delegate1<void,IAsyncResource*>::FunctionType*	m_FuncPtr;
		IResourceProperty::SourceType					m_eSourceType;
		IResourceProperty::DispatchThread				m_eDispatchThread;
		uint											m_nHashCode;
		uint											m_Depends;//这个资源对其他资源的依赖，默认为1，意思是对自己的依赖
		bool											m_HasDelayDepends;//存在延迟加载的资源
		bool											m_isHostingFree;//托管删除
	public:
		IAsyncSource( IResourceProperty::SourceType cst, IResourceProperty::DispatchThread dt, bool hf );
		IAsyncSource( const IAsyncSource& rhs );
		virtual ~IAsyncSource(void);
	private:
		virtual void _DoReleaseUnnecessaryDataAfterLoaded( ) = 0;
	private:
		virtual uint _GetResourceHashCode() const;//返回资源的哈希值
		virtual void _DoYieldJob();
	protected:
		void _DependenceReady( );//已经有一个依赖想准备完毕，尝试加入资源制作队列
	public:
		Delegate1<void,const SmartPtr<IAsyncSource>& >::FunctionType* DependencePushed();//增加一个依赖资源，返回接受这个以来资源加载完毕的回掉
		Delegate1<void,const SmartPtr<IAsyncSource>& >::FunctionType* DependenceLoaded();//增加一个依赖资源，返回接受这个以来资源加载完毕的回掉
		void OnDependencePushed( const SmartPtr<IAsyncSource>&  dep );//当依赖资源加载完毕后
		void OnDependenceLoaded( const SmartPtr<IAsyncSource>&  dep );//当依赖资源加载完毕后
	public:
		void ClearMetadata();
		void ReleaseUnnecessaryDataAfterLoaded();//资源被make后释放不需要的数据
	public:
		BOHGE_FORCEINLINE void SetSelfReference( SmartPtr<IAsyncSource>* self )
		{
			m_pSelfReference = self;
		}
		BOHGE_FORCEINLINE void SetDispatchThread( IResourceProperty::DispatchThread dt )
		{
			m_eDispatchThread = dt;
		}
		BOHGE_FORCEINLINE void SetListener( Delegate1<void,IAsyncResource*>::FunctionType* const ptr )
		{
			ASSERT( NULL == m_FuncPtr );
			m_FuncPtr = ptr;
		}
		BOHGE_FORCEINLINE void CastDoneEvent( IAsyncResource* res )
		{
			m_FuncPtr->Invoker( res );
		}
		//BOHGE_FORCEINLINE Delegate1<void,IAsyncResource*>::FunctionType* GetListener() const
		//{
		//	return m_FuncPtr;
		//}
		BOHGE_FORCEINLINE void SetLoadedCallBacke( Delegate1<void, const SmartPtr<IAsyncSource>& >::FunctionType* func )
		{
			m_pSourceLoadedCallBack->Connect( func );
		}
		BOHGE_FORCEINLINE void SetDelayDependLoadedCallBacke( Delegate1<void, const SmartPtr<IAsyncSource>& >::FunctionType* func )
		{
			m_pSourceLoadedDelayDependsCallBack->Connect( func );
		}
		BOHGE_FORCEINLINE IResourceProperty::SourceType GetSourceType() const
		{
			return m_eSourceType;
		}
		BOHGE_FORCEINLINE IResourceProperty::DispatchThread GetDispatchThread() const
		{
			return m_eDispatchThread;
		}
		BOHGE_FORCEINLINE void PushSourceMetadata( const IMetadata& meta )
		{
			m_pISourceMetadataList->push_back( static_cast<IMetadata*>( meta.Duplicate() ) );//放在这里是不想让NEW在外部调用
		}
		BOHGE_FORCEINLINE const ISourceMetadataList& GetSourceMetadata() const
		{
			return *m_pISourceMetadataList;
		}
		BOHGE_FORCEINLINE uint GetHashCode()
		{
			if ( 0 == m_nHashCode )
			{
				m_nHashCode = _GetResourceHashCode();
			}
			return m_nHashCode;
		}
		BOHGE_FORCEINLINE void HasDelayDepends()
		{
			m_HasDelayDepends = true;
		}
		BOHGE_FORCEINLINE bool isHostingFree()
		{
			return m_isHostingFree;
		}
	};
}
