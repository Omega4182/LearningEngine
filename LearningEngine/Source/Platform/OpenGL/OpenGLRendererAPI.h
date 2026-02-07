#pragma once

#include "LE/Renderer/RendererAPI.h"

namespace LE
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:

		OpenGLRendererAPI();

		virtual void Init() override;

		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) override;


		virtual void SetClearColor(const glm::vec4& Color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const TSharedPtr<VertexArray>& VertexArray, uint32_t IndexCount = 0) override;
	};
}