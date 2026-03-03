
#pragma once

#include "LEpch.h"

#include "LE/Core/Memory/UniquePtr.h"
#include "LE/Events/Event.h"

namespace LE
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& Title = "Learning Engine", uint32_t Width = 1280, uint32_t Height = 720)
			: Title(Title), Width(Width), Height(Height) {}
	};

	class Window
	{
	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		inline virtual uint32_t GetWidth() const = 0;
		inline virtual uint32_t GetHeight() const = 0;

		inline virtual void SetEventCallback(const EventCallbackFn& Callback) = 0;
		virtual void SetVSync(bool bEnabled) = 0;
		virtual bool IsVSync() const = 0;

		inline virtual void* GetNativeWindow() const = 0;

		static UniquePtr<Window> Create(const WindowProps& props = WindowProps());
	};
}
