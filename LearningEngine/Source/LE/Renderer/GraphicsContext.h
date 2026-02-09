#pragma once

namespace LE
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static TUniquePtr<GraphicsContext> Create(void* Window);
	};
}