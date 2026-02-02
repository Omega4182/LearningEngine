
#pragma once

#include "LE/Core/Core.h"
#include "LE/Core/Window.h"
#include "LE/Events/ApplicationEvent.h"
#include "LE/Core/LayerStack.h"

#include "LE/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace LE
{
	class Application
	{
	public:

		inline static Application& Get()
		{
			return *s_Instance;
		}

		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* Layer);
		void PushOverlay(Layer* Overlay);

		inline Window& GetWindow() const
		{
			return *m_Window;
		}

	private:

		void Run();

		bool OnWindowCloseCallback(const WindowCloseEvent& e);
		bool OnWindowResizeCallback(const WindowResizeEvent& e);

		friend int ::main(int argc, char** argv);

	private:

		static Application* s_Instance;

		TUniquePtr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool bIsRunning = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.f;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

