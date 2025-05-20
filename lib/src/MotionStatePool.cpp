#include "BulletECS/Containers/MotionStatePool.h"
#include <cassert>

namespace BulletECS
{
	MotionStatePool::~MotionStatePool()
	{
		for (size_t i = 0; i < m_hasMotionState.size(); i++)
		{
			if (m_hasMotionState[i])
			{
				ptr(i)->~btDefaultMotionState();
			}
		}
	}


	btDefaultMotionState* MotionStatePool::add(Entity entity, const btTransform& transformData)
	{
		size_t idx = entity.ID;
		assert(!m_hasMotionState[idx] && "Entity already has a MotionState.");

		void* location = &m_rigidBodiesStorage[idx];
		btDefaultMotionState* motionState = new (location) btDefaultMotionState(transformData);
		m_hasMotionState[idx] = true;
		return motionState;
	}

	void MotionStatePool::remove(Entity entity)
	{
		size_t idx = entity.ID;
		assert(m_hasMotionState[idx] && "Entity does not have a MotionState to remove.");
		ptr(idx)->~btDefaultMotionState();
		m_hasMotionState[idx] = false;

	}

	btDefaultMotionState* MotionStatePool::get(Entity entity)
	{
		size_t idx = entity.ID;
		if (m_hasMotionState[idx])
		{
			return ptr(idx);
		}
		return nullptr;
	}

}