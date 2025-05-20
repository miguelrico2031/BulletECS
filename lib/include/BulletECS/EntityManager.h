#pragma once
#include "BulletECS/Entity.h"
#include <queue>
namespace BulletECS
{
	class EntityManager
	{
	public:
		EntityManager() = default;

		Entity createEntity();
		void destroyEntity(Entity entity);
		
	private:
		entity_id_t m_nextEntityID = 1;
		std::queue<Entity> m_destroyedAvailableEntities = {};
	};
}