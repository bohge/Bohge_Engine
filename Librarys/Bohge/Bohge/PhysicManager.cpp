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
//		BulletÎïÀí		   //
/////////////////////////////
#include "Predefine.h"
#include "PhysicManager.h"



namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	PhysicManager::PhysicManager(void)
	{
		///-----initialization_start-----

		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		m_pCollisionConfiguration = NEW btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		m_pDispatcher = NEW	btCollisionDispatcher(m_pCollisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		m_pOverlappingPairCache = NEW btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		m_pSolver = new btSequentialImpulseConstraintSolver();

		m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher,m_pOverlappingPairCache,m_pSolver,m_pCollisionConfiguration);

		m_pDynamicsWorld->setGravity(btVector3(0,-10,0));

		m_pCollisionShapeArray = NEW CollisionArray();



		////test
		//btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(5000.),btScalar(5000.),btScalar(5000.)));
		////groundShape->initializePolyhedralFeatures();
		////	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),50);

		//m_pCollisionShapeArray->push_back(groundShape);

		//btTransform groundTransform;
		//groundTransform.setIdentity();
		//groundTransform.setOrigin(btVector3(0,-4950,0));

		////We can also use DemoApplication::localCreateRigidBody, but for clarity it is provided here:
		//{
		//	btScalar mass(0.);

		//	//rigidbody is dynamic if and only if mass is non zero, otherwise static
		//	bool isDynamic = (mass != 0.f);

		//	btVector3 localInertia(0,0,0);
		//	if (isDynamic)
		//		groundShape->calculateLocalInertia(mass,localInertia);

		//	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		//	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
		//	btRigidBody* body = new btRigidBody(rbInfo);

		//	//add the body to the dynamics world
		//	m_pDynamicsWorld->addRigidBody(body);
		//}

	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	PhysicManager::~PhysicManager(void)
	{
		SAFE_DELETE( m_pCollisionConfiguration );
		SAFE_DELETE( m_pDispatcher );
		SAFE_DELETE( m_pOverlappingPairCache );
		SAFE_DELETE( m_pSolver );
		SAFE_DELETE( m_pDynamicsWorld );
		SAFE_DELETE( m_pCollisionShapeArray );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------

}