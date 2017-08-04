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
#include "Framework/RenderComponent.h"
#include "Framework/IPipelineSystem.h"
#include "OCTreeRenderQueueSystem.h"
#include "Framework/Framework.h"
#include "TreeIterator.h"
#include "OCTree.h"


using namespace eastl;
using namespace BohgeEngine;
namespace OCTreeRenderQueueSystemPlugin
{
	//---------------------------------------------------------------------------------------------------------
	OCTreeRenderQueueSystem::OCTreeRenderQueueSystem(void)
		:m_pOCTreeList(NULL),
		m_pRenderMap(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	OCTreeRenderQueueSystem::~OCTreeRenderQueueSystem(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTreeRenderQueueSystem::Culling( BohgeEngine::IInserter* ins )
	{
		FOR_EACH(OCTreeList, it, *m_pOCTreeList)
		{
			it->second->Culling( ins );
		}
		FOR_EACH(RenderComponentMap, it, *m_pRenderMap)
		{
			if ( it->second->isRenderProperty( RenderObjectProperty::RP_SHOW ) )
			{
				if ( IntersectedType::IT_NO != ins->Intersect( it->second->GetBindBox() ) )
				{
					ins->Pushback( it->second );
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTreeRenderQueueSystem::_OnRenderComponentBindChange( BohgeEngine::RenderComponent* rc )
	{
		TreeIterator* ptr = rc->GetUserData<TreeIterator>();
		if ( NULL != ptr )
		{
			ptr->GetTree()->RemoveRenderComponent( ptr );
			if ( OC_MAX_CHANGE_TIMES == ptr->GetChangeTimes() )
			{
				rc->SetUserData( NULL );
				SAFE_DELETE( ptr );
				m_pRenderMap->insert( make_pair( rc->GetObjectID(), rc ) );
			}
			else
			{
				FOR_EACH(OCTreeList, it, *m_pOCTreeList)
				{
					if ( it->second->InsertRenderComponent(rc) )
					{
						break;
					}
				}
				ptr->Changed();
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTreeRenderQueueSystem::_OnAttachRenderComponent( BohgeEngine::RenderComponent* rc )
	{
		rc->SetUserData( NEW TreeIterator() );
		FOR_EACH(OCTreeList, it, *m_pOCTreeList)
		{
			if ( it->second->InsertRenderComponent(rc) )
			{
				break;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTreeRenderQueueSystem::_OnDetachRenderComponent( BohgeEngine::RenderComponent* rc )
	{
		TreeIterator* ptr = rc->GetUserData<TreeIterator>();
		if ( NULL != ptr )
		{
			ptr->GetTree()->RemoveRenderComponent( ptr );
			rc->SetUserData( NULL );
			SAFE_DELETE( ptr );
		}
		else
		{
			m_pRenderMap->erase( rc->GetObjectID() );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTreeRenderQueueSystem::RegistEvent( Framework& framework )
	{
		IRenderQueueSystem::RegistEvent( framework );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &OCTreeRenderQueueSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &OCTreeRenderQueueSystem::OnFrameworkExit ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTreeRenderQueueSystem::OnFrameworkEntry( float time, BohgeEngine::Framework& framework )
	{
		m_pOCTreeList = NEW OCTreeList;
		m_pRenderMap = NEW RenderComponentMap;
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTreeRenderQueueSystem::OnFrameworkExit( float time, BohgeEngine::Framework& framework )
	{
		SAFE_DELETE( m_pRenderMap );
		SAFE_DELETE( m_pOCTreeList );
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTreeRenderQueueSystem::_OnInsertCullZone( const BohgeEngine::Aabbox3df& zone )
	{
		m_pOCTreeList->push_back( make_pair( zone, NEW OCTree( 0, zone ) ));
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTreeRenderQueueSystem::_OnRemoveCullZone( const BohgeEngine::Aabbox3df& zone )
	{
		FOR_EACH(OCTreeList, it, *m_pOCTreeList)
		{
			if ( zone == it->first )
			{
				SAFE_DELETE( it->second );
				m_pOCTreeList->erase(it);
				return;
			}
		}
		ASSERT(false);
	}
}