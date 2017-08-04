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
#include "NodeMessage.h"
#include "IComponent.h"
#include "Framework.h"
#include "Node.h"
#include "Log.h"


using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( Node );
	//-------------------------------------------------------------------------------------------------------
	Node::Node(void)
		:m_pAttributeDelegateArray(NULL),
		m_pMessageDelegateArray(NULL),
		m_RootNode(NULL),
		m_NodeDepth(0)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	Node::~Node(void)
	{
		if ( !m_NodeName.empty() )
		{
			Framework::Instance()->RemoveNode( this );
		}
		ASSERT( 0 == m_Components.size() );//保证继承的最上层方调用西沟
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::_SetFather( Node* root )
	{
		m_RootNode = root;
		_RecaculateDepth();
		FOR_EACH( ComponentProperty::ComponentMap, com, m_Components )
		{
			com->second->OnRootNodeChange();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::_RecaculateDepth() /*当父节点改变时，重新计算每个节点和每个节点的字节点的depth */
	{
		m_NodeDepth = NULL == m_RootNode ? 0 : 1 + m_RootNode->NodeDepth();
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, m_SubnodeMap )
		{
			subnode->second->_RecaculateDepth();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::_OnRootNodeChange( )
	{
		FOR_EACH( ComponentProperty::ComponentMap, com, m_Components )
		{
			com->second->OnRootNodeChange( );
		}
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, m_SubnodeMap )
		{
			subnode->second->_OnRootNodeChange( );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::InsertIntoManager( const eastl::string& name )
	{
		m_NodeName = name;
		Framework::Instance()->InsertNode( this );
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::AttachNode( Node* node ) /*在当前节点上附上?个子节点 */
	{
		if ( NULL != node
			&& m_SubnodeMap.end() == m_SubnodeMap.find( (intptr_t)node)
			&& m_RootNode != node )
		{
			if( NULL != node->m_RootNode )
			{
				node->m_RootNode->DetachNode( node );
			}
			node->_SetFather( this );
			m_SubnodeMap.insert( eastl::make_pair( (intptr_t)node, node ) );
		}
		else
		{
			DEBUGLOG( "Can't attach node object !! \n" );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::DetachNode( Node* node ) /*剥离当前节点的子节点 */
	{
		if ( m_SubnodeMap.end() !=  m_SubnodeMap.find( (intptr_t)node ) )
		{
			node->_SetFather( NULL );
			m_SubnodeMap.erase( (intptr_t)node );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::AttachComponent( IComponent* com )
	{
		com->OnAttachNode( this );
		m_Components.insert( make_pair( Utility::HashCode( com->GetTypeName() ), com ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::ReleaseComponent()
	{
		SAFE_DELETE_ARRAY( m_pAttributeDelegateArray );
		SAFE_DELETE_ARRAY( m_pMessageDelegateArray );
		FOR_EACH( ComponentProperty::ComponentMap, com, m_Components )
		{
			com->second->Release();
		}
		m_Components.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::RegisterAttributeProvider( NodeProperty::GetAttribute na, ProviderFunction* func )
	{
		if ( NULL == m_pAttributeDelegateArray )
		{
			m_pAttributeDelegateArray = NEW AttributeDelegate[NodeProperty::GA_COUNT];
		}
		m_pAttributeDelegateArray[na].Connect( func );
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::RegisterMessageListener( NodeProperty::SetAttribute mt, MessageFunction* func )
	{
		if ( NULL == m_pMessageDelegateArray )
		{
			m_pMessageDelegateArray = NEW MessageDelegate[NodeProperty::SA_COUNT];
		}
		m_pMessageDelegateArray[mt].Connect( func );
	}
	//-------------------------------------------------------------------------------------------------------
	const Object* Node::Attribute( NodeProperty::GetAttribute na ) const
	{
		return m_pAttributeDelegateArray[na].Unicast();
	}
	//-------------------------------------------------------------------------------------------------------
	const Object* Node::RootAttribute( NodeProperty::GetAttribute na ) const
	{
		return NULL == m_RootNode ? NULL : m_RootNode->Attribute( na );
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::SendMessage( const NodeMessage& message )
	{
		m_pMessageDelegateArray[message.GetMessageType()].Multicast( message );
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, m_SubnodeMap )
		{
			subnode->second->_OnRootNodeChange( );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void Node::SetAttribute( NodeProperty::SetAttribute mt, const Object& obj )
	{
		SendMessage( NodeMessage( NULL, &obj, mt ) );
	}
	//-------------------------------------------------------------------------------------------------------
	IComponent* Node::FindComponent( const string& name )
	{
		ComponentProperty::ComponentMap::iterator res =
			m_Components.find( Utility::HashCode( name ) );
		if ( m_Components.end() != res )
		{
			return res->second;
		}
		ASSERT( false );
		return NULL;
	}
}