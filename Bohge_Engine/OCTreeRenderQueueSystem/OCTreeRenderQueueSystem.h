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
#include "Framework/IRenderQueueSystem.h"


#include <EASTL/list.h>

namespace OCTreeRenderQueueSystemPlugin
{
	class OCTree;
	class OCTreeRenderQueueSystem : public BohgeEngine::IRenderQueueSystem
	{
	private:
		enum Constant
		{
			OC_MAX_CHANGE_TIMES = 2,//静态物体理论不会触发change，为了保险，这里还是设置成2吧
		};
	private:
		typedef eastl::map< intptr_t, BohgeEngine::RenderComponent* > RenderComponentMap;
		typedef eastl::list< eastl::pair< BohgeEngine::Aabbox3df, OCTree* > > OCTreeList;
	private:
		OCTreeList*						m_pOCTreeList;
		RenderComponentMap*				m_pRenderMap;
	public:
		OCTreeRenderQueueSystem(void);
		virtual ~OCTreeRenderQueueSystem(void);
	private:
		virtual void _OnAttachRenderComponent( BohgeEngine::RenderComponent* rc );
		virtual void _OnDetachRenderComponent( BohgeEngine::RenderComponent* rc );
		virtual void _OnRenderComponentBindChange( BohgeEngine::RenderComponent* rc );
		virtual void _OnInsertCullZone( const BohgeEngine::Aabbox3df& zone );
		virtual void _OnRemoveCullZone( const BohgeEngine::Aabbox3df& zone );
	public:
		virtual void RegistEvent( BohgeEngine::Framework& framework ); //注册事件
		void OnFrameworkEntry( float time, BohgeEngine::Framework& framework );
		void OnFrameworkExit( float time, BohgeEngine::Framework& framework );
	public:
		virtual void Culling( BohgeEngine::IInserter* ins );//计算裁剪
	};
}