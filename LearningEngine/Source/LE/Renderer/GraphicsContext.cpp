#include "LEpch.h"
#include "GraphicsContext.h"

#include "LE/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace LE
{
	UniquePtr<GraphicsContext> GraphicsContext::Create(void* Window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				LE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return MakeUnique<OpenGLContext>(static_cast<GLFWwindow*>(Window));
			}
		}

		LE_CORE_ASSERT(false, "Unknown RendererAPI selected!");
		return nullptr;
	}
}