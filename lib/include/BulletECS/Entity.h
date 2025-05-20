#pragma once
#include <cstdint>

#ifndef BULLET_ECS_MAX_ENTITIES
#define BULLET_ECS_MAX_ENTITIES 4096
#endif


namespace BulletECS
{
	using entity_id_t = uint32_t;
	using entity_version_t = uint32_t;
	constexpr entity_id_t NULL_ENTITY = 0;
	constexpr entity_id_t MAX_ENTITIES = BULLET_ECS_MAX_ENTITIES;

	struct Entity
	{
		entity_id_t ID = NULL_ENTITY; //this is the "actual" entity, used for array indexing
		entity_version_t version = 0; //this is for reusing entity IDs, to be able to tell apart a destroyed entity from the newly creadted one if the share ids
	};
}