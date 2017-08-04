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
#include "Framework/IResourceSystem.h"

#include <EASTL/string.h>
#include <EASTL/list.h>
#include <EASTL/map.h>

namespace BohgeEngine
{
	class IMonopolySource;
	class ISharedSource;
	class ISharedResource;
	class IThreadMutex;
	class IJobDispatcher;
}

namespace ResourceSystemPlugin
{
	class ResourceSystem : public BohgeEngine::IResourceSystem
	{
	private:
		typedef eastl::map< intptr_t, BohgeEngine::SmartPtr<BohgeEngine::ISharedSource> > SourceMap;
	private:
		struct SharedResourcePair
		{
			BohgeEngine::ISharedResource*	Resource;
			BohgeEngine::SmartPtr<BohgeEngine::ISharedSource>	m_RootSource;//唯一经历过自身加载的，可能有一些设置在加载过程中
			SourceMap						m_NeederMap;
			SourceMap						m_WaitingMap;
			bool							m_isSourceLoaded;
			SharedResourcePair():Resource(NULL),m_isSourceLoaded(false){}
		};
	private:
		typedef eastl::map< uint, SharedResourcePair* > IResourcePairMap;//资源的map
		typedef eastl::list< BohgeEngine::SmartPtr<BohgeEngine::IAsyncSource> > ISourceList;
		typedef eastl::list< SharedResourcePair* > ResourcePairList;
	private:
		IResourcePairMap				m_IResourcePairMap[BohgeEngine::IResourceProperty::SST_COUNT];
		ISourceList						m_SharedRenderLoadedSource;//已经加载完毕等待make的队列
		ISourceList						m_SharedLogicLoadedSource;//已经加载完毕等待make的队列
		ISourceList						m_MonopolyRenderLoadedSource;
		ISourceList						m_MonopolyLogicLoadedSource;
		ResourcePairList				m_RenderReleaseResource;//需要析构的资源
		ResourcePairList				m_LogicReleaseResource;//需要析构的资源
		BohgeEngine::IThreadMutex*		m_pMutex;
		BohgeEngine::IJobDispatcher*	m_pIJobDispatcher;//加载线程
	public:
		ResourceSystem(void);
		virtual ~ResourceSystem(void);
	private:
		void _ProcessSharedSource( ISourceList& que );
		void _ProcessMonopolySource( ISourceList& que );
		void _DoReleaseResource( ResourcePairList& que );
		
		void _LoadSharedSource( BohgeEngine::SmartPtr<BohgeEngine::ISharedSource> source );
		void _LoadMonoPolySource( BohgeEngine::SmartPtr<BohgeEngine::IMonopolySource> source );
		void _UnloadSharedSource( BohgeEngine::SmartPtr<BohgeEngine::ISharedSource> source );

		void _MonopolySourcrLoadedCallBacke( const BohgeEngine::SmartPtr<BohgeEngine::IMonopolySource>& source );
		void _SharedSourcrLoadedCallBacke( const BohgeEngine::SmartPtr<BohgeEngine::ISharedSource>& source );
	public://注册的事件函数
		void OnFrameworkEntry( float time, BohgeEngine::Framework& framework ); //注册系统类型rtti
		void OnFrameworkRenderExit( float time, BohgeEngine::Framework& framework );
		void OnFrameworkLogicExit( float time, BohgeEngine::Framework& framework );
		void OnFrameworkBeginUpdata( float time, BohgeEngine::Framework& framework );
		void OnFrameworkBeginRender( float time, BohgeEngine::Framework& framework );
	public:
		void OnSourceLoadedCallBack( const BohgeEngine::SmartPtr<BohgeEngine::IAsyncSource>& source );//当数据在异步线程加载完毕后，会调用这个函数将数据push到队列中，等待在主线程制作成资源
	public:
		virtual void RegistEvent( BohgeEngine::Framework& framework ); //注册事件
		virtual void LoadResource( BohgeEngine::SmartPtr<BohgeEngine::IAsyncSource> source );//加载资源
		virtual void UnloadResource( BohgeEngine::SmartPtr<BohgeEngine::IAsyncSource> source );//卸载资源
		virtual BohgeEngine::ILoader* LoaderFactory( BohgeEngine::IResourceProperty::SourceLoaderType slt );//生成资源加载器
		virtual void RecycleBin( BohgeEngine::ILoader* ptr );//销毁资源
	};

}