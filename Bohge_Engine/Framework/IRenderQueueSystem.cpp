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
#include "IRenderQueueSystem.h"
#include "RenderComponent.h"
#include "Framework.h"


using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( IRenderQueueSystem );
	SYSTEM_SINGLETON_IMPLEMENT( IRenderQueueSystem );
	//---------------------------------------------------------------------------------------------------------
	IRenderQueueSystem::IRenderQueueSystem( void )
		:m_pAttachedComponentMap(NULL),
		m_pToAttachedList(NULL),
		m_pToDettachedList(NULL),
		m_pToChangedList(NULL),
		m_pInsertZone(NULL),
		m_pRemoveZone(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	IRenderQueueSystem::~IRenderQueueSystem( void )
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void IRenderQueueSystem::RegistEvent( Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &IRenderQueueSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &IRenderQueueSystem::OnFrameworkExit ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void IRenderQueueSystem::OnFrameworkEntry( float time, BohgeEngine::Framework& framework )
	{
		m_pAttachedComponentMap = NEW RenderComponentMap;
		m_pToAttachedList = NEW RenderComponentList;
		m_pToDettachedList = NEW RenderComponentList;
		m_pToChangedList = NEW RenderComponentList;
		m_pInsertZone = NEW ZoneList;
		m_pRemoveZone = NEW ZoneList;
	}
	//---------------------------------------------------------------------------------------------------------
	void IRenderQueueSystem::OnFrameworkExit( float time, BohgeEngine::Framework& framework )
	{
		SAFE_DELETE( m_pAttachedComponentMap );
		SAFE_DELETE( m_pToAttachedList );
		SAFE_DELETE( m_pToDettachedList );
		SAFE_DELETE( m_pToChangedList );
		SAFE_DELETE( m_pRemoveZone );
		SAFE_DELETE( m_pInsertZone );
	}
	//---------------------------------------------------------------------------------------------------------
	void IRenderQueueSystem::UpdateSystem()
	{
		FOR_EACH( RenderComponentMap, it, *m_pAttachedComponentMap )
		{
			it->second->TryRecaculateBindBox( );
		}
		_DoInsertCullZone();
		_DoAttachRenderComponent();
		_DoRenderComponentBindChange();
		_DoDetachRenderComponent();
		_DoRemoveCullZone();
	}
	//---------------------------------------------------------------------------------------------------------
	void IRenderQueueSystem::_DoRenderComponentBindChange()
	{
		FOR_EACH( RenderComponentList, rc, *m_pToChangedList )
		{
			_OnRenderComponentBindChange( *rc );
		}
		m_pToChangedList->clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void IRenderQueueSystem::_DoAttachRenderComponent()
	{
		FOR_EACH( RenderComponentList, rc, *m_pToAttachedList )
		{
			ASSERT( m_pAttachedComponentMap->end() == m_pAttachedComponentMap->find( (*rc)->GetObjectID() ) );
			m_pAttachedComponentMap->insert( make_pair( (*rc)->GetObjectID(), (*rc) ) );
			_OnAttachRenderComponent( (*rc) );
		}
		m_pToAttachedList->clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void IRenderQueueSystem::_DoDetachRenderComponent()
	{
		FOR_EACH( RenderComponentList, rc, *m_pToDettachedList )
		{
			m_pAttachedComponentMap->erase( (*rc)->GetObjectID() );
			_OnDetachRenderComponent( (*rc) );
		}
		m_pToDettachedList->clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void IRenderQueueSystem::_DoInsertCullZone()
	{
		FOR_EACH( ZoneList, rc, *m_pInsertZone )
		{
			_OnInsertCullZone( *rc );
		}
		m_pInsertZone->clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void IRenderQueueSystem::_DoRemoveCullZone()
	{
		FOR_EACH( ZoneList, rc, *m_pRemoveZone )
		{
			_OnRemoveCullZone( *rc );
		}
		m_pRemoveZone->clear();
	}
}