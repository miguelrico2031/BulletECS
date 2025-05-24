#include "BulletECS/PhysicsWorld.h"
#include <cassert>

namespace BulletECS
{
	PhysicsWorld::PhysicsWorld(btVector3 gravity)
	{
		m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
		m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());
		m_broadphase = std::make_unique<btDbvtBroadphase>();
		m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
		m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get());
	}

	PhysicsWorld::PhysicsWorld(
		std::unique_ptr<btCollisionConfiguration> collisionConfiguration,
		std::unique_ptr<btDispatcher> dispatcher,
		std::unique_ptr<btBroadphaseInterface> broadphase,
		std::unique_ptr<btConstraintSolver> solver,
		std::unique_ptr<btDynamicsWorld> dynamicsWorld)
		: m_collisionConfiguration(std::move(collisionConfiguration)),
		  m_dispatcher(std::move(dispatcher)),
		  m_broadphase(std::move(broadphase)),
		  m_solver(std::move(solver)),
		  m_dynamicsWorld(std::move(dynamicsWorld))
	{

	}

	PhysicsWorld::~PhysicsWorld()
	{
		m_dynamicsWorld = nullptr; //delete dynamics wolrd before everything else
		//TODO: maybe force the deletion order following the bullet helloworld example
	}




	btVector3 PhysicsWorld::getGravity() const
	{
		return m_dynamicsWorld->getGravity();
	}

	// Sets the dynamics world gravity, that means iterating each non static rigidbody and changing its gravity
	void PhysicsWorld::setGravity(btVector3 gravity)
	{
		m_dynamicsWorld->setGravity(gravity);
	}



	void PhysicsWorld::stepSimulation(float timeStep, int maxSubSteps, float fixedTimeStep)
	{
		m_dynamicsWorld->stepSimulation(static_cast<btScalar>(timeStep), maxSubSteps, static_cast<btScalar>(fixedTimeStep));
	}

	Entity PhysicsWorld::createEntity()
	{
		return m_entityManager.createEntity();
	}


	btDefaultMotionState* PhysicsWorld::addMotionState(Entity entity, const btTransform& transformData)
	{
		return m_motionStatePool.add(entity, transformData);
	}

	btBoxShape* PhysicsWorld::setBoxCollider(Entity entity, btVector3 halfExtents)
	{
		return m_collisionShapeContainer.setBox(entity, halfExtents);
	}

	btCylinderShape* PhysicsWorld::setCylinderCollider(Entity entity, btVector3 halfExtents)
	{
		return m_collisionShapeContainer.setCylinder(entity, halfExtents);
	}

	btSphereShape* PhysicsWorld::setSphereCollider(Entity entity, float radius)
	{
		return m_collisionShapeContainer.setSphere(entity, radius);
	}

	btCapsuleShape* PhysicsWorld::setCapsuleCollider(Entity entity, float radius, float height)
	{
		return m_collisionShapeContainer.setCapsule(entity, radius, height);
	}

	btCollisionShape* PhysicsWorld::setColliderFromExistentEntity(Entity entity, Entity existentEntityWithCollider)
	{
		return m_collisionShapeContainer.setFromExistentEntity(entity, existentEntityWithCollider);
	}

	void PhysicsWorld::addTag(Entity entity, const std::string& name)
	{
		m_tagPool.add(entity, name);
	}

	btRigidBody* PhysicsWorld::addRigidBody(Entity entity, float mass, float restitution)
	{
		//TODO: make sure the entity has a motionState and collider
		btDefaultMotionState* motionState = getMotionState(entity);
		btCollisionShape* collider = getCollisionShape(entity);

		assert((motionState && collider) && "Cannot add a RigidBody to an entity without motionState and collider");

		btVector3 localInertia(0, 0, 0);

		if (mass != 0.0f) //if is dynamic
		{
			collider->calculateLocalInertia(mass, localInertia);
		}

		btRigidBody::btRigidBodyConstructionInfo rbData(mass, motionState, collider, localInertia);
		rbData.m_restitution = restitution;

		btRigidBody* rigidBody = m_rigidBodyPool.add(entity, rbData);

		m_dynamicsWorld->addRigidBody(rigidBody);

		return rigidBody;
	}



	void PhysicsWorld::removeMotionState(Entity entity)
	{
		assert(!m_rigidBodyPool.has(entity) && "Cannot remove MotionState before RigidBody. Remove RigidBody first");
		m_motionStatePool.remove(entity);
	}

	void PhysicsWorld::removeCollider(Entity entity)
	{
		assert(!m_rigidBodyPool.has(entity) && "Cannot remove Collider before RigidBody. Remove RigidBody first");
		m_collisionShapeContainer.remove(entity);
	}

	void PhysicsWorld::removeRigidBody(Entity entity)
	{
		btRigidBody* rigidBody = getRigidBody(entity);
		assert(rigidBody && "Cannot remove non existent RigidBody.");
		m_dynamicsWorld->removeRigidBody(rigidBody);
		m_rigidBodyPool.remove(entity);
	}

	void PhysicsWorld::removeTag(Entity entity)
	{
		m_tagPool.remove(entity);
	}

	void PhysicsWorld::destroyEntity(Entity entity)
	{
		if (m_rigidBodyPool.has(entity))
		{
			removeRigidBody(entity);
		}
		if (m_collisionShapeContainer.has(entity))
		{
			removeCollider(entity);
		}
		if (m_motionStatePool.has(entity))
		{
			removeMotionState(entity);
		}
		if (m_tagPool.has(entity))
		{
			removeTag(entity);
		}
		m_entityManager.destroyEntity(entity);
	}

	btDefaultMotionState* PhysicsWorld::getMotionState(Entity entity)
	{
		return m_motionStatePool.get(entity);
	}

	const btDefaultMotionState* PhysicsWorld::getMotionState(const Entity entity) const
	{
		return m_motionStatePool.get(entity);
	}

	btCollisionShape* PhysicsWorld::getCollisionShape(Entity entity)
	{
		return m_collisionShapeContainer.get(entity);
	}

	const btCollisionShape* PhysicsWorld::getCollisionShape(const Entity entity) const
	{
		return m_collisionShapeContainer.get(entity);
	}

	btRigidBody* PhysicsWorld::getRigidBody(Entity entity)
	{
		return m_rigidBodyPool.get(entity);
	}

	const btRigidBody* PhysicsWorld::getRigidBody(const Entity entity) const
	{
		return m_rigidBodyPool.get(entity);
	}

	const std::string& PhysicsWorld::getTag(Entity entity) const
	{
		if (auto* tag = m_tagPool.get(entity))
		{
			return tag->name;
		}
		return NO_TAG;
	}


}