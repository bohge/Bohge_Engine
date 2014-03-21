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



/////////////////////////
//		可绘制节点	   //
/////////////////////////
#pragma once
#include "Log.h"
#include "Device.h"
#include "Engine.h"
#include "aabbox.hpp"
#include "Frustum.hpp"
#include "SceneManager.h"



namespace BohgeEngine
{
	//---------------------------------------------------------------
	class Engine;
	class SharedBuffer;
	class RendBuffer;




	class NodeType
	{
	public:
		enum Node
		{
			NT_ATTRIBUTENODE,
			NT_SCENENODE,
		};
	private:
		Node		m_NodeType;
	protected:
		void _SetNodeType( Node t )
		{
			m_NodeType = t;
		}
	public:
		virtual ~NodeType(){}
	public:
		const Node& GetNodeType()
		{
			return m_NodeType;
		}
	};

	class IAttributeNode : public NodeType//一个带有属性的节点
	{
		friend class SceneManager;
	private:
		typedef std::map< int, IAttributeNode* >	SonNodeMap;
	public:
		enum Node_Attribute
		{
			ATT_WORLDTRANSFORM, //缩放，旋转，平移的一个矩阵
			ATT_WORLDTRANSFORM_INVERS, //缩放，旋转，平移的一个矩阵的逆矩阵
			ATT_WORLDPOSITION,
			ATT_WORLDSCALE,
			ATT_WORLDROTATION,
			ATT_NORMALTRANSFORM,//法线的矩阵，只有旋转
			ATT_ANIMATION_REAL,
			ATT_ANIMATION_DUAL,

			ATT_USER_0,
			ATT_USER_1,
			ATT_USER_2,
			ATT_USER_3,
			ATT_USER_4,
			ATT_USER_5,
			ATT_USER_6,
			ATT_USER_7,
			ATT_USER_8,
			ATT_USER_9,
		};
	private:
		bool					m_isFatherChange;//当父节点改变，通知到子节点的flag
		ushort					m_NodeDepth;
		IAttributeNode*			m_FatherNode;
		SonNodeMap				m_SonNodes;
	public:
		explicit IAttributeNode()
			:m_FatherNode( NULL ),
			m_isFatherChange(false),
			m_NodeDepth(0)
		{
			_SetNodeType( NodeType::NT_ATTRIBUTENODE );
			Engine::Instance().GetSceneManager()->AttachNode(this);
		}
		virtual ~IAttributeNode()
		{
			if ( NULL != m_FatherNode )//如果父节点不为空，通知父节点清除自己
			{
				m_FatherNode->DetachNode( this );
			}
			for ( SonNodeMap::iterator it = m_SonNodes.begin();//通知子类，你没有父节点了
				it != m_SonNodes.end();
				it ++ )
			{
				it->second->_SetFather( NULL );//将子节点的父节点设置为NULL
				//SAFE_DELETE( it->second );//或者直接删除，2选1
			}
			m_SonNodes.clear();
			if( NULL != Engine::Instance().GetSceneManager() )//最后析构顺序可能导致为空，这里检查
			{
				Engine::Instance().GetSceneManager()->RemoveNode(this);
			}
		}
	private:
		BOHGE_FORCEINLINE void _SetFather( IAttributeNode* root )
		{
			m_isFatherChange = true;
			m_FatherNode = root;
			_RecaculateDepth();
		}
		void _RecaculateDepth()//当父节点改变时，重新计算每个节点和每个节点的字节点的depth
		{
			m_NodeDepth = NULL == m_FatherNode ? 0 : 1 + m_FatherNode->NodeDepth();
			for ( SonNodeMap::iterator it = m_SonNodes.begin();
				it != m_SonNodes.end();
				it ++ )
			{
				it->second->_RecaculateDepth();
			}
		}
	public:
		BOHGE_FORCEINLINE void AttachNode( IAttributeNode* node )//在当前节点上附上一个子节点
		{
			if ( NULL != node && m_SonNodes.end() == m_SonNodes.find( (int)node) && m_FatherNode != node )
			{
				if( NULL != node->m_FatherNode )
				{
					node->m_FatherNode->DetachNode( node );
				}
				node->_SetFather( this );
				m_SonNodes.insert( std::make_pair( (int)node, node ) );
			}
		}
		BOHGE_FORCEINLINE void DetachNode( IAttributeNode* node )//剥离当前节点的子节点
		{
			if ( m_SonNodes.end() !=  m_SonNodes.find( (int)node ) )
			{
				node->_SetFather( NULL );
				m_SonNodes.erase( (int)node );
			}
		}
		BOHGE_FORCEINLINE ushort NodeDepth() const//计算该节点的迭代深度
		{
			return m_NodeDepth;
		}
		BOHGE_FORCEINLINE const void* FatherAttribute( const Node_Attribute& name ) const
		{
			return NULL == m_FatherNode ? NULL : m_FatherNode->Attribute( name );
		}
		BOHGE_FORCEINLINE void CastChangedMessage()//通知自己和子节点们，父节点改变了
		{
			for ( SonNodeMap::iterator it = m_SonNodes.begin();
				it != m_SonNodes.end();
				it ++ )
			{
				it->second->m_isFatherChange = true;
			}
		}
		BOHGE_FORCEINLINE bool isFatherChange() const//父节点是否更换
		{
			return m_isFatherChange;
		}
		BOHGE_FORCEINLINE void SetFatherChanged()//设置被改变旗帜
		{
			m_isFatherChange = true;
		}
		BOHGE_FORCEINLINE void ResetFatherChanged()
		{
			m_isFatherChange = false;
		}
		BOHGE_FORCEINLINE int SonCount() //子节点的数量
		{
			return m_SonNodes.size();
		}
	public:
		virtual const void* Attribute( const Node_Attribute& name ) const
		{
			return NULL;
		}
		virtual IAttributeNode* GetNodeByIndex( int index )//得到字节点，针对模型，得到骨骼节点，其他一般返回this
		{
			return this;
		}
	};





	class ISceneNode : public IAttributeNode//可以自绘制得节点
	{
	public:
		enum RenderStatus
		{
			RS_SHOW			= 1 << 0,
			RS_NOCULL		= 1 << 1,
			RS_SHADOW		= 1 << 2,//阴影
			RS_TRANSPARENT	= 1 << 3,//是否透明
		};
	private:
		bool			m_isAttached;
		int				m_RendStatus;
	protected:
		aabbox3df		m_BindBox;//包围盒必须是世界坐标下的绑定盒
	public:
		explicit ISceneNode( )
			:IAttributeNode(),
			m_RendStatus(RS_SHOW)
		{
			_SetNodeType( NodeType::NT_SCENENODE );
			m_isAttached = true;
			Engine::Instance().GetSceneManager()->AttachObject(this);
		}
		explicit ISceneNode( int rs )
			:IAttributeNode(),
			m_RendStatus( RS_SHOW | rs )		
		{
			_SetNodeType( NodeType::NT_SCENENODE );
			m_isAttached = true;
			Engine::Instance().GetSceneManager()->AttachObject(this);
		}
		virtual ~ISceneNode()
		{
			if ( m_isAttached && NULL != Engine::Instance().GetSceneManager() )//最后析构顺序可能导致为空，这里检查
			{
				m_isAttached = false;
				Engine::Instance().GetSceneManager()->RemoveObject(this);
			}
		};
	public:
		virtual void BeginUpdate( Engine& engine ){};
		virtual void Update( uint milisecon ){};
		virtual void EndUpdate( Engine& engine ){};
		virtual bool isInview( const Frustumf& cFru )//判断当前帧是否可见是否需要绘制
		{
			return IntersectedType::IT_NO != cFru.Intersect( m_BindBox );
		};
		virtual void BeginRender( Engine& engine ){};
		virtual void Render( Engine& engine ){};
		virtual void EndRender( Engine& engine ){};
	public:
#ifdef DRAWAABBOX
		BOHGE_FORCEINLINE void RenderBind( Engine& dirver )
		{
			m_BindBox.Render( dirver );
		}
#endif
		BOHGE_FORCEINLINE void AttachObject()//挂在场景管理器上，绘制
		{
			if ( !m_isAttached )
			{
				m_isAttached = true;
				Engine::Instance().GetSceneManager()->AttachObject(this);
			}		
		}
		BOHGE_FORCEINLINE void DetachObject()//从场景管理器剥离
		{
			if ( m_isAttached )
			{
				m_isAttached = false;
				Engine::Instance().GetSceneManager()->RemoveObject(this);
			}		
		}
		BOHGE_FORCEINLINE void SetRender(bool isr)
		{
			if ( isr )
			{
				SetStatus( RS_SHOW );
			}
			else
			{
				EraseStatus( RS_SHOW );
			}
		}
		BOHGE_FORCEINLINE bool isShow() const
		{
			return isStatus(RS_SHOW);
		}
		BOHGE_FORCEINLINE const aabbox3df& WorldBindBox() const
		{
			return m_BindBox;
		}
		BOHGE_FORCEINLINE bool isStatus( RenderStatus r ) const
		{
			return ( m_RendStatus & r ) != 0;
		}
		BOHGE_FORCEINLINE void SetStatus( RenderStatus r )
		{
			m_RendStatus |= r;
		}
		BOHGE_FORCEINLINE void EraseStatus( RenderStatus r )
		{
			m_RendStatus &= (~r);
		}
	};



	//下面这两个属性停止使用了，全部放到IRenderNode中，作为一个Status
	////---------------------------------------------------------------
	//class IShadowable	//可以产生阴影的node
	//{
	//public:
	//	virtual ~IShadowable(){};
	//	virtual RendBuffer& GetMashEdgeBuffer() const = 0;
	//	virtual Matrix44f& GetTransform() const = 0;
	//};
	//
	////---------------------------------------------------------------
	//class IBeShadowable	//可以被阴影影响的物体
	//{
	//public:
	//	virtual ~IBeShadowable(){};
	//	virtual RendBuffer& GetRenderBuffer() const = 0;
	//	virtual Matrix44f& GetTransform() const = 0;
	//};

	//---------------------------------------------------------------
	class IGetBuffer	//可以读取顶点的数据的节点
	{
	public:
		virtual ~IGetBuffer(){};
		virtual void GetVertexBuffer(SharedBuffer& input) const = 0 ;
	};
	//---------------------------------------------------------------
	class IInstanceable
	{
	public:
		virtual ~IInstanceable(){}
		virtual void GetTransform(SharedBuffer& input) const = 0;
	};

}