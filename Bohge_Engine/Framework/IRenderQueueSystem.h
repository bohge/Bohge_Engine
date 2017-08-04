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
#include "SystemSingleton.h"
#include "IPlugin.h"
#include "3DMath.h"


#include <EASTL/map.h>


namespace BohgeEngine
{
	class IInserter;
	class RenderComponent;
	class BOHGE_FRAMEWORK IRenderQueueSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IRenderQueueSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IRenderQueueSystem, IPlugin );
	private:
		typedef eastl::map< intptr_t, RenderComponent* > RenderComponentMap;
		typedef eastl::list< RenderComponent* > RenderComponentList;
		typedef eastl::list< Aabbox3df > ZoneList;
	private:
		RenderComponentMap*			m_pAttachedComponentMap;
		RenderComponentList*		m_pToAttachedList;
		RenderComponentList*		m_pToDettachedList;
		RenderComponentList*		m_pToChangedList;
		ZoneList*					m_pInsertZone;
		ZoneList*					m_pRemoveZone;
	public:
		IRenderQueueSystem(void);
		virtual ~IRenderQueueSystem(void);
	private:
		virtual void _OnRenderComponentBindChange( RenderComponent* rc ) = 0;
		virtual void _OnAttachRenderComponent( RenderComponent* rc ) = 0;
		virtual void _OnDetachRenderComponent( RenderComponent* rc ) = 0;
		virtual void _OnInsertCullZone( const Aabbox3df& zone ) = 0;
		virtual void _OnRemoveCullZone( const Aabbox3df& zone ) = 0;
	public:
		virtual void Culling( IInserter* ins ) = 0;//计算裁剪
	private:
		void _DoInsertCullZone();
		void _DoRemoveCullZone();
		void _DoRenderComponentBindChange();
		void _DoAttachRenderComponent();
		void _DoDetachRenderComponent();
	public:
		virtual void RegistEvent( Framework& framework ); //注册事件
		void OnFrameworkEntry( float time, Framework& framework );
		void OnFrameworkExit( float time, Framework& framework );
	public:
		void UpdateSystem();//更新
	public:
		BOHGE_FORCEINLINE void AttachRenderComponent( RenderComponent* rc )
		{
			m_pToAttachedList->push_back( rc );
		}
		BOHGE_FORCEINLINE void DetachRenderComponent( RenderComponent* rc )
		{
			m_pToDettachedList->push_back( rc );
		}
		BOHGE_FORCEINLINE void RenderComponentBindChange( RenderComponent* rc )
		{
			m_pToChangedList->push_back( rc );
		}
		BOHGE_FORCEINLINE void InsertCullZone( const Aabbox3df& zone )
		{
			m_pInsertZone->push_back( zone );
		}
		BOHGE_FORCEINLINE void RemoveCullZone( const Aabbox3df& zone )
		{
			m_pRemoveZone->push_back( zone );
		}
	};
}
