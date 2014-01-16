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



#include "LocatableObject.h"
#include "PhysicManager.h"
#include "Engine.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	LocatableObject::LocatableObject( IAttributeNode* const host  )
		:m_pHost(host),
		m_isChange( true ),
		m_LocalScale(1,1,1),
		m_isPositionChange(true),
		m_isRotationChange(true),
		m_isScaleChange(true),
		m_isWorldRotationChange(true),
		m_isWorldTransfromChange(true),
		m_isWorldTransfromInversChange(true)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	LocatableObject::~LocatableObject(void)
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void LocatableObject::_WorldTransfromUpdata()
	{
//#error 旋转和缩放针对父节点的更新是不对的
		if ( m_pHost->isFatherChange() )
		{
			m_isScaleChange = true;
			m_isRotationChange = true;
			m_isPositionChange = true;
		}
		m_isWorldTransfromChange = true;
		m_isWorldRotationChange = true;
		m_pHost->CastChangedMessage();//通知子节点
	}
	//-------------------------------------------------------------------------------------------------------
	void LocatableObject::Update( uint milisecond )//如果有更新
	{
		if( m_isChange || m_pHost->isFatherChange())
		{
			m_isChange = false;
			_WorldTransfromUpdata();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LocatableObject::SetLocalPosition( const vector3f& p )
	{
		m_isWorldTransfromChange = true;
		m_isWorldRotationChange = true;
		m_isChange = true;
		m_isPositionChange= true;
		m_LocalPosition = p;
	}
	//-------------------------------------------------------------------------------------------------------
	void LocatableObject::SetLocalRotation( const Quaternionf& r )
	{
		m_isWorldTransfromChange = true;
		m_isWorldRotationChange = true;
		m_isChange = true;
		m_isRotationChange = true;
		m_LocalRotation = r;
		//m_Up = Math::Transform_Quat( UP, m_Rotation );
		//m_Up.NormalizeSelf();
		//m_Forward = Math::Transform_Quat( FORWARD, m_Rotation );
		//m_Forward.NormalizeSelf();
	}
	//-------------------------------------------------------------------------------------------------------
	void LocatableObject::SetLocalScale( const vector3f& s )
	{
		m_isWorldTransfromChange = true;
		m_isWorldRotationChange = true;
		m_isChange = true;
		m_isScaleChange= true;
		m_LocalScale = s;
	}
	//-------------------------------------------------------------------------------------------------------
	const vector3f& LocatableObject::GetWorldPosition() const
	{
		if ( m_isPositionChange )
		{
			const void* data;
			m_isPositionChange = false;
			data = m_pHost->FatherAttribute( IAttributeNode::ATT_WORLDSCALE );
			vector3f fatherscale = NULL == data ? vector3f(1,1,1) : *((const vector3f*)data);
			data = m_pHost->FatherAttribute( IAttributeNode::ATT_WORLDROTATION );
			Quaternionf rot = NULL == data ? Quaternionf(0,0,0,1) : *((const Quaternionf*)data);
			data = m_pHost->FatherAttribute( IAttributeNode::ATT_WORLDPOSITION );
			m_WorldPosition = NULL == data ? m_LocalPosition * fatherscale : m_LocalPosition * fatherscale * rot + *((const vector3f*)data);
		}
		return m_WorldPosition;
	}
	//-------------------------------------------------------------------------------------------------------
	const vector3f& LocatableObject::GetWorldScale() const
	{
		if ( m_isScaleChange )
		{
			const void* data;
			m_isScaleChange = false;
			data = m_pHost->FatherAttribute( IAttributeNode::ATT_WORLDSCALE );
			vector3f fatherscale = NULL == data ? vector3f(1,1,1) : *((const vector3f*)data);
			m_WorldScale = NULL == data ? m_LocalScale : m_LocalScale * *((const vector3f*)data);
		}
		return m_WorldScale;
	}
	//-------------------------------------------------------------------------------------------------------
	const Quaternionf& LocatableObject::GetWorldRotation() const
	{
		if ( m_isRotationChange )
		{
			const void* data;
			m_isRotationChange = false;
			data = m_pHost->FatherAttribute( IAttributeNode::ATT_WORLDROTATION );
			m_WorldRotation = NULL == data ? m_LocalRotation : m_LocalRotation * *((const Quaternionf*)data);
		}
		return m_WorldRotation;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	const Matrix44f& LocatableObject::GetWorldTransform() const
	{
		if ( m_isWorldTransfromChange )
		{
			m_isWorldTransfromChange = false;
			m_isWorldTransfromInversChange = true;
			m_WorldTransfrom = Math::Transformation<float>( NULL, NULL, &GetWorldScale(), NULL, &GetWorldRotation(), &GetWorldPosition() );
		}
		return m_WorldTransfrom;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	const Matrix33f& LocatableObject::GetWorldRotationMatrix() const
	{
		if ( m_isWorldRotationChange )
		{
			m_isWorldRotationChange = false;
			m_WorldRotationMatrix = GetWorldRotation().ToMatrix33();
		}
		return m_WorldRotationMatrix;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	const Matrix44f& LocatableObject::GetWorldTransformInvers() const
	{
		if ( m_isWorldTransfromInversChange )
		{
			m_isWorldTransfromInversChange = false;
			GetWorldTransform().GetInverse( m_WorldTransfromInvers );
		}
		return m_WorldTransfromInvers;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	const void* LocatableObject::LocatableAttribute( const IAttributeNode::Node_Attribute& name ) const
	{
		switch( name )
		{
		case IAttributeNode::ATT_WORLDSCALE : return &GetWorldScale();
		case IAttributeNode::ATT_WORLDROTATION : return &GetWorldRotation();
		case IAttributeNode::ATT_WORLDPOSITION : return &GetWorldPosition();
		case IAttributeNode::ATT_WORLDTRANSFORM : return &GetWorldTransform();
		case IAttributeNode::ATT_WORLDTRANSFORM_INVERS : return &GetWorldTransformInvers();
		case IAttributeNode::ATT_NORMALTRANSFORM : return &GetWorldRotationMatrix();
		}
		//ASSERT(false);
		return NULL;
	}













	//-------------------------------------------------------------------------------------------------------
	PhysicalObject::PhysicalObject(  IAttributeNode* const host  )
		:LocatableObject(host),
		m_pBody(NULL),
		m_pShape(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	PhysicalObject::~PhysicalObject()
	{
		Engine::Instance().GetPhysicManager()->RemoveObject( m_pShape, m_pBody );
		SAFE_DELETE( m_pMotion );
		SAFE_DELETE( m_pShape );
		SAFE_DELETE( m_pBody );
	}
	//-------------------------------------------------------------------------------------------------------
	void PhysicalObject::_InitPhysical( aabbox3df& colShape, float mass, const vector3f& localInertia )
	{
		ASSERT( NULL == m_pShape && NULL == m_pBody );
		vector3f halfsize = colShape.GetHalfSize();
		m_pShape = new btBoxShape( btVector3(btScalar(halfsize.m_x),btScalar(halfsize.m_y),btScalar(halfsize.m_z)) );
		if ( !Math::isEqual(mass, 0.0f) )
		{
			btVector3 bv = btVector3(localInertia.m_x, localInertia.m_y, localInertia.m_z);
			m_pShape->calculateLocalInertia( mass, bv );
		}
		btTransform groundTransform;
		groundTransform.setIdentity();
		m_pMotion = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,m_pMotion,m_pShape, btVector3(localInertia.m_x, localInertia.m_y, localInertia.m_z) );
		m_pBody = new btRigidBody(rbInfo);
		Engine::Instance().GetPhysicManager()->AddObject( m_pShape, m_pBody );
	}
	//-------------------------------------------------------------------------------------------------------
	void PhysicalObject::Update( uint milisecond )
	{
		if ( isChange() || m_pBody->isActive() )
		{
			btTransform groundTransform;
			m_pMotion->getWorldTransform( groundTransform );
			btQuaternion btq = groundTransform.getRotation();
			LocatableObject::SetLocalRotation( Quaternionf( btq.x(), btq.y(), btq.z(), btq.w() ) );
			btVector3 pos = groundTransform.getOrigin();
			LocatableObject::SetLocalPosition( vector3f( pos.x(), pos.y(), pos.z() ) );
			LocatableObject::Update( milisecond );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void PhysicalObject::SetLocalRotation( const Quaternionf& r )
	{
		LocatableObject::SetLocalRotation( r );

		Quaternionf worldRota = GetWorldRotation();
		btTransform groundTransform;
		m_pMotion->getWorldTransform( groundTransform );
		groundTransform.setRotation( btQuaternion( worldRota.m_x, worldRota.m_y, worldRota.m_z, worldRota.m_w ) );
		m_pMotion->setWorldTransform( groundTransform );
		m_pBody->setWorldTransform( groundTransform );
	}
	//-------------------------------------------------------------------------------------------------------
	void PhysicalObject::SetLocalPosition( const vector3f& p )
	{
		LocatableObject::SetLocalPosition( p );

		vector3f worldpos = GetWorldPosition();
		btTransform groundTransform;
		m_pMotion->getWorldTransform( groundTransform );
		groundTransform.setOrigin( btVector3( worldpos.m_x, worldpos.m_y, worldpos.m_z ) );
		m_pMotion->setWorldTransform( groundTransform );
		m_pBody->setWorldTransform( groundTransform );
	}
	//-------------------------------------------------------------------------------------------------------
	void PhysicalObject::SetLocalScale( const vector3f& s )
	{
		LocatableObject::SetLocalScale( s );

		vector3f worldscale = GetWorldScale();
		m_pShape->setLocalScaling( btVector3( worldscale.m_x, worldscale.m_y, worldscale.m_z ) );
	}
	//-------------------------------------------------------------------------------------------------------
}