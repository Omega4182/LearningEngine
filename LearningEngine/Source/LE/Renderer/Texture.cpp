#include "LEpch.h"
#include "Texture.h"

#include "LE/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace LE
{
	SharedPtr<Texture2D> Texture2D::Create(uint32_t Width, uint32_t Height)
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
				return MakeShared<OpenGLTexture2D>(Width, Height);
			}
		}

		LE_CORE_ASSERT(false, "Unknown RendererAPI selected!");
		return nullptr;
	}

	SharedPtr<Texture2D> Texture2D::Create(const std::string& Path)
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
				return MakeShared<OpenGLTexture2D>(Path);
			}
		}

		LE_CORE_ASSERT(false, "Unknown RendererAPI selected!");
		return nullptr;
	}
}