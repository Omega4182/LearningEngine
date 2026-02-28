#include "LEpch.h"
#include "Renderer.h"
#include "LE/Renderer/Renderer2D.h"

namespace LE
{
	TUniquePtr<Renderer::SceneData> Renderer::s_SceneData = MakeUnique<Renderer::SceneData>();

	void Renderer::Init()
	{
		LE_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t Width, uint32_t Height)
	{
		RenderCommand::SetViewport(0, 0, Width, Height);
	}

	void Renderer::BeginScene(const OrthographicCamera& Camera)
	{
		s_SceneData->ViewProjectionMatrix = Camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const SharedPtr<Shader>& Shader, const SharedPtr<VertexArray>& VertexArray, const glm::mat4& Transform)
	{
		Shader->Bind();
		Shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		Shader->SetMat4("u_Transform", Transform);

		VertexArray->Bind();
		RenderCommand::DrawIndexed(VertexArray);
	}
}