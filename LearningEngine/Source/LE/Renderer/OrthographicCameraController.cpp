#include "LEpch.h"
#include "OrthographicCameraController.h"

#include "LE/Core/Input.h"
#include "LE/Core/KeyCodes.h"

namespace LE
{
	OrthographicCameraController::OrthographicCameraController(float AspectRatio)
		: m_AspectRatio(AspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep DeltaTime)
	{
		LE_PROFILE_FUNCTION();

		const float finalSpeed = m_CameraTranslationSpeed * DeltaTime;

		if (Input::IsKeyPressed(LE_KEY_W))
			m_CameraPosition += glm::vec3(0, finalSpeed, 0);
		else if (Input::IsKeyPressed(LE_KEY_S))
			m_CameraPosition += glm::vec3(0, -finalSpeed, 0);

		if (Input::IsKeyPressed(LE_KEY_A))
			m_CameraPosition += glm::vec3(-finalSpeed, 0.f, 0);
		else if (Input::IsKeyPressed(LE_KEY_D))
			m_CameraPosition += glm::vec3(finalSpeed, 0.f, 0);

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		LE_PROFILE_FUNCTION();

		EventDispatcher dispatcher = EventDispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(LE_BIND(this, &OrthographicCameraController::OnMouseScroll));
		dispatcher.Dispatch<WindowResizeEvent>(LE_BIND(this, &OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float Width, float Height)
	{
		m_AspectRatio = Width / Height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScroll(MouseScrolledEvent& e)
	{
		LE_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetMouseYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		LE_PROFILE_FUNCTION();

		OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
		return false;
	}
}