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
#include "PipelineProperty.h"
#include "SystemSingleton.h"
#include "IPlugin.h"
#include "3DMath.h"


#include <EASTL/map.h>
#include <EASTL/vector.h>

namespace BohgeEngine
{
	class IInserter;
	class LightComponent;
	class BOHGE_FRAMEWORK IEnvironmentSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IEnvironmentSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IEnvironmentSystem, IPlugin )
	private:
		typedef eastl::map< intptr_t, LightComponent* > LightComponentMap;
		typedef eastl::list< LightComponent* > LightComponentList;
		typedef eastl::list< Aabbox3df > ZoneList;
	private:
		LightComponentMap*					m_pAttachedComponentMap;
		LightComponentList*					m_pToAttachedList;
		LightComponentList*					m_pToDettachedList;
		LightComponentList*					m_pToChangedList;
		ZoneList*							m_pInsertZone;
		ZoneList*							m_pRemoveZone;
	public:
		IEnvironmentSystem(void);
		virtual ~IEnvironmentSystem(void);
	private:
		virtual void _OnLightComponentChange( LightComponent* rc ) = 0;
		virtual void _OnAttachLightComponent( LightComponent* rc ) = 0;
		virtual void _OnDetachLightComponent( LightComponent* rc ) = 0;
		virtual void _OnInsertCullZone( const Aabbox3df& zone ) = 0;
		virtual void _OnRemoveCullZone( const Aabbox3df& zone ) = 0;
	public:
		virtual void Culling( IInserter* ins ) = 0;//¼ÆËã²Ã¼ô
	private:
		void _DoLightComponentChange();
		void _DoAttachLightComponent();
		void _DoDetachLightComponent();
		void _DoInsertCullZone();
		void _DoRemoveCullZone();
	public:
		virtual void RegistEvent( Framework& framework ); //×¢²áÊÂ¼þ
		void OnFrameworkEntry( float time, Framework& framework );
		void OnFrameworkExit( float time, Framework& framework );
	public:
		void UpdateSystem();
	public:
		BOHGE_FORCEINLINE void AttachLightComponent( LightComponent* rc )
		{
			m_pToAttachedList->push_back( rc );
		}
		BOHGE_FORCEINLINE void DetachLightComponent( LightComponent* rc )
		{
			m_pToDettachedList->push_back( rc );
		}
		BOHGE_FORCEINLINE void LightComponentChange( LightComponent* rc )
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