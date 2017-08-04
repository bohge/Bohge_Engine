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
#include "PackageNode.h"
#include "PackageTree.h"




using namespace eastl;
namespace BohgeEngine
{

	RTTI_TYPE_IMPLEMENT( PackageNode );
	//------------------------------------------------------------------------------------------------------
	PackageNode::PackageNode( PackageTree* t )
		:m_pHostTree(t),
		m_pFather(NULL),
		m_pLeftSon(NULL),
		m_pRightSon(NULL),
		m_isTempSlot(false),
		m_isUseing(false)
	{
	}
	//------------------------------------------------------------------------------------------------------
	PackageNode::~PackageNode(void)
	{
		SAFE_DELETE( m_pLeftSon );
		SAFE_DELETE( m_pRightSon );		
	}
	//------------------------------------------------------------------------------------------------------
	void PackageNode::Initialize( const Aabrectd& zone, PackageNode* father )
	{
		m_Size = zone.RightTop() - zone.LeftBottom();
		m_Rect = zone;
		m_pFather = father;
		m_pHostTree->InsertFreeSlot( m_Size, this );
	}
	//------------------------------------------------------------------------------------------------------
	void PackageNode::_DoHorizontalSlice( const vector2d& size )
	{
		const vector2d& lb = m_Rect.LeftBottom();
		const vector2d& rt = m_Rect.RightTop();
		Aabrectd left( vector2d( lb.m_x + size.m_x, lb.m_y ), vector2d( rt.m_x, lb.m_y + size.m_y ) );
		Aabrectd right( vector2d( lb.m_x, lb.m_y + size.m_y ), rt );
		m_pLeftSon = NEW PackageNode(m_pHostTree);
		m_pRightSon = NEW PackageNode(m_pHostTree);

		m_pLeftSon->Initialize( left, this );
		m_pRightSon->Initialize( right, this );
	}
	//------------------------------------------------------------------------------------------------------
	void PackageNode::_DoVerticalSlice( const vector2d& size )
	{
		const vector2d& lb = m_Rect.LeftBottom();
		const vector2d& rt = m_Rect.RightTop();
		Aabrectd left( vector2d(lb.m_x, lb.m_y+size.m_y), vector2d( lb.m_x + size.m_x, rt.m_y ) );
		Aabrectd right( vector2d( lb.m_x + size.m_x, lb.m_y ), rt );
		m_pLeftSon = NEW PackageNode(m_pHostTree);
		m_pRightSon = NEW PackageNode(m_pHostTree);

		m_pLeftSon->Initialize( left, this );
		m_pRightSon->Initialize( right, this );
	}
	//------------------------------------------------------------------------------------------------------
	const vector2d& PackageNode::InsertPackage( const vector2d& size )
	{
		ASSERT( isCapable(size) );
		m_pHostTree->RemoveFreeSlot( m_Size, this );
		m_isUseing = true;
		if ( NULL == m_pLeftSon
			&& NULL == m_pRightSon )
		{
			m_Size = size;//当前slot的可用空间改变
			if ( m_Size.m_x > m_Size.m_y )//宽大于高，数值切割，保证更多的有效面积
			{
				_DoVerticalSlice( size );
			}
			else//高大于宽水平切歌
			{
				_DoHorizontalSlice( size );
			}
		}
		return m_Rect.LeftBottom();
	}
	//------------------------------------------------------------------------------------------------------
	bool PackageNode::_TryReleaseChild()
	{
		ASSERT( NULL != m_pLeftSon && NULL != m_pRightSon );
		if ( !m_isUseing && m_pLeftSon->_isFreeNode() && m_pRightSon->_isFreeNode() )//无人使用且，左右两个根节点都是空的，则合并
		{
			if ( m_isTempSlot )
			{
				m_isTempSlot = false;
				m_pHostTree->RemoveFreeSlot( m_Size, this );
			}
			m_Size = m_Rect.RightTop() - m_Rect.LeftBottom();//子节点被释放，修改尺寸
			m_pHostTree->RemoveFreeSlot( m_pLeftSon->GetSize(), m_pLeftSon );
			m_pHostTree->RemoveFreeSlot( m_pRightSon->GetSize(), m_pRightSon );
			m_pHostTree->InsertFreeSlot( m_Size, this );
			SAFE_DELETE( m_pLeftSon );
			SAFE_DELETE( m_pRightSon );
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------------------------------------------
	bool PackageNode::_DoReleaseNode()
	{
		if ( _TryReleaseChild() )
		{
			if( m_pFather )
			{
				m_pFather->_DoReleaseNode();
			}
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------------------------------------------
	void PackageNode::ReleaseNode()
	{
		ASSERT( !_isEndNode() );
		if ( !_DoReleaseNode() )
		{
			m_isUseing = false;
			m_isTempSlot = true;
			m_pHostTree->InsertFreeSlot( m_Size, this );//将自己插入自由节点队列中
		}
	}
}