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
#include "Framework/RenderComponent.h"
#include "Framework/Object.hpp"

#include <EASTL/list.h>

namespace BohgeEngine{ class RenderComponent; }
namespace OCTreeRenderQueueSystemPlugin
{
	typedef eastl::list< BohgeEngine::RenderComponent* > RenderComponentList;
	class OCTree;
	class TreeIterator : public BohgeEngine::Object
	{
		RTTI_TYPE_DECLEAR( TreeIterator );
	private:
		OCTree*							m_rpTree;
		RenderComponentList*			m_rpList;
		RenderComponentList::iterator	m_Iterator;//自己在list中的索引
		ushort							m_ChangeTimes;//改变的次数
	public:
		TreeIterator(void):m_ChangeTimes(0),m_rpList(NULL),m_Iterator(NULL),m_rpTree(NULL){}
		virtual ~TreeIterator(void){}
	public:
		BOHGE_FORCEINLINE void SetListPtr( RenderComponentList* ptr )
		{
			m_rpList = ptr;
		}
		BOHGE_FORCEINLINE void SetIterator( RenderComponentList::iterator it )
		{
			m_Iterator = it;
		}
		BOHGE_FORCEINLINE void SetTree( OCTree* ptr )
		{
			m_rpTree = ptr;
		}
		BOHGE_FORCEINLINE OCTree* GetTree()
		{
			return m_rpTree;
		}
		BOHGE_FORCEINLINE RenderComponentList* GetListPtr()
		{
			return m_rpList;
		}
		BOHGE_FORCEINLINE RenderComponentList::iterator GetIterator()
		{
			return m_Iterator;
		}
		BOHGE_FORCEINLINE void Changed()
		{
			++ m_ChangeTimes;
		}
		BOHGE_FORCEINLINE ushort GetChangeTimes()
		{
			return m_ChangeTimes;
		}
	};
}