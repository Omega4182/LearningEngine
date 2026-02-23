#pragma once

#include "LE/Scene/Scene.h"

#include <entt.hpp>

namespace LE
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity Handle, Scene* Scene);
		Entity(const Entity&) = default;
		
		template<typename T, typename ... Args>
		T& AddComponent(Args&&... args)
		{
			LE_CORE_ASSERT(HasComponent<T>() == false, "Entity already has the component!");
			return ScenePtr->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			LE_CORE_ASSERT(HasComponent<T>(), "Entity does not have the component!");
			ScenePtr->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			LE_CORE_ASSERT(HasComponent<T>(), "Entity does not have the component!");
			return ScenePtr->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return ScenePtr->m_Registry.any_of<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* ScenePtr = nullptr;
	};
}