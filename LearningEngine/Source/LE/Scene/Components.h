#pragma once

#include <glm/glm.hpp>

#include "LE/Scene/SceneCamera.h"

namespace LE
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& NewTag)
			: Tag(NewTag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& NewTransform)
			: Transform(NewTransform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.f, 1.f, 1.f, 1.f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& NewColor)
			: Color(NewColor) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool bPrimary = true; // TODO: To be moved somewhere (PlayerController or scene)
		bool bFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& Projection)
			: Camera(Projection) { }
	};
}