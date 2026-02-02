
#include "LEpch.h"
#include "WindowsWindow.h"
#include "LE/Events/MouseEvent.h"
#include "LE/Events/KeyEvent.h"
#include "LE/Events/ApplicationEvent.h"

#include "LE/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace LE
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int ErrorCode, const char* Msg)
	{
		LE_CORE_ERROR("GLFW Error ({0}): {1}", ErrorCode, Msg);
		LE_CORE_ASSERT(false, "");
	}

	TUniquePtr<Window> Window::Create(const WindowProps& Props)
	{
		return MakeUnique<WindowsWindow>(Props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& Props)
	{
		LE_PROFILE_FUNCTION();

		Init(Props);
	}

	WindowsWindow::~WindowsWindow()
	{
		LE_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& Props)
	{
		LE_PROFILE_FUNCTION();

		m_Data.Title = Props.Title;
		m_Data.Width = Props.Width;
		m_Data.Height = Props.Height;

		LE_CORE_INFO("Creating window {0} ({1}, {2})", Props.Title, Props.Width, Props.Height);

		if (s_GLFWInitialized == false)
		{
			LE_PROFILE_SCOPE("glfwInit")
			bool bSuccess = glfwInit();
			LE_CORE_ASSERT(bSuccess, "Failed to initialize GLFW!");

			glfwSetErrorCallback(&GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		{
			LE_PROFILE_SCOPE("glfwCreateWindow")

		#ifdef LE_DEBUG
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			{
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			}
		#endif

			m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}

		m_Context = OpenGLContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* Window, int Width, int Height)
			{
				WindowData& data = *(static_cast<WindowData*>(glfwGetWindowUserPointer(Window)));
				WindowResizeEvent resizeEvent = WindowResizeEvent(Width, Height);
				data.Width = Width;
				data.Height = Height;
				data.EventCallback(resizeEvent);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* Window)
			{
				WindowData& data = *(static_cast<WindowData*>(glfwGetWindowUserPointer(Window)));
				WindowCloseEvent closeEvent = WindowCloseEvent();
				data.EventCallback(closeEvent);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
			{
				WindowData& data = *(static_cast<WindowData*>(glfwGetWindowUserPointer(Window)));

				switch (Action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent keyPressedEvent = KeyPressedEvent(Key, Scancode, 0);
						data.EventCallback(keyPressedEvent);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent keyReleasedEvent = KeyReleasedEvent(Key, Scancode);
						data.EventCallback(keyReleasedEvent);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent keyPressedEvent = KeyPressedEvent(Key, Scancode, 1);
						data.EventCallback(keyPressedEvent);
						break;
					}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* Window, unsigned int Codepoint)
			{
				WindowData& data = *(static_cast<WindowData*>(glfwGetWindowUserPointer(Window)));
				KeyTypedEvent keyTypedEvent = KeyTypedEvent(Codepoint);
				data.EventCallback(keyTypedEvent);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* Window, int Button, int Action, int Mods)
			{
				WindowData& data = *(static_cast<WindowData*>(glfwGetWindowUserPointer(Window)));

				switch (Action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent keyPressedEvent = MouseButtonPressedEvent(Button);
						data.EventCallback(keyPressedEvent);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent keyReleasedEvent = MouseButtonReleasedEvent(Button);
						data.EventCallback(keyReleasedEvent);
						break;
					}
					case GLFW_REPEAT:
					{
						MouseButtonPressedEvent keyPressedEvent = MouseButtonPressedEvent(Button);
						data.EventCallback(keyPressedEvent);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* Window, double XOffset, double YOffset)
			{
				WindowData& data = *(static_cast<WindowData*>(glfwGetWindowUserPointer(Window)));
				MouseScrolledEvent mouseScrolledEvent = MouseScrolledEvent((float)XOffset, (float)YOffset);
				data.EventCallback(mouseScrolledEvent);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* Window, double XPos, double YPos)
			{
				WindowData& data = *(static_cast<WindowData*>(glfwGetWindowUserPointer(Window)));
				MouseMovedEvent mouseMovedEvent = MouseMovedEvent((float)XPos, (float)YPos);
				data.EventCallback(mouseMovedEvent);
			});
	}

	void WindowsWindow::Shutdown()
	{
		LE_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		LE_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}
	
	void WindowsWindow::SetVSync(bool bEnabled)
	{
		LE_PROFILE_FUNCTION();

		glfwSwapInterval(bEnabled);
		m_Data.bIsVSync = bEnabled;
	}
}