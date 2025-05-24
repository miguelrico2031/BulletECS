#pragma once
#include "BulletECS/Entity.h"
#include <vector>
#include <type_traits>
#include <bitset>
#include <cassert>
#include <iostream>
namespace BulletECS
{
	template <class T>
	class ComponentPool
	{
		using PoolArray = std::vector<std::aligned_storage_t<sizeof(T), alignof(T)>>;


	public:
		ComponentPool() {}
		~ComponentPool()
		{
			for (size_t i = 0; i <= m_highestEntityEver; i++)
			{
				if (m_hasComponent[i])
				{
					ptr(i)->~T();
				}
			}
		}

		template <typename ...Args>
		T* add(Entity entity, Args&&... args)
		{
			size_t idx = entity.ID;
			static_assert(std::is_constructible_v<T, Args...>, "Component cannot be constructed with the given arguments.");
			assert(idx <= MAX_ENTITIES && "Entity ID out of bounds.");
			assert(!m_hasComponent[idx] && "Cannot add same component twice.");

			void* location = &m_storage[idx];
			T* component = new (location) T(std::forward<Args>(args)...);
			m_hasComponent[idx] = true;
			m_highestEntityEver = idx > m_highestEntityEver ? idx : m_highestEntityEver;
			return component;
		}

		void remove(Entity entity)
		{
			size_t idx = entity.ID;
			assert(m_hasComponent[idx] && "Cannot remove non existent component.");
			ptr(idx)->~T();
			m_hasComponent[idx] = false;
		}

		inline bool has(Entity entity) const { return m_hasComponent[entity.ID]; }

		T* get(Entity entity)
		{
			size_t idx = entity.ID;
			if (m_hasComponent[idx])
			{
				return ptr(idx);
			}
			return nullptr;
		}

		const T* get(Entity entity) const
		{
			size_t idx = entity.ID;
			if (m_hasComponent[idx])
			{
				return ptr(idx);
			}
			return nullptr;
		}


	private:
		inline T* ptr(size_t idx)
		{
			return reinterpret_cast<T*>(&m_storage[idx]);
		}
		inline const T* ptr(size_t idx) const
		{
			return reinterpret_cast<const T*>(&m_storage[idx]);
		}


	private:
		PoolArray m_storage = PoolArray(MAX_ENTITIES + 1);
		std::bitset<MAX_ENTITIES + 1> m_hasComponent;
		entity_id_t m_highestEntityEver = NULL_ENTITY;


#pragma region Iterators

	public:
		class EntityIterator
		{
		public:
			EntityIterator(ComponentPool* pool, entity_id_t index)
				: m_pool(pool), m_index(index)
			{
				skipUninitialized();
				//std::cout << "iterator created\n";
			}

			Entity operator *() const { return Entity{m_index, 0}; } //uninitialized version because it's unknown

			EntityIterator& operator++()
			{
				m_index++;
				skipUninitialized();
				return *this;
			}

			bool operator==(const EntityIterator& other) const { return m_index == other.m_index && m_pool == other.m_pool; }
			bool operator!=(const EntityIterator& other) const { return !(*this == other); }

		private:
			void skipUninitialized()
			{
				while (m_index <= m_pool->m_highestEntityEver && !m_pool->m_hasComponent[m_index])
				{
					m_index++;
				}
			}
		private:
			ComponentPool* m_pool;
			entity_id_t m_index;
		};

		/**/class ConstEntityIterator
		{
		public:
			ConstEntityIterator(const ComponentPool* pool, entity_id_t index)
				: m_pool(pool), m_index(index)
			{
				skipUninitialized();
				//std::cout << "CONST iterator created\n";
			}

			Entity operator *() const { return Entity{ m_index, 0 }; } //uninitialized version because it's unknown

			ConstEntityIterator& operator++()
			{
				m_index++;
				skipUninitialized();
				return *this;
			}

			bool operator==(const ConstEntityIterator& other) const { return m_index == other.m_index && m_pool == other.m_pool; }
			bool operator!=(const ConstEntityIterator& other) const { return !(*this == other); }

		private:
			void skipUninitialized()
			{
				while (m_index <= m_pool->m_highestEntityEver && !m_pool->m_hasComponent[m_index])
				{
					m_index++;
				}
			}
		private:
			const ComponentPool* m_pool;
			entity_id_t m_index;
		};

#pragma endregion
		
		
		EntityIterator begin() { return EntityIterator(this, 1); }
		EntityIterator end() { return EntityIterator(this, m_highestEntityEver + 1); }

		ConstEntityIterator begin() const { return ConstEntityIterator(this, 1); }
		ConstEntityIterator end() const { return ConstEntityIterator(this, m_highestEntityEver + 1); }
	};
}