#include "LEpch.h"
#include "LE/Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace LE
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	SceneCamera::SceneCamera(const glm::mat4& Projection)
		: Camera(Projection)
	{
	}

	void SceneCamera::SetOrthographic(float Size, float NearClip, float FarClip)
	{
		m_OrthographicSize = Size;
		m_OrthographicNear = NearClip;
		m_OrthographicFar = FarClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t Width, uint32_t Height)
	{
		m_AspectRatio = static_cast<float>(Width) / static_cast<float>(Height);
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		const float OrthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		const float OrthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		const float OrthoBottom = -m_OrthographicSize * 0.5f;
		const float OrthoTop = m_OrthographicSize * 0.5f;

		m_Projection = glm::ortho(OrthoLeft, OrthoRight, OrthoBottom, OrthoTop, m_OrthographicNear, m_OrthographicFar);
	}
}