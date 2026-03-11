#pragma once

#include <glm/glm.hpp>

namespace LE
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& Projection)
			: m_Projection(Projection) { }

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.f);
	};
}