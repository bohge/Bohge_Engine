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
#include "Framework/3DMath.h"
#include "TreeIterator.h"


namespace BohgeEngine{ class RenderComponent; }
namespace OCTreeRenderQueueSystemPlugin
{
	class TreeNode;
	class OCTreeRenderQueueSystem;
	class OCTree : public BohgeEngine::Object
	{
		RTTI_TYPE_DECLEAR( OCTree );
	private:
		enum Constant
		{
			TC_MAX_LEAF_COUNT = 5,//每个分支最大的子叶数量
			TC_MAX_DEPTH = 5,//最大迭代深度

			TC_CHILD_NODE_SIZE = 8,

			TC_UP_LEFT_FRONT = 0,
			TC_UP_LEFT_BACK,
			TC_UP_RIGHT_FRONT,
			TC_UP_RIGHT_BACK,
			TC_DOWN_LEFT_FRONT,
			TC_DOWN_LEFT_BACK,
			TC_DOWN_RIGHT_FRONT,
			TC_DOWN_RIGHT_BACK,
		};
	private:
		OCTree*							m_OCTreeArray[TC_CHILD_NODE_SIZE];
		const BohgeEngine::Aabbox3df	m_BindBox;//包围盒
		RenderComponentList				m_RenderComponents;//节点上的成员
		RenderComponentList				m_JustFits;//只在这个节点合适，再向下迭代就不是完全包围，这种节点值可以放在这个子节点中
		bool							m_hasChild;//是否有子节点
		const ushort					m_uDepth;//节点深度
	public:
		OCTree( ushort depth, const BohgeEngine::Aabbox3df& box );
		~OCTree(void);
	private:
		void _InitializeChild();//初始化子节点
		bool _InsertChildOCTree( BohgeEngine::RenderComponent* rc );//更新
	public:
		bool InsertRenderComponent( BohgeEngine::RenderComponent* rc );//插入一个渲染体
		void RemoveRenderComponent( TreeIterator* tit );//移除
		void Culling( BohgeEngine::IInserter* ins );
	public:
		BOHGE_FORCEINLINE const BohgeEngine::Aabbox3df& GetBindBox() const
		{
			return m_BindBox;
		}
		BOHGE_FORCEINLINE bool isContainsAnyRenderComponentOrChild() const
		{
			return m_RenderComponents.size() | m_JustFits.size() | m_hasChild;
		}
	};
}