#pragma once

#include "LE/Renderer/Camera.h"

namespace LE
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		SceneCamera(const glm::mat4& Projection);

		virtual ~SceneCamera() = default;

		void SetOrthographic(float Size, float NearClip, float FarClip);

		void SetViewportSize(uint32_t Width, uint32_t Height);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float Size) { m_OrthographicSize = Size; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		float m_OrthographicSize = 10.f;
		float m_OrthographicNear = -1.f;
		float m_OrthographicFar = 1.f;
		float m_AspectRatio = 0.f;
	};
}