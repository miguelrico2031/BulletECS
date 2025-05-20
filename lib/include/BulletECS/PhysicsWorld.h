#pragma once
#include <LinearMath/btVector3.h>
#include <memory>
#include <btBulletDynamicsCommon.h>
#include "BulletECS/EntityManager.h"
#include "BulletECS/Containers/RigidBodyPool.h"
#include "BulletECS/Containers/MotionStatePool.h"
#include "BulletECS/Containers/CollisionShapeContainer.h"

namespace BulletECS
{
	class PhysicsWorld
	{
	public:
		PhysicsWorld(btVector3 gravity);
		
		//Allows user to customize the Bullet managers used for the simulation
		PhysicsWorld(
			std::unique_ptr<btCollisionConfiguration> collisionConfiguration, 
			std::unique_ptr<btDispatcher> dispatcher, 
			std::unique_ptr<btBroadphaseInterface> broadphase, 
			std::unique_ptr<btConstraintSolver> solver, 
			std::unique_ptr<btDynamicsWorld> dynamicsWorld);
		
		~PhysicsWorld();


		btVector3 getGravity() const;
		// Sets the dynamics world gravity, that means iterating each non static rigidbody and changing its gravity
		void setGravity(btVector3 gravity);

		void stepSimulation(float timeStep, int maxSubSteps = 1, float fixedTimeStep = 1.0f / 60.0f);


		Entity createEntity();

		btDefaultMotionState* addMotionState(Entity entity, const btTransform& transformData);
		btRigidBody* addRigidBody(Entity entity, float mass);
		
		//These are called "set" because collision shapes are shared between rigidbodies as much as possible in the CollisionShapeContainer class
		btBoxShape* setBoxCollider(Entity entity, btVector3 halfExtents);
		btCylinderShape* setCylinderCollider(Entity entity, btVector3 halfExtents);
		btSphereShape* setSphereCollider(Entity entity, float radius);
		btCapsuleShape* setCapsuleCollider(Entity entity, float radius, float height);
		btCollisionShape* setColliderFromExistentEntity(Entity entity, Entity existentEntityWithCollider);

		//only if the entity has neither collider nor rigidBody
		void removeMotionState(Entity entity);
		//only if the entity has no rigidBody
		void removeCollider(Entity entity);

		void removeRigidBody(Entity entity);

		//removes (if exists) its rigidBody then its collider and then its motionState
		void destroyEntity(Entity entity);


		//these return nullptr if the entity does not have the component
		btDefaultMotionState* getMotionState(Entity entity);
		btCollisionShape* getCollisionShape(Entity entity);
		btRigidBody* getRigidBody(Entity entity);

	private:
		std::unique_ptr<btCollisionConfiguration> m_collisionConfiguration = nullptr;
		std::unique_ptr<btDispatcher> m_dispatcher = nullptr;
		std::unique_ptr<btBroadphaseInterface> m_broadphase = nullptr;
		std::unique_ptr<btConstraintSolver> m_solver = nullptr;
		std::unique_ptr<btDynamicsWorld> m_dynamicsWorld = nullptr;

		EntityManager m_entityManager;
		RigidBodyPool m_rigidBodyPool;
		MotionStatePool m_motionStatePool;
		CollisionShapeContainer m_collisionShapeContainer;
	};
}

