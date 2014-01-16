LOCAL_PATH := $(call my-dir)

#ŒÔ¿Ìø‚

include $(CLEAR_VARS)

LOCAL_MODULE    := bullet

LOCAL_ARM_MODE	:= arm

LOCAL_CFLAGS	:=   -O2 -DTARGET_FLOAT32_IS_FIXED -IInclude -Iandroid

LOCAL_SRC_FILES :=\
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/BroadphaseCollision/btDispatcher.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/BroadphaseCollision/btDbvt.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btUnionFind.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btManifoldResult.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btGhostObject.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btCollisionObject.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btTriangleMesh.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btTriangleCallback.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btSphereShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btShapeHull.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btEmptyShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btCylinderShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btConvexShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btConvexHullShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btConvex2dShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btConeShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btConcaveShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btCompoundShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btCollisionShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btCapsuleShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btBoxShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/CollisionShapes/btBox2dShape.cpp \
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/gim_tri_collision.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/gim_memory.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/gim_contact.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/gim_box_set.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/btTriangleShapeEx.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/btGImpactShape.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/btGImpactBvh.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/Gimpact/btContactProcessing.cpp \
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
	\
	\
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/Character/btKinematicCharacterController.cpp \
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btGearConstraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/Dynamics/Bullet-C-API.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/Dynamics/btRigidBody.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/Vehicle/btRaycastVehicle.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletDynamics/Vehicle/btWheelInfo.cpp \
	\
	\
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuMinkowskiPenetrationDepthSolver.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuContactResult.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuCollisionShapes.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/boxBoxDistance.cpp \
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuSampleTask/SpuSampleTask.cpp \
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuSampleTaskProcess.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuLibspe2Support.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuGatheringCollisionDispatcher.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuFakeDma.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuContactManifoldCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuCollisionTaskProcess.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SpuCollisionObjectWrapper.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/SequentialThreadSupport.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/PosixThreadSupport.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/btThreadSupportInterface.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/btParallelConstraintSolver.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletMultiThreaded/btGpu3DGridBroadphase.cpp \
	\
	\
	\
	../../../../Librarys/Bohge/External/bullet/src/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletSoftBody/btSoftRigidDynamicsWorld.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletSoftBody/btSoftBodyHelpers.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletSoftBody/btSoftBody.cpp \
	../../../../Librarys/Bohge/External/bullet/src/BulletSoftBody/btDefaultSoftBodySolver.cpp \
	\
	\
	\
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/vlookup.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/splitplane.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/raytri.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/planetri.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/meshvolume.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/float_math.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/fitsphere.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/ConvexDecomposition.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/ConvexBuilder.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/concavity.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/cd_wavefront.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/cd_hull.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/bestfitobb.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition/bestfit.cpp \
	\
	\
	\
	../../../../Librarys/Bohge/External/bullet/Extras/GIMPACTUtils/btGImpactConvexDecompositionShape.cpp \
	\
	\
	\
	../../../../Librarys/Bohge/External/bullet/Extras/HACD/hacdManifoldMesh.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/HACD/hacdICHull.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/HACD/hacdHACD.cpp \
	../../../../Librarys/Bohge/External/bullet/Extras/HACD/hacdGraph.cpp \
	\
	\
	\
	../../../../Librarys/Bohge/External/bullet/src/LinearMath/btVector3.cpp \
	../../../../Librarys/Bohge/External/bullet/src/LinearMath/btSerializer.cpp \
	../../../../Librarys/Bohge/External/bullet/src/LinearMath/btQuickprof.cpp \
	../../../../Librarys/Bohge/External/bullet/src/LinearMath/btPolarDecomposition.cpp \
	../../../../Librarys/Bohge/External/bullet/src/LinearMath/btGeometryUtil.cpp \
	../../../../Librarys/Bohge/External/bullet/src/LinearMath/btConvexHullComputer.cpp \
	../../../../Librarys/Bohge/External/bullet/src/LinearMath/btConvexHull.cpp \
	../../../../Librarys/Bohge/External/bullet/src/LinearMath/btAlignedAllocator.cpp \
	
LOCAL_CFLAGS += -march=armv5te -mtune=xscale -msoft-float -fpic -mthumb-interwork \
-ffunction-sections -funwind-tables -fstack-protector -fno-short-enums \
-fno-exceptions -fno-rtti \
-D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -DANDROID -O2 -DNDEBUG -g \

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../../../Librarys/Bohge/External/bullet/src \
	$(LOCAL_PATH)/../../../../Librarys/Bohge/External/bullet/Extras/ConvexDecomposition \
	$(LOCAL_PATH)/../../../../Librarys/Bohge/External/bullet/Extras/GIMPACTUtils \
	$(LOCAL_PATH)/../../../../Librarys/Bohge/External/bullet/Extras/HACD

  
include $(BUILD_STATIC_LIBRARY)