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
#include "PhysicProperty.h"
#include "IPlugin.h"
#include "3DMath.h"


#include <EASTL/map.h>

namespace BohgeEngine
{
	class PhysicComponent;
	class BOHGE_FRAMEWORK IPhysicSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IPhysicSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IPhysicSystem, IPlugin );
	private:
		typedef eastl::map< uint, PhysicComponent* > PhysicComponentMap;
	private:
		PhysicComponentMap*				m_pPhysicComponentMap;
	public:
		IPhysicSystem(void);
		virtual ~IPhysicSystem(void);
	public:
		virtual void RegistEvent( Framework& framework ); //注册事件
		void OnFrameworkEntry( float time, Framework& framework );
		void OnFrameworkExit( float time, Framework& framework );
	private:
		virtual void _OnAttachPhysicComponent( PhysicComponent* rc ) = 0;
		virtual void _OnDetachPhysicComponent( PhysicComponent* rc ) = 0;
		virtual void _DoSetGravity( const vector3f& g ) = 0;
	public:
		virtual PhysicComponent* PhysicComponentFactory( ) const = 0;//创建一个job处理池
		virtual void RecycleBin( PhysicComponent* com ) const = 0;//回收
	public:
		void AttachPhysicComponent( PhysicComponent* rc );
		void DetachPhysicComponent( PhysicComponent* rc );
	protected:
		BOHGE_FORCEINLINE PhysicComponentMap& _GetPhysicComponentMap()
		{
			return *m_pPhysicComponentMap;
		}
	public:
		BOHGE_FORCEINLINE void SetGravity( const vector3f& g )
		{
			_DoSetGravity( g );
		}
	};
}