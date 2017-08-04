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
#include "PhysicProperty.h"
#include "IComponent.h"
#include "3DMath.h"



namespace BohgeEngine
{
	class BOHGE_FRAMEWORK PhysicComponent : public IComponent
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( PhysicComponent, IComponent );
	private:
		vector3f			m_Inertia;
		float				m_fMass;
	public:
		PhysicComponent(void);
		~PhysicComponent(void);
	private:
		virtual void _DoSetLocalRotation( const Quaternionf& r ) = 0;
		virtual void _DoSetLocalPosition( const vector3f& p ) = 0;	//多层挂接的时候父节点的缩放会影响字节点相对位置
		virtual void _DoSetLocalScale( const vector3f& s ) = 0;
		virtual void _DoSetMass( float mass ) = 0;
		virtual void _DoSetInertia( const vector3f& i ) = 0;
	private:
		virtual void _DoOnAttachNode( );//当挂接在node上
	public:
		virtual void Release( );//将自己释放掉
	public:
		void MessageLoaclRotation( const NodeMessage& message );
		void MessageLoaclPosition( const NodeMessage& message );
		void MessageLoaclScale( const NodeMessage& message );
	public:
		virtual void Initialize( PhysicProperty::ConvexShapeType cst, const Aabbox3df& colShape ) = 0;
	public:
		BOHGE_FORCEINLINE void SetMass( float mass )
		{
			m_fMass = mass;
			_DoSetMass( m_fMass );
		}
		BOHGE_FORCEINLINE void SetInertia( const vector3f& i )
		{
			m_Inertia = i;
			_DoSetInertia( i );
		}
		BOHGE_FORCEINLINE float GetMass()
		{
			return m_fMass;
		}
		BOHGE_FORCEINLINE const vector3f& GetInertia()
		{
			return m_Inertia;
		}
	};
}