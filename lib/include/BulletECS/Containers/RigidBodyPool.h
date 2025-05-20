#pragma once
#include <vector>
#include <type_traits>
#include "BulletECS/Entity.h"
#include <btBulletDynamicsCommon.h>
#include <bitset>

namespace BulletECS
{
	class RigidBodyPool
	{
	public:
		RigidBodyPool()
			: m_rigidBodiesStorage(MAX_ENTITIES + 1)
		{ }
		~RigidBodyPool();

		btRigidBody* add(Entity entity, const btRigidBody::btRigidBodyConstructionInfo& rbData);
		void remove(Entity entity);
		inline bool has(Entity entity) const { return m_hasRigidBody[entity.ID]; }
		btRigidBody* get(Entity entity);

	private:
		// Helper to get pointer to the storage as btRigidBody*
		inline btRigidBody* ptr(size_t idx)
		{
			return reinterpret_cast<btRigidBody*>(&m_rigidBodiesStorage[idx]);
		}
		inline const btRigidBody* ptr(size_t idx) const
		{
			return reinterpret_cast<const btRigidBody*>(&m_rigidBodiesStorage[idx]);
		}


	private:
		// Use uninitialized storage for placement new
		std::vector<std::aligned_storage_t<sizeof(btRigidBody), alignof(btRigidBody)>> m_rigidBodiesStorage;
		std::bitset<MAX_ENTITIES> m_hasRigidBody;
	};
}