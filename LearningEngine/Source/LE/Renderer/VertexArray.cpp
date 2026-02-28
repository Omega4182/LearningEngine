#include "LEpch.h"
#include "VertexArray.h"

#include "LE/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace LE
{
	SharedPtr<VertexArray> VertexArray::Create()
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
				return MakeShared<OpenGLVertexArray>();
			}
		}

		LE_CORE_ASSERT(false, "Unknown RendererAPI selected!");
		return nullptr;
	}
}