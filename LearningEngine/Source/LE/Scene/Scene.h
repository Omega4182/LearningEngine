#pragma once

#include "LE/Core/Timestep.h"
#include "LE/Renderer/Renderer2D.h"

#include <entt.hpp>

namespace LE
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep DeltaTime);

		Entity CreateEntity(const std::string& Name = std::string());

	private:

		entt::registry m_Registry;

		friend Entity;
	};
}