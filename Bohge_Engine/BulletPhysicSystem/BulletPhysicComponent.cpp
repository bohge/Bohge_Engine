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
#include "BulletPhysicComponent.h"
#include "Framework/NodeMessage.h"
#include "Framework/Node.h"


using namespace BohgeEngine;
namespace BulletPhysicSystemPlugin
{
	//---------------------------------------------------------------------------------------------------------
	BulletPhysicComponent::BulletPhysicComponent(void)
		:m_pBody(NULL),
		m_pShape(NULL),
		m_pMotion(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	BulletPhysicComponent::~BulletPhysicComponent(void)
	{
		SAFE_DELETE( m_pShape );
		SAFE_DELETE( m_pMotion );
		SAFE_DELETE( m_pBody );
	}
	//---------------------------------------------------------------------------------------------------------
	void BulletPhysicComponent::OnRootNodeChange()
	{
		//进行一些挂接操作，随后填充
	}
	//---------------------------------------------------------------------------------------------------------
	void BulletPhysicComponent::Initialize( BohgeEngine::PhysicProperty::ConvexShapeType cst, const BohgeEngine::Aabbox3df& colShape )
	{
		ASSERT( NULL == m_pShape && NULL == m_pBody );
		vector3f halfsize = colShape.GetHalfSize();
		switch( cst )
		{
		case PhysicProperty::CST_BOX: m_pShape = new btBoxShape( btVector3(btScalar(halfsize.m_x),btScalar(halfsize.m_y),btScalar(halfsize.m_z)) ); break;
		case PhysicProperty::CST_CAPSULE: m_pShape = new btCapsuleShape( Math::GetMax( halfsize.m_x, halfsize.m_z), halfsize.m_y * 2.0f ); break;
		case PhysicProperty::CST_CYLINDER: m_pShape= new btCylinderShape( btVector3(btScalar(halfsize.m_x),btScalar(halfsize.m_y),btScalar(halfsize.m_z)) ); break;
		case PhysicProperty::CST_SPHERE: m_pShape = new btSphereShape( Math::GetMax( Math::GetMax( halfsize.m_x, halfsize.m_z), halfsize.m_y ) ); break;
		case PhysicProperty::CST_CONE: m_pShape = new btConeShape( Math::GetMax( halfsize.m_x, halfsize.m_z), halfsize.m_y * 2.0f ); break;
		default:ASSERT(false);
		}
		btTransform groundTransform;
		groundTransform.setIdentity();
		m_pMotion = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(1,m_pMotion,m_pShape, btVector3() );
		m_pBody = new btRigidBody(rbInfo);
	}
	//---------------------------------------------------------------------------------------------------------
	void BulletPhysicComponent::_DoSetMass( float mass )
	{
		if ( !Math::isEqual(GetMass(), 0.0f) )
		{
			btVector3 bv = btVector3(GetInertia().m_x, GetInertia().m_y, GetInertia().m_z);
			m_pShape->calculateLocalInertia( GetMass(), bv );
			m_pBody->setMassProps( GetMass(), bv );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void BulletPhysicComponent::_DoSetInertia( const BohgeEngine::vector3f& i )
	{
		if ( !Math::isEqual(GetMass(), 0.0f) )
		{
			btVector3 bv = btVector3(GetInertia().m_x, GetInertia().m_y, GetInertia().m_z);
			m_pShape->calculateLocalInertia( GetMass(), bv );
			m_pBody->setMassProps( GetMass(), bv );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void BulletPhysicComponent::_DoSetLocalRotation( const BohgeEngine::Quaternionf& r )
	{
		const Quaternionf& worldRota = *_HostNode()->Attribute<Quaternionf>( NodeProperty::GA_WORLD_ROTATION );
		btTransform groundTransform;
		m_pMotion->getWorldTransform( groundTransform );
		groundTransform.setRotation( btQuaternion( worldRota.m_x, worldRota.m_y, worldRota.m_z, worldRota.m_w ) );
		m_pMotion->setWorldTransform( groundTransform );
		m_pBody->setWorldTransform( groundTransform );
	}
	//---------------------------------------------------------------------------------------------------------
	void BulletPhysicComponent::_DoSetLocalPosition( const BohgeEngine::vector3f& p )
	{
		const vector3f& worldRota = *_HostNode()->Attribute<vector3f>( NodeProperty::GA_WORLD_POSITION );
		btTransform groundTransform;
		m_pMotion->getWorldTransform( groundTransform );
		groundTransform.setOrigin( btVector3( worldRota.m_x, worldRota.m_y, worldRota.m_z ) );
		m_pMotion->setWorldTransform( groundTransform );
		m_pBody->setWorldTransform( groundTransform );
	}
	//---------------------------------------------------------------------------------------------------------
	void BulletPhysicComponent::_DoSetLocalScale( const BohgeEngine::vector3f& s )
	{
		const vector3f& worldscale = *_HostNode()->Attribute<vector3f>( NodeProperty::GA_WORLD_SCALE );
		m_pShape->setLocalScaling( btVector3( worldscale.m_x, worldscale.m_y, worldscale.m_z ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void BulletPhysicComponent::TryUpdate()
	{
		if ( m_pBody->isActive() )
		{
			btTransform groundTransform;
			m_pMotion->getWorldTransform( groundTransform );
			btQuaternion btq = groundTransform.getRotation();
			btVector3 pos = groundTransform.getOrigin();
			Quaternionf r = Quaternionf( btq.x(), btq.y(), btq.z(), btq.w() );
			vector3f p = vector3f( pos.x(), pos.y(), pos.z() );

			_HostNode()->SendMessage( NodeMessage( this, &p, NodeProperty::SA_POSITION ) );
			_HostNode()->SendMessage( NodeMessage( this, &r, NodeProperty::SA_ROTATION ) );
		}
	}

}