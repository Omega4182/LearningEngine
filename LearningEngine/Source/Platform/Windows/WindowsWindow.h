
#pragma once

#include "LE/Core/Window.h"
#include "LE/Renderer/GraphicsContext.h"
#include "LE/Core/Memory/UniquePtr.h"
#include <GLFW/glfw3.h>

namespace LE
{
	class WindowsWindow : public Window
	{
	public:

		WindowsWindow(const WindowProps& Props);
		virtual ~WindowsWindow();
		
		virtual void OnUpdate() override;

		inline virtual uint32_t GetWidth() const override
		{
			return m_Data.Width;
		}

		inline virtual uint32_t GetHeight() const override
		{
			return m_Data.Height;
		}

		inline virtual void SetEventCallback(const EventCallbackFn& Callback) override
		{
			m_Data.EventCallback = Callback;
		}

		virtual void SetVSync(bool bEnabled) override;
		virtual bool IsVSync() const override
		{
			return m_Data.bIsVSync;
		}

		inline virtual void* GetNativeWindow() const override
		{
			return m_Window;
		}

	protected:

		virtual void Init(const WindowProps& Props);
		virtual void Shutdown();

	private:

		GLFWwindow* m_Window;
		UniquePtr<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool bIsVSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}