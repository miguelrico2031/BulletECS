#include "BulletECS/Containers/CollisionShapeContainer.h"
#include <sstream>

namespace BulletECS
{
	btBoxShape* CollisionShapeContainer::setBox(Entity entity, btVector3 halfExtents)
	{
		std::ostringstream oss;
		oss << "box:" << halfExtents.getX() << "," << halfExtents.getY() << "," << halfExtents.getZ();
		std::string key = oss.str();

		btBoxShape* ptr = nullptr;
		if (m_uniqueCollisionShapes.find(key) == m_uniqueCollisionShapes.end())
		{
			std::shared_ptr<btBoxShape> sharedPtr = std::make_shared<btBoxShape>(halfExtents);
			m_uniqueCollisionShapes[key] = sharedPtr;
			ptr = sharedPtr.get();
		}
		else
		{
			ptr = dynamic_cast<btBoxShape*>(m_uniqueCollisionShapes[key].get());
		}

		m_entitiesWithShape[entity.ID] = m_uniqueCollisionShapes[key];
		return ptr;
	}

	btCylinderShape* CollisionShapeContainer::setCylinder(Entity entity, btVector3 halfExtents)
	{
		std::ostringstream oss;
		oss << "cyl:" << halfExtents.getX() << ',' << halfExtents.getY() << ',' << halfExtents.getZ();
		std::string key = oss.str();

		btCylinderShape* ptr = nullptr;
		if (m_uniqueCollisionShapes.find(key) == m_uniqueCollisionShapes.end())
		{
			std::shared_ptr<btCylinderShape> sharedPtr = std::make_shared<btCylinderShape>(halfExtents);
			m_uniqueCollisionShapes[key] = sharedPtr;
			ptr = sharedPtr.get();
		}
		else
		{
			ptr = dynamic_cast<btCylinderShape*>(m_uniqueCollisionShapes[key].get());
		}

		m_entitiesWithShape[entity.ID] = m_uniqueCollisionShapes[key];
		return ptr;
	}

	btSphereShape* CollisionShapeContainer::setSphere(Entity entity, float radius)
	{
		std::ostringstream oss;
		oss << "sph:" << radius;
		std::string key = oss.str();

		btSphereShape* ptr = nullptr;
		if (m_uniqueCollisionShapes.find(key) == m_uniqueCollisionShapes.end())
		{
			std::shared_ptr<btSphereShape> sharedPtr = std::make_shared<btSphereShape>(radius);
			m_uniqueCollisionShapes[key] = sharedPtr;
			ptr = sharedPtr.get();
		}
		else
		{
			ptr = dynamic_cast<btSphereShape*>(m_uniqueCollisionShapes[key].get());
		}

		m_entitiesWithShape[entity.ID] = m_uniqueCollisionShapes[key];
		return ptr;
	}

	btCapsuleShape* CollisionShapeContainer::setCapsule(Entity entity, float radius, float height)
	{
		std::ostringstream oss;
		oss << "cap:" << radius << ',' << height;
		std::string key = oss.str();

		btCapsuleShape* ptr = nullptr;
		if (m_uniqueCollisionShapes.find(key) == m_uniqueCollisionShapes.end())
		{
			std::shared_ptr<btCapsuleShape> sharedPtr = std::make_shared<btCapsuleShape>(radius, height);
			m_uniqueCollisionShapes[key] = sharedPtr;
			ptr = sharedPtr.get();
		}
		else
		{
			ptr = dynamic_cast<btCapsuleShape*>(m_uniqueCollisionShapes[key].get());
		}

		m_entitiesWithShape[entity.ID] = m_uniqueCollisionShapes[key];
		return ptr;
	}


	btCollisionShape* CollisionShapeContainer::setFromExistentEntity(Entity entity, Entity existentEntityWithCollider)
	{
		auto it = m_entitiesWithShape.find(existentEntityWithCollider.ID);
		if (it == m_entitiesWithShape.end())
		{
			return nullptr;
		}
		m_entitiesWithShape[entity.ID] = it->second;
		return it->second.get();
	}


	void CollisionShapeContainer::remove(Entity entity)
	{
		m_entitiesWithShape.erase(entity.ID);
	}


	btCollisionShape* CollisionShapeContainer::get(Entity entity)
	{
		auto it = m_entitiesWithShape.find(entity.ID);
		if (it == m_entitiesWithShape.end())
		{
			return nullptr;
		}
		return it->second.get();
	}
}