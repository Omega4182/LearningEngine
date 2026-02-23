#pragma once

#include <glm/glm.hpp>

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
}