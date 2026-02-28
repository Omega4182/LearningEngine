#include "LEpch.h"

#include "LE/Scene/Scene.h"
#include "LE/Scene/Entity.h"
#include "LE/Scene/Components.h"

namespace LE
{
	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{

	}

	void Scene::OnUpdate(Timestep DeltaTime)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto& entity : group)
		{
			auto [Transform, SpriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(Transform, SpriteRenderer.Color);
		}
	}

	Entity Scene::CreateEntity(const std::string& Name)
	{
		Entity NewEntity = { m_Registry.create(), this };
		NewEntity.AddComponent<TransformComponent>();
		TagComponent& Tag = NewEntity.AddComponent<TagComponent>();
		Tag.Tag = Name.empty() ? "Entity" : Name;

		return NewEntity;
	}
}