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
#include "FrameworkEvent.h"
#include "NodeManager.h"
#include "Singleton.hpp"
#include "Delegate.hpp"
#include "Utility.hpp"
#include "Object.hpp"
#include "LogicJob.h"
#include "Config.h"

#include <EASTL/map.h>

namespace BohgeEngine
{
	class ICamera;
	class IPlugin;
	class ILiteapp;
	class IComponent;
	class IMemorySystem;
	class IJobDispatcher;
	class BOHGE_FRAMEWORK Framework : public Object
	{
		RTTI_TYPE_DECLEAR( Framework );
		SINGLETON_DECLARE( Framework );
	private:
		Config						m_Config;
		void*						m_hWindows;
		Delegate1< void,const ViewResolution& >		m_ResizeviewDelegate;
	private:
		FrameworkEvent::IPluginList		m_IPluginList;//保存全部的插件
		FrameworkDelegate				m_MainThreadDelegateArray[FrameworkEvent::GC_RENDER_DELEGATE_COUNT];//主线程事件队列
		FrameworkEvent::EventType		m_eEventType;
		IJobDispatcher*					m_pLogicUpdateThread;
		NodeManager*					m_pNodeManager;
		SmartPtr<LogicJob>				m_spLogicJob;
	private:
		Framework(void);
		~Framework(void);
	public:
		void SetWindowsHandle( void* h );
		void PushPlugin( const eastl::string& name, IPlugin* plugin );
		void Lanuch();//启动系统
		void Exit( );//销毁系统
		void Update( );
	public:
		void SetMemoryAllocator( const IMemorySystem& mem );//设置内存分配器
		void ResetMemoryAllocator();//充值内存分配器为默认分配器
		void RegisterEventListener( FrameworkEvent::EventType et, FrameworkDelegate::FunctionType* func );
		void Resizeview( const vector4d& view );
		FunctionPtr RegistResizeviewListener( Delegate1< void,const ViewResolution& >::FunctionType* func );
		void RemoveResizeviewListener( FunctionPtr ptr );
		void SetConfig( const Config& cfg );
		void SetLitePath( const eastl::string& path );
		const Config& GetConfig() const;
	public://脚本可以调用的
		IPlugin* GetPlugin( const eastl::string& name );
		Node* FindNode( const char* name );
	public:
		//---------------------------------------------------------------------------------------------------------
		void InsertNode( Node* node )
		{
			m_pNodeManager->InsertNode( node );
		}
		//---------------------------------------------------------------------------------------------------------
		void RemoveNode( const Node* node )
		{
			m_pNodeManager->RemoveNode( node );
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void* GetWindows()
		{
			return m_hWindows;
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetVersion()
		{
			return 1;
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const FrameworkEvent::IPluginList& GetPluginList() const
		{
			return m_IPluginList;
		}
	};
}