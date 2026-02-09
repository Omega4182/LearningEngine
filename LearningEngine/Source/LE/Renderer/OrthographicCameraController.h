#pragma once

#include "LE/Renderer/OrthographicCamera.h"
#include "LE/Core/Timestep.h"

#include "LE/Events/ApplicationEvent.h"
#include "LE/Events/MouseEvent.h"

namespace LE
{
	class OrthographicCameraController
	{
	public:

		OrthographicCameraController(float AspectRatio);

		void OnUpdate(Timestep DeltaTime);
		void OnEvent(Event& e);

		void OnResize(float Width, float Height);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:

		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:

		float m_AspectRatio;
		float m_ZoomLevel = 1.f;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = glm::vec3(0, 0, 0);

		float m_CameraTranslationSpeed = 1.f;
	};
}