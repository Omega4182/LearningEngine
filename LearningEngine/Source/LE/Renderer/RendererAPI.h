#pragma once

#include "LE/Renderer/VertexArray.h"
#include <glm/glm.hpp>

namespace LE
{
	class RendererAPI
	{
	public:

		enum class API
		{
			None = 0,
			OpenGL
		};

	public:

		virtual void Init() = 0;

		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) = 0;

		virtual void SetClearColor(const glm::vec4& Color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const TSharedPtr<VertexArray>& VertexArray, uint32_t IndexCount = 0) = 0;

		inline static API GetAPI() { return s_API; }
		static TUniquePtr<RendererAPI> Create();

	private:

		static API s_API;
	};
}