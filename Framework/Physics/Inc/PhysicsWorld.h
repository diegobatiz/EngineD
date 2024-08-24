#pragma once

#include "PhysicsDebugDrawer.h"

namespace EngineD::Physics
{
	class PhysicsObject;

	class PhysicsWorld final
	{
	public:
		struct Settings
		{
			EngineD::Math::Vector3 gravity{ 0.0f, -9.81f, 0.0f };
			uint32_t simulationSteps = 10;
			float fixedTimeStep = 1.0f / 60.0f;
		};

		static void StaticInitialize(const Settings& settings);
		static void StaticTerminate();
		static PhysicsWorld* Get();

		PhysicsWorld() = default;
		~PhysicsWorld();

		void Initialize(const Settings& settings);
		void Terminate();

		void Update(float deltaTime);
		void DebugUI();

		void Register(PhysicsObject* physicsObject);
		void Unregister(PhysicsObject* physicsObject);

	private:
		using PhysicsObjects = std::vector<PhysicsObject*>;
		PhysicsObjects mPhysicsObjects;

		Settings mSettings;
		PhysicsDebugDrawer mDebugDrawer;

		//Bullet Objects
		btBroadphaseInterface* mInterface = nullptr;
		btCollisionDispatcher* mDispatcher = nullptr;
		btDefaultCollisionConfiguration* mCollisionConfiguration = nullptr;
		btSequentialImpulseConstraintSolver* mSolver = nullptr;

		friend class SoftBody;
#ifdef USE_SOFT_BODY
		btSoftRigidDynamicsWorld* mDynamicsWorld = nullptr;
		btSoftBodyWorldInfo& GetSoftBodyWorldInfo() { return mDynamicsWorld->getWorldInfo(); }
#else
		btDiscreteDynamicsWorld* mDynamicsWorld = nullptr;
		btSoftBodyWorldInfo& GetSoftBodyWorldInfo() { return btSoftBodyWorldInfo(); }
#endif
	};
}