#pragma once
#include "BulletECS/Entity.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <btBulletDynamicsCommon.h>
namespace BulletECS
{

	class CollisionShapeContainer
	{
	public:
		btBoxShape* setBox(Entity entity, btVector3 halfExtents);
		btCylinderShape* setCylinder(Entity entity, btVector3 halfExtents);
		btSphereShape* setSphere(Entity entity, float radius);
		btCapsuleShape* setCapsule(Entity entity, float radius, float height);
		btCollisionShape* setFromExistentEntity(Entity entity, Entity existentEntityWithCollider);

		void remove(Entity entity);

		inline bool has(Entity entity) const { return m_entitiesWithShape.find(entity.ID) != m_entitiesWithShape.end(); }

		btCollisionShape* get(Entity entity);
		const btCollisionShape* get(Entity entity) const;
		

	private:
		std::unordered_map <std::string, std::shared_ptr<btCollisionShape>> m_uniqueCollisionShapes;
		std::unordered_map <entity_id_t, std::shared_ptr<btCollisionShape>> m_entitiesWithShape;
	};
}