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
#include "ComponentProperty.h"
#include "NodeProperty.h"
#include "Object.hpp"


#include <EASTL/vector.h>

namespace BohgeEngine
{
	class BOHGE_FRAMEWORK Node : public Object
	{
		RTTI_TYPE_DECLEAR( Node )
	private:
		ComponentProperty::SubnodeMap		m_SubnodeMap;//子节点
		ComponentProperty::ComponentMap		m_Components;//保存组件的数组
		eastl::string		m_NodeName;
		AttributeDelegate*	m_pAttributeDelegateArray;//组建注册提供某些数据
		MessageDelegate*	m_pMessageDelegateArray;
		Node*				m_RootNode;//根节点
		ushort				m_NodeDepth;
	public:
		Node(void);
		virtual ~Node(void);
	private:
		void _OnRootNodeChange( );
		void _SetFather( Node* root );
		void _RecaculateDepth();//当父节点改变时，重新计算每个节点和每个节点的字节点的depth
	public:
		void InsertIntoManager( const eastl::string& name );//挂在到节点管理器中，让脚本查询
		void AttachNode( Node* node );//在当前节点上附上一个子节点
		void DetachNode( Node* node );//剥离当前节点的子节点;
		void AttachComponent( IComponent* com );//添加一个组建
		void ReleaseComponent( );//释放全部的组件
		void RegisterAttributeProvider( NodeProperty::GetAttribute na, ProviderFunction* func );//注册为一个数据原
		void RegisterMessageListener( NodeProperty::SetAttribute mt, MessageFunction* func );//注册事件监听
		void SendMessage( const NodeMessage& message );//向自己的组件和根节点广播消息
		const Object* RootAttribute( NodeProperty::GetAttribute na ) const;
		const Object* Attribute( NodeProperty::GetAttribute na ) const;//得到节点属性
		void SetAttribute( NodeProperty::SetAttribute mt, const Object& obj );//设置某个节点属性
		IComponent* FindComponent( const eastl::string& name );//效率低下，建议找到之后保存
	protected:
		BOHGE_FORCEINLINE ComponentProperty::SubnodeMap& _GetSubnodeMap()
		{
			return m_SubnodeMap;
		}
		BOHGE_FORCEINLINE Node* _GetRoot()
		{
			return m_RootNode;
		}
	public:
		template<typename T>
		BOHGE_FORCEINLINE const T* Attribute( NodeProperty::GetAttribute na )
		{
			return static_cast<const T*>( m_pAttributeDelegateArray[na].Unicast() );
		}
		BOHGE_FORCEINLINE ushort NodeDepth() const//该节点的迭代深度
		{
			return m_NodeDepth;
		}
		BOHGE_FORCEINLINE const eastl::string& GetName() const
		{
			return m_NodeName;
		}
	};
}