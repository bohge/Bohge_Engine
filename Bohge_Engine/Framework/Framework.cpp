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
#include "IMemorySystem.h"
#include "IJobDispatcher.h"
#include "IThreadSystem.h"
#include "ITimeSystem.h"
#include "IMetadata.h"
#include "Framework.h"
#include "IIOSystem.h"
#include "Framework.h"



namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( Framework );
	SINGLETON_IMPLEMENT( Framework );
	//---------------------------------------------------------------------------------------------------------
	Framework::Framework( void )
		:m_hWindows(NULL),
		m_pNodeManager( NEW NodeManager )
	{

	}
	//---------------------------------------------------------------------------------------------------------
	Framework::~Framework( void )
	{
		SAFE_DELETE( m_pNodeManager );
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::PushPlugin( const eastl::string& name, IPlugin* plugin )
	{
		ASSERT( NULL == GetPlugin( name ) );
		m_IPluginList.push_back( eastl::make_pair( Utility::HashCode(name), plugin ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::SetWindowsHandle( void* h )
	{
		m_hWindows = h;
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::Lanuch()
	{
		m_spLogicJob = SmartPtr<LogicJob>( NEW LogicJob );
		FOR_EACH_CONST( FrameworkEvent::IPluginList, plugin, m_IPluginList )//注册插件事件
		{
			plugin->second->RegistEvent( *this );
		}
		m_eEventType = FrameworkEvent::ET_RENDER_ON_ENTRY;
		m_MainThreadDelegateArray[FrameworkEvent::ET_RENDER_ON_ENTRY-FrameworkEvent::ET_RENDER_ON_ENTRY].Multicast( 0.0f, *this );
		m_spLogicJob->OnLanuch( this );//初始化逻辑线程
		//启动逻辑线程
		m_pLogicUpdateThread = 
			static_cast< IJobDispatcher* >(
			IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_JOB_DISPATCHER ) );
		m_pLogicUpdateThread->SetSortType( IJobDispatcher::ST_UNSORT );
		m_pLogicUpdateThread->SetThreadCount(1);
		m_pLogicUpdateThread->Start();
		m_pLogicUpdateThread->PushJob( m_spLogicJob );
		while(!m_spLogicJob->isLiveing() );
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::Exit( )
	{
		Update();//手动再updata一下，走资源释放流程
		m_eEventType = FrameworkEvent::ET_RENDER_ON_EXIT;
		m_pLogicUpdateThread->Stop();
		IThreadSystem::Instance()->RecycleBin( m_pLogicUpdateThread );
		m_spLogicJob->OnExit();
		m_MainThreadDelegateArray[FrameworkEvent::ET_RENDER_ON_EXIT-FrameworkEvent::ET_RENDER_ON_ENTRY].RMulticast( 0.0f, *this );
		for ( int i = FrameworkEvent::ET_RENDER_ON_ENTRY - FrameworkEvent::ET_RENDER_ON_ENTRY;
			i < FrameworkEvent::GC_RENDER_DELEGATE_COUNT;
			i ++ )
		{
			m_MainThreadDelegateArray[i].ClearConnect();
		}
		m_spLogicJob = SmartPtr<LogicJob>(NULL);
		m_IPluginList.clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::Update()
	{
		float timespan = ITimeSystem::Instance()->GetFrameSpan();
		for ( int i = FrameworkEvent::ET_RENDER_BEGIN_RENDER - FrameworkEvent::ET_RENDER_ON_ENTRY;
			i < FrameworkEvent::GC_RENDER_DELEGATE_COUNT;
			i ++ )
		{
			m_eEventType = static_cast<FrameworkEvent::EventType>(i);
			m_MainThreadDelegateArray[i].Multicast( timespan, *this );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::SetMemoryAllocator( const IMemorySystem& mem )
	{
		Allocator::s_pAlloc = mem.GetAllocFunction();
		Allocator::s_pRelease = mem.GetReleaseFunction();
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::ResetMemoryAllocator()
	{
		Allocator::Reset();
	}
	//---------------------------------------------------------------------------------------------------------
	IPlugin* Framework::GetPlugin( const eastl::string& name )
	{
		uint hash = Utility::HashCode( name );
		FOR_EACH( FrameworkEvent::IPluginList, plugin, m_IPluginList )//注册插件事件
		{
			if ( plugin->first == hash )
			{
				return plugin->second;
			}
		}
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::RegisterEventListener( FrameworkEvent::EventType et, FrameworkDelegate::FunctionType* func )
	{
		switch( et )
		{
		case FrameworkEvent::ET_LOGIC_ON_ENTRY:
		case FrameworkEvent::ET_LOGIC_ON_EXIT:
		case FrameworkEvent::ET_LOGIC_BEGIN_UPDATE:
		case FrameworkEvent::ET_LOGIC_DO_UPDATE:
		case FrameworkEvent::ET_LOGIC_END_UPDATE: m_spLogicJob->RegisterEventListener( et, func ); break;

		case FrameworkEvent::ET_RENDER_ON_ENTRY:
		case FrameworkEvent::ET_RENDER_ON_EXIT:
		case FrameworkEvent::ET_RENDER_BEGIN_RENDER:
		case FrameworkEvent::ET_RENDER_DO_RENDER:
		case FrameworkEvent::ET_RENDER_END_RENDER:
		case FrameworkEvent::ET_RENDER_SWAP_RENDER_BUFFER: m_MainThreadDelegateArray[et-FrameworkEvent::ET_RENDER_ON_ENTRY].Connect( func ); break;
		default:ASSERT(false);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	const Config& Framework::GetConfig() const
	{
		return m_Config;
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::SetConfig( const Config& cfg )
	{
		m_Config = cfg;
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::SetLitePath( const eastl::string& path )
	{
		m_Config.SetLitePath( path );
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::Resizeview( const vector4d& view )
	{
		if ( NULL != m_hWindows )
		{
			ViewResolution vr;
			vr.m_OldResolution = m_Config.GetResolution();
			vr.m_OldViewSize = m_Config.GetViewSize();
			vector2f rsl = m_Config.GetResolution();
			const vector4f oldView = m_Config.GetViewSize();
			rsl /= vector2f( oldView.m_z - oldView.m_x, oldView.m_w - oldView.m_y );
			m_Config.SetViewSize( view );
			m_Config.SetResolution( vector2f( view.m_z - view.m_x , view.m_w - view.m_y ) *rsl );
			vr.m_NewResolution = m_Config.GetResolution();
			vr.m_NewViewSize = m_Config.GetViewSize();
			m_ResizeviewDelegate.Multicast( vr );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	FunctionPtr Framework::RegistResizeviewListener( Delegate1< void,const ViewResolution& >::FunctionType* func )
	{
		return m_ResizeviewDelegate.Connect( func );
	}
	//---------------------------------------------------------------------------------------------------------
	void Framework::RemoveResizeviewListener( FunctionPtr ptr )
	{
		m_ResizeviewDelegate.ClearConnect( ptr );
	}
	//---------------------------------------------------------------------------------------------------------
	Node* Framework::FindNode( const char* name )
	{
		return m_pNodeManager->FindNode( name );
	}

}