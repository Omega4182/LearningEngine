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
		// Finding camera to render
		Camera* MainCamera = nullptr;
		glm::mat4* CameraTransform = nullptr;
		{
			auto View = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto Entity : View)
			{
				const auto& [Transform, Camera] = View.get<TransformComponent, CameraComponent>(Entity);

				if (Camera.bPrimary)
				{
					MainCamera = &Camera.Camera;
					CameraTransform = &Transform.Transform;
					break;
				}
			}
		}

		if (MainCamera != nullptr)
		{
			Renderer2D::BeginScene(*MainCamera, *CameraTransform);

			auto Group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto Entity : Group)
			{
				const auto& [Transform, SpriteRenderer] = Group.get<TransformComponent, SpriteRendererComponent>(Entity);

				Renderer2D::DrawQuad(Transform, SpriteRenderer.Color);
			}

			Renderer2D::EndScene();
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

	void Scene::OnViewportResize(uint32_t Width, uint32_t Height)
	{
		m_ViewportWidth = Width;
		m_ViewportHeight = Height;

		auto View = m_Registry.view<CameraComponent>();
		for (auto Entity : View)
		{
			auto& CameraComp = View.get<CameraComponent>(Entity);

			if (CameraComp.bFixedAspectRatio == false)
			{
				CameraComp.Camera.SetViewportSize(Width, Height);
			}
		}
	}
}