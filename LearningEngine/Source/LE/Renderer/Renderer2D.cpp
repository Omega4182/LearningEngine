#include "LEpch.h"
#include "Renderer2D.h"

#include "LE/Renderer/VertexArray.h"
#include "LE/Renderer/Shader.h"
#include "LE/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace LE
{
	struct Renderer2DStorage
	{
		TSharedPtr<VertexArray> VertexArray;
		TSharedPtr<Shader> TextureShader;
		TSharedPtr<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		LE_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		s_Data->VertexArray = VertexArray::Create();
		s_Data->TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);

		float vb[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.f, 0.f,
			 0.5f, -0.5f, 0.0f, 1.f, 0.f,
			 0.5f,  0.5f, 0.0f, 1.f, 1.f,
			-0.5f,  0.5f, 0.0f, 0.f, 1.f
		};

		TSharedPtr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vb, sizeof(vb));

		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position", false },
			{ ShaderDataType::Float2, "a_TexCoord", false }
		};

		vertexBuffer->SetLayout(layout);
		s_Data->VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t ib[6] = { 0, 1, 2, 2, 3, 0 };
		TSharedPtr<IndexBuffer> indexBuffer = IndexBuffer::Create(ib, 6);
		s_Data->VertexArray->SetIndexBuffer(indexBuffer);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));
	}

	void Renderer2D::Shutdown()
	{
		LE_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& Camera)
	{
		LE_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", Camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		LE_PROFILE_FUNCTION();

	}

	// Primitives

	void Renderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Color)
	{
		DrawQuad(glm::vec3(Position, 0.f), Size, Color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Color)
	{
		LE_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->WhiteTexture->Bind(0);

		s_Data->TextureShader->SetFloat4("u_Color", Color);
		//s_Data->TextureShader->SetFloat("u_TilingFactor", 10.f);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), Position) * glm::scale(glm::mat4(1.f), glm::vec3(Size, 1.f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const TSharedPtr<Texture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
	{
		DrawQuad(glm::vec3(Position, 0.f), Size, Texture, TilingFactor, TintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const TSharedPtr<Texture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
	{
		LE_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		Texture->Bind(0);

		s_Data->TextureShader->SetFloat4("u_Color", TintColor);
		s_Data->TextureShader->SetFloat("u_TilingFactor", TilingFactor);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), Position) * glm::scale(glm::mat4(1.f), glm::vec3(Size, 1.f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color)
	{
		DrawRotatedQuad(glm::vec3(Position, 0.f), Size, Rotation, Color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color)
	{
		LE_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->WhiteTexture->Bind(0);

		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.f));
		//s_Data->TextureShader->SetFloat("u_TilingFactor", TilingFactor);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), Position) * 
			glm::rotate(glm::mat4(1.f), Rotation, glm::vec3(0.f, 0.f, 1.f)) *
			glm::scale(glm::mat4(1.f), glm::vec3(Size, 1.f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const TSharedPtr<Texture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
	{
		DrawRotatedQuad(glm::vec3(Position, 0.f), Size, Rotation, Texture, TilingFactor, TintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const TSharedPtr<Texture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
	{
		LE_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		Texture->Bind(0);

		s_Data->TextureShader->SetFloat4("u_Color", TintColor);
		s_Data->TextureShader->SetFloat("u_TilingFactor", TilingFactor);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), Position) *
			glm::rotate(glm::mat4(1.f), Rotation, glm::vec3(0.f, 0.f, 1.f)) * 
			glm::scale(glm::mat4(1.f), glm::vec3(Size, 1.f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}
}
