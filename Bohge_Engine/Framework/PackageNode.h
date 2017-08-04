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
#include "PackageData.h"




#include <EASTL/map.h>
namespace BohgeEngine
{
	class PackageTree;
	class BOHGE_FRAMEWORK PackageNode : public Object
	{
		RTTI_TYPE_DECLEAR( PackageNode );
	private:
		Aabrectd			m_Rect;//总rect大小
		vector2d			m_Size;//总尺寸大小
		PackageTree* const	m_pHostTree;
		PackageNode*		m_pFather;	//父节点
		PackageNode*		m_pLeftSon;	//左子叶(面积小的节点)
		PackageNode*		m_pRightSon;//右子叶(面积大的节点)
		bool				m_isTempSlot;//插入一个
		bool				m_isUseing;//是否使用中
	public:
		PackageNode( PackageTree* t );
		virtual ~PackageNode(void);
	private:
		void _DoHorizontalSlice( const vector2d& size );//水平切割
		void _DoVerticalSlice( const vector2d& size );//垂直切割
		bool _TryReleaseChild();
		bool _DoReleaseNode();//释放该节点
	public:
		void Initialize( const Aabrectd& zone, PackageNode* father );
		const vector2d& InsertPackage( const vector2d& size );
		void ReleaseNode();//释放该节点
	private:
		BOHGE_FORCEINLINE bool _isEndNode()
		{
			return NULL == m_pLeftSon;
		}
		BOHGE_FORCEINLINE bool _isFreeNode()
		{
			return NULL == m_pLeftSon;
		}
	public:
		BOHGE_FORCEINLINE bool isCapable( const vector2d& size )//有能力包含
		{
			return ( m_Size.m_x >= size.m_x && m_Size.m_y >= size.m_y );
		}
		BOHGE_FORCEINLINE const vector2d& GetSize()
		{
			return m_Size;
		}
	};
}

