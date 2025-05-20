#include "BulletECS/Containers/RigidBodyPool.h"
#include <cassert>
namespace BulletECS
{

	RigidBodyPool::~RigidBodyPool()
	{
		// Destroy all constructed rigid bodies
		for (size_t i = 0; i < m_hasRigidBody.size(); i++)
		{
			if (m_hasRigidBody[i])
			{
				ptr(i)->~btRigidBody();
			}
		}
	}


	btRigidBody* RigidBodyPool::add(Entity entity, const btRigidBody::btRigidBodyConstructionInfo& rbData)
	{
		size_t idx = entity.ID;
		assert(!m_hasRigidBody[idx] && "Entity already has a RigidBody.");

		void* location = &m_rigidBodiesStorage[idx];
		btRigidBody* rigidBody = new (location) btRigidBody(rbData);
		m_hasRigidBody[idx] = true;
		return rigidBody;
	}


	void RigidBodyPool::remove(Entity entity)
	{
		size_t idx = entity.ID;
		assert(m_hasRigidBody[idx] && "Entity does not have a RigidBody to remove.");
		ptr(idx)->~btRigidBody();
		m_hasRigidBody[idx] = false;
	
	}

	btRigidBody* RigidBodyPool::get(Entity entity)
	{
		size_t idx = entity.ID;
		if (m_hasRigidBody[idx])
		{
			return ptr(idx);
		}
		return nullptr;
	}


}