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



/////////////////////////////
//		BulletŒÔ¿Ì		   //
/////////////////////////////
#pragma once
#include "Predefine.h"
#include "VariableType.h"
#include "btBulletDynamicsCommon.h"



//class btCollisionShape;
//class btCollisionDispatcher;
//class btBroadphaseInterface;
//class btDiscreteDynamicsWorld;
//class btDefaultCollisionConfiguration;
//class btSequentialImpulseConstraintSolver;
//template <typename T> class btAlignedObjectArray;
namespace BohgeEngine
{
	class PhysicManager
	{
	private:
		typedef btAlignedObjectArray<btCollisionShape*> CollisionArray;
	private:
		btCollisionDispatcher*						m_pDispatcher;
		btBroadphaseInterface*						m_pOverlappingPairCache;
		btDiscreteDynamicsWorld*					m_pDynamicsWorld;
		btDefaultCollisionConfiguration*			m_pCollisionConfiguration;
		btSequentialImpulseConstraintSolver*		m_pSolver;
		CollisionArray*								m_pCollisionShapeArray;
	public:
		PhysicManager(void);
		~PhysicManager(void);
	public:
		BOHGE_FORCEINLINE void Update( uint millisecond )
		{
			m_pDynamicsWorld->stepSimulation( btScalar( millisecond ) * 0.001f, 10 );
		}
		BOHGE_FORCEINLINE void AddObject( btCollisionShape* colShape, btRigidBody* body )
		{
			m_pCollisionShapeArray->push_back(colShape);
			m_pDynamicsWorld->addRigidBody( body );
		}
		BOHGE_FORCEINLINE void RemoveObject( btCollisionShape* colShape, btRigidBody* body )
		{
			m_pDynamicsWorld->removeRigidBody( body );
			m_pCollisionShapeArray->remove( colShape );
		}
	};

}