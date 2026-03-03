#include "LEpch.h"
#include "RendererAPI.h"

#include "LE/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace LE
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	UniquePtr<RendererAPI> RendererAPI::Create()
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
				return MakeUnique<OpenGLRendererAPI>();
			}
		}

		LE_CORE_ASSERT(false, "Unknown RendererAPI selected!");
		return nullptr;
	}
}