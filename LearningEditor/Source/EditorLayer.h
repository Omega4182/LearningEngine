#pragma once

#include <LearningEngine.h>

namespace LE
{
	class EditorLayer : public Layer
	{
	public:

		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep DeltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:

		TSharedPtr<VertexArray> m_VertexArray;
		TSharedPtr<Shader> m_Shader;
		TSharedPtr<Texture2D> m_Texture;
		TSharedPtr<Framebuffer> m_Framebuffer;

		TSharedPtr<Scene> m_ActiveScene;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		OrthographicCameraController m_CameraController;

		glm::vec2 m_ViewportSize = glm::vec2(0.f, 0.f);

		glm::vec4 m_SquareColor = glm::vec4(0.2f, 0.2f, 0.8f, 1.f);
	};
}