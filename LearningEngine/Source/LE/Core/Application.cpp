
#include "LEpch.h"

#include "Application.h"
#include "Input.h"

#include "LE/Renderer/Renderer.h"

#include "LE/Core/Timestep.h"

// TEMP:
#include <GLFW/glfw3.h>

namespace LE
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		LE_PROFILE_FUNCTION();

		LE_CORE_ASSERT(!s_Instance, "Only one instance of Application can be present!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(LE_BIND(this, &Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		LE_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		LE_PROFILE_FUNCTION();

		EventDispatcher dispatcher = EventDispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(LE_BIND(this, &Application::OnWindowCloseCallback));
		dispatcher.Dispatch<WindowResizeEvent>(LE_BIND(this, &Application::OnWindowResizeCallback));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.IsHandled())
			{
				break;
			}

			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* Layer)
	{
		LE_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(Layer);
	}

	void Application::PushOverlay(Layer* Overlay)
	{
		LE_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(Overlay);
	}

	void Application::Close()
	{
		bIsRunning = false;
	}

	void Application::Run()
	{
		LE_PROFILE_FUNCTION();

		while (bIsRunning)
		{
			LE_PROFILE_SCOPE("Engine Loop");

			// TODO: move glfw call to Platform::GetTime()
			float time = static_cast<float>(glfwGetTime());
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (m_Minimized == false)
			{
				LE_PROFILE_SCOPE("LayerStack OnUpdate");
				for (Layer* currentLayer : m_LayerStack)
				{
					currentLayer->OnUpdate(timestep);
				}
			}

			m_ImGuiLayer->Begin();
			{
				LE_PROFILE_SCOPE("LayerStack OnImGuiRender");
				for (Layer* currentLayer : m_LayerStack)
				{
					currentLayer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowCloseCallback(const WindowCloseEvent & e)
	{
		bIsRunning = false;
		return true;
	}

	bool Application::OnWindowResizeCallback(const WindowResizeEvent& e)
	{
		LE_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}