#pragma once
#include <vector>
#include "BulletECS/Entity.h"
#include <btBulletDynamicsCommon.h>
#include <bitset>

namespace BulletECS
{
	class MotionStatePool
	{
	public:
		MotionStatePool()
			: m_rigidBodiesStorage(MAX_ENTITIES + 1)
		{
		}
		~MotionStatePool();

		btDefaultMotionState* add(Entity entity, const btTransform& transformData);
		void remove(Entity entity);
		inline bool has(Entity entity) const { return m_hasMotionState[entity.ID]; }
		btDefaultMotionState* get(Entity entity);

	private:
		inline btDefaultMotionState* ptr(size_t idx)
		{
			return reinterpret_cast<btDefaultMotionState*>(&m_rigidBodiesStorage[idx]);
		}
		inline const btDefaultMotionState* ptr(size_t idx) const
		{
			return reinterpret_cast<const btDefaultMotionState*>(&m_rigidBodiesStorage[idx]);
		}

	private:
		//TODO: replace defaultMotionState for custom, lighter one that only has 1 btTransform?
		std::vector<std::aligned_storage_t<sizeof(btDefaultMotionState), alignof(btDefaultMotionState)>> m_rigidBodiesStorage;
		std::bitset<MAX_ENTITIES> m_hasMotionState;
	};
}