#pragma once

#include "LE/Core/Memory/UniquePtr.h"

namespace LE
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static UniquePtr<GraphicsContext> Create(void* Window);
	};
}