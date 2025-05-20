#include "BulletECS/EntityManager.h"
#include <cassert>
#include <set>
namespace BulletECS
{
	Entity EntityManager::createEntity()
	{
		if (m_destroyedAvailableEntities.empty())
		{
			assert(m_nextEntityID <= MAX_ENTITIES && "Too many entities created.");
			return Entity{ m_nextEntityID++, 1 }; //version = 1 because it's the 1st entity created with that ID, version 0 means not initialized
		}

		//get a destroyed entity and increment its version to make a new one
		Entity entity = m_destroyedAvailableEntities.front();
		m_destroyedAvailableEntities.pop();
		entity.version++;
		return entity;
	}
	void BulletECS::EntityManager::destroyEntity(Entity entity)
	{
		m_destroyedAvailableEntities.push(entity);
	}
}