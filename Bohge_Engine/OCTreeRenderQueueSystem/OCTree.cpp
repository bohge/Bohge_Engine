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
#include "OCTreeRenderQueueSystem.h"
#include "Framework/IInserter.h"
#include "Framework/Log.h"
#include "OCTree.h"


using namespace BohgeEngine;
namespace OCTreeRenderQueueSystemPlugin
{
	RTTI_TYPE_IMPLEMENT( OCTree );
	//---------------------------------------------------------------------------------------------------------
	OCTree::OCTree( ushort depth, const BohgeEngine::Aabbox3df& box )
		:m_BindBox(box),
		m_uDepth( depth ),
		m_hasChild(false)
	{
		memset( m_OCTreeArray, NULL, sizeof(m_OCTreeArray) );
	}
	//---------------------------------------------------------------------------------------------------------
	OCTree::~OCTree(void)
	{
		for ( int i = 0 ; i < TC_CHILD_NODE_SIZE ; ++ i )
		{
			SAFE_DELETE( m_OCTreeArray[i] );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTree::_InitializeChild()
	{
		ASSERT( !m_hasChild );
		m_hasChild = true;
		vector3f cen = m_BindBox.GetCenter();
		vector3f ulb = m_BindBox.GetPoint(Aabbox3df::PS_ULB);
		vector3f ulf = m_BindBox.GetPoint(Aabbox3df::PS_ULF);
		vector3f urb = m_BindBox.GetPoint(Aabbox3df::PS_URB);
		vector3f urf = m_BindBox.GetPoint(Aabbox3df::PS_URF);
		vector3f dlb = m_BindBox.GetPoint(Aabbox3df::PS_DLB);
		vector3f dlf = m_BindBox.GetPoint(Aabbox3df::PS_DLF);
		vector3f drb = m_BindBox.GetPoint(Aabbox3df::PS_DRB);
		vector3f drf = m_BindBox.GetPoint(Aabbox3df::PS_DRF);

		m_OCTreeArray[TC_UP_LEFT_BACK] = NEW OCTree( m_uDepth+1, Aabbox3df( ulb, cen ) );
		m_OCTreeArray[TC_UP_LEFT_FRONT] = NEW OCTree( m_uDepth+1, Aabbox3df( ulf, cen ) );
		m_OCTreeArray[TC_UP_RIGHT_BACK] = NEW OCTree( m_uDepth+1, Aabbox3df( urb, cen ) );
		m_OCTreeArray[TC_UP_RIGHT_FRONT] = NEW OCTree( m_uDepth+1, Aabbox3df( urf, cen ) );
		m_OCTreeArray[TC_DOWN_LEFT_BACK] = NEW OCTree( m_uDepth+1, Aabbox3df( dlb, cen ) );
		m_OCTreeArray[TC_DOWN_LEFT_FRONT] = NEW OCTree( m_uDepth+1, Aabbox3df( dlf, cen ) );
		m_OCTreeArray[TC_DOWN_RIGHT_BACK] = NEW OCTree( m_uDepth+1, Aabbox3df( drb, cen ) );
		m_OCTreeArray[TC_DOWN_RIGHT_FRONT] = NEW OCTree( m_uDepth+1, Aabbox3df( drf, cen ) );
	}
	//---------------------------------------------------------------------------------------------------------
	bool OCTree::_InsertChildOCTree( BohgeEngine::RenderComponent* rc )
	{
		for ( int i = 0 ; i < TC_CHILD_NODE_SIZE ; ++ i )
		{
			if( m_OCTreeArray[i]->InsertRenderComponent( rc ) )
			{
				return true;
			}
		}
		{//子节点都没法完全包含，就包含在jf数组中
			m_JustFits.push_front( rc );
			RenderComponentList::iterator it = m_JustFits.begin();
			TreeIterator* ti = rc->GetUserData<TreeIterator>();
			ti->SetTree( this );
			ti->SetListPtr( &m_JustFits );
			ti->SetIterator( it );
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	bool OCTree::InsertRenderComponent( BohgeEngine::RenderComponent* rc )
	{
		IntersectedType::Volume type = GetBindBox().IntersectType( rc->GetBindBox() );
		if ( IntersectedType::IT_NO != type )
		{
			if ( m_hasChild )
			{
				return _InsertChildOCTree( rc );
			}
			else
			{

				switch( type )
				{
				case IntersectedType::IT_NO:
				case IntersectedType::IT_PARTIAL: return false;//必须完全包围,遇到这两种情况都返回不属于该节点
				case IntersectedType::IT_FULL:
					{
						{
							m_RenderComponents.push_front( rc );
							RenderComponentList::iterator it = m_RenderComponents.begin();
							TreeIterator* ti = rc->GetUserData<TreeIterator>();
							ti->SetTree( this );
							ti->SetListPtr( &m_RenderComponents );
							ti->SetIterator( it );
						}
						//判断是否后超值
						if ( TC_MAX_LEAF_COUNT < m_RenderComponents.size() && m_uDepth < TC_MAX_DEPTH )
						{
							_InitializeChild();
							FOR_EACH( RenderComponentList, rcit, m_RenderComponents )
							{
								_InsertChildOCTree( *rcit );
							}
							m_RenderComponents.clear();
						}
						return true;
					}
				default:ASSERT(false);break;
				}
			}
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTree::RemoveRenderComponent( TreeIterator* tit )
	{
		tit->GetListPtr()->erase( tit->GetIterator() );
	}
	//---------------------------------------------------------------------------------------------------------
	void OCTree::Culling( BohgeEngine::IInserter* ins )
	{
		if ( isContainsAnyRenderComponentOrChild() )
		{
			IntersectedType::Volume type = ins->Intersect( GetBindBox() );
//#ifdef _DEBUG
//			switch( type )
//			{
//			case IntersectedType::IT_NO:
//				{
//					DEBUGLOG( "Node depth %d, IT_NO, m_JustFits has %d, m_RenderComponents has %d\r\n", m_uDepth, m_JustFits.size(), m_RenderComponents.size() );
//				}break;
//			case IntersectedType::IT_PARTIAL:
//				{
//					DEBUGLOG( "Node depth %d, IT_PARTIAL, m_JustFits has %d, m_RenderComponents has %d\r\n", m_uDepth, m_JustFits.size(), m_RenderComponents.size() );
//				}break;
//			case IntersectedType::IT_FULL:
//				{
//					DEBUGLOG( "Node depth %d, IT_FULL, m_JustFits has %d, m_RenderComponents has %d\r\n", m_uDepth, m_JustFits.size(), m_RenderComponents.size() );
//				}break;
//			default:ASSERT(false);break;
//			}
//#endif
			switch( type )
			{
			case IntersectedType::IT_NO:break;
			case IntersectedType::IT_PARTIAL:
				//{//注掉的意思这里把部分可见的也直接渲染不再进行判断，降低CPU消耗
				//	FOR_EACH( RenderComponentList, rcit, m_JustFits )
				//	{
				//		if ( IntersectedType::IT_NO != ins->Intersect( (*rcit)->GetBindBox() ) )
				//		{
				//			ins->Pushback( *rcit );
				//		}
				//	}

				//	if ( m_hasChild )
				//	{
				//		ASSERT( 0 == m_RenderComponents.size() );
				//		for ( int i = 0 ; i < TC_CHILD_NODE_SIZE ; ++ i )
				//		{
				//			m_OCTreeArray[i]->Culling( ins );
				//		}
				//	}
				//	else
				//	{
				//		FOR_EACH( RenderComponentList, rcit, m_RenderComponents )
				//		{
				//			if ( IntersectedType::IT_NO != ins->Intersect( (*rcit)->GetBindBox() ) )
				//			{
				//				ins->Pushback( *rcit );
				//			}
				//		}
				//	}
				//}break;
			case IntersectedType::IT_FULL:
				{
					FOR_EACH( RenderComponentList, rcit, m_JustFits )
					{
						if ( (*rcit)->isRenderProperty( RenderObjectProperty::RP_SHOW ) )
						{
							ins->Pushback( *rcit );
							(*rcit)->SetVisible( true );
						}
					}

					if ( m_hasChild )
					{
						ASSERT( 0 == m_RenderComponents.size() );
						for ( int i = 0 ; i < TC_CHILD_NODE_SIZE ; ++ i )
						{
							m_OCTreeArray[i]->Culling( ins );
						}
					}
					else
					{
						FOR_EACH( RenderComponentList, rcit, m_RenderComponents )
						{
							if ( (*rcit)->isRenderProperty( RenderObjectProperty::RP_SHOW ) )
							{
								ins->Pushback( *rcit );
								(*rcit)->SetVisible( true );
							}
						}
					}
				}break;
			default:ASSERT(false);break;
			}
		}
	}
}
