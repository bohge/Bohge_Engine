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
#include "IFunctionInvoker.h"
#include "IScriptResource.h"
#include "IScriptSystem.h"
#include "DoRegister.h"
#include "Framework.h"



using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( IScriptSystem );
	SYSTEM_SINGLETON_IMPLEMENT( IScriptSystem );
	//-------------------------------------------------------------------------------------------------------
	IScriptSystem::IScriptSystem(void)
		:m_pAttachList(NULL),
		m_pDetachList(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//-------------------------------------------------------------------------------------------------------
	IScriptSystem::~IScriptSystem(void)
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//-------------------------------------------------------------------------------------------------------
	IClassResource* IScriptSystem::LoadScript( ScriptProperty::ScriptThread st, const string& name )
	{
		IClassResource* sc = NULL;
		switch( st )
		{
		case ScriptProperty::ST_RENDER: sc = _DoLoadRenderScript( name ); break;
		case ScriptProperty::ST_LOGIC: sc = _DoLoadLogicScript( name ); break;
		default:ASSERT(false);
		}
		return sc;
	}
	//-------------------------------------------------------------------------------------------------------
	IScriptResource* IScriptSystem::ScriptFactory( ScriptProperty::ScriptThread st )
	{
		IScriptResource* sc = NULL;
		switch( st )
		{
		case ScriptProperty::ST_RENDER: sc = _DoRenderScriptFactory( ); break;
		case ScriptProperty::ST_LOGIC: sc = _DoLogicScriptFactory( ); break;
		default:ASSERT(false);
		}
		return sc;
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::RecycleBin( IScriptResource* sr )
	{
		switch( sr->GetScriptThread() )
		{
		case ScriptProperty::ST_RENDER:
			{
				sr->ReleaseAll();
				_DoRenderRecycleBin( sr );}
			break;
		case ScriptProperty::ST_LOGIC:
			{
				if ( m_pDetachList )//空说明系统已经退出，直接删除，这样会导致exit函数掉不到
				{
					m_pDetachList->push_back( sr );
				}
				else
				{
					sr->ReleaseAll();
					_DoLogicRecycleBin( sr );
				}
			}break;
		default:ASSERT(false);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::_DoAttach()
	{
		FOR_EACH( IScriptResourceList, itsc, *m_pAttachList )
		{
			uint hash = (*itsc)->GetScriptHash();
			if( m_pLoadedSrcipt->end() == m_pLoadedSrcipt->find(hash) );//确保叫本res只挂在一次
			{
				m_pLoadedSrcipt->insert( make_pair( hash, *itsc ) );
				{//调用初始化
					IFunctionInvoker* func = (*itsc)->SignFunciton( ScriptProperty::FUNC_INITIALIZE, 0 );
					if ( func )
					{
						func->Invoke();
					}
				}
				for ( int i = 0 ; i < ScriptProperty::SLE_COUNT ; ++ i )
				{
					IFunctionInvoker* func = (*itsc)->SignFunciton( ScriptProperty::FUNC_LOOPED[i], 0 );
					if ( func )
					{
						m_ScriptFunctionMap[i].insert( make_pair( hash, func ) );
					}
				}
			}
		}
		m_pAttachList->clear();
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::_DoDetach()
	{
		FOR_EACH( IScriptResourceList, itsc, *m_pDetachList )
		{
			uint hash = (*itsc)->GetScriptHash();
			ASSERT( m_pLoadedSrcipt->end() != m_pLoadedSrcipt->find(hash) );
			m_pLoadedSrcipt->erase( hash );
			{//调用初始化
				IFunctionInvoker* func = (*itsc)->SignFunciton( ScriptProperty::FUNC_EXIT, 0 );
				if ( func )
				{
					func->Invoke();
				}
			}
			for ( int i = 0 ; i < ScriptProperty::SLE_COUNT ; ++ i )
			{
				m_ScriptFunctionMap[i].erase( hash );
			}
			(*itsc)->ReleaseAll();//删除函数
			_DoLogicRecycleBin( *itsc );
		}
		m_pDetachList->clear();
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::_RegisterFramework()
	{
		DoRegister( *this );
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::RegistEvent( BohgeEngine::Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_ENTRY, MEMBER_FUNC_PTR( &IScriptSystem::OnFrameworkRenderEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &IScriptSystem::OnFrameworkLogicEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &IScriptSystem::OnFrameworkLogicExit ) );

		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_BEGIN_UPDATE, MEMBER_FUNC_PTR( &IScriptSystem::OnBeginUpdate ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_DO_UPDATE, MEMBER_FUNC_PTR( &IScriptSystem::OnDoUpdate ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_END_UPDATE, MEMBER_FUNC_PTR( &IScriptSystem::OnEndUpdate ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::OnFrameworkRenderEntry( float time, Framework& framework )//处理函数注册
	{
		SetGlobal( ScriptProperty::__GlobalFrameworkName, &framework );
		//const FrameworkEvent::IPluginList& plugins = framework.GetPluginList();
		//FOR_EACH_CONST( FrameworkEvent::IPluginList, plug, plugins )
		//{
		//	SetGlobal( plug->second->GetTypeName(), plug->second );
		//}
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::OnFrameworkLogicEntry( float time, Framework& framework )
	{
		m_pLoadedSrcipt = NEW IScriptResourceMap;
		m_pAttachList = NEW IScriptResourceList;
		m_pDetachList = NEW IScriptResourceList;
		SetGlobal( ScriptProperty::__GlobalFrameworkName, &framework );
		const FrameworkEvent::IPluginList& plugins = framework.GetPluginList();
		FOR_EACH_CONST( FrameworkEvent::IPluginList, plug, plugins )
		{
			if ( isRegisteredClass( plug->second->GetTypeName() ) )
			{
				SetGlobal( plug->second->GetTypeName(), plug->second );
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::OnFrameworkLogicExit( float time, Framework& framework )
	{
		_DoDetach();
		FOR_EACH( IScriptResourceMap, itsc, *m_pLoadedSrcipt )//执行脚本退出
		{
			IFunctionInvoker* func = itsc->second->SignFunciton( ScriptProperty::FUNC_EXIT, 0 );
			if ( func )
			{
				func->Invoke();
			}
		}
		SAFE_DELETE( m_pLoadedSrcipt );
		SAFE_DELETE( m_pAttachList );
		SAFE_DELETE( m_pDetachList );
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::OnBeginUpdate( float time, Framework& framework )
	{
		_DoAttach();
		_DoDetach();
		FOR_EACH( FunctionInvokerMap, func, m_ScriptFunctionMap[ScriptProperty::SLE_BEGIN_UPDATE] )
		{
			func->second->Invoke();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::OnDoUpdate( float time, Framework& framework )
	{
		FOR_EACH( FunctionInvokerMap, func, m_ScriptFunctionMap[ScriptProperty::SLE_DO_UPDATE] )
		{
			func->second->Invoke();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void IScriptSystem::OnEndUpdate( float time, Framework& framework )
	{
		FOR_EACH( FunctionInvokerMap, func, m_ScriptFunctionMap[ScriptProperty::SLE_END_UPDATE] )
		{
			func->second->Invoke();
		}
	}
}