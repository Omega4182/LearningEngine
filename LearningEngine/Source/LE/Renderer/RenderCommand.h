#pragma once

#include "RendererAPI.h"

#include <glm/glm.hpp>

namespace LE
{
	class RenderCommand
	{
	public:

		inline static void Init()
		{
			s_RenderAPI->Init();
		}

		inline static void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
		{
			s_RenderAPI->SetViewport(X, Y, Width, Height);
		}

		inline static void SetClearColor(const glm::vec4& Color)
		{
			s_RenderAPI->SetClearColor(Color);
		}

		inline static void Clear()
		{
			s_RenderAPI->Clear();
		}

		inline static void DrawIndexed(const TSharedPtr<VertexArray>& VertexArray, uint32_t IndexCount = 0)
		{
			s_RenderAPI->DrawIndexed(VertexArray, IndexCount);
		}

	private:

		static TUniquePtr<RendererAPI> s_RenderAPI;
	};
}