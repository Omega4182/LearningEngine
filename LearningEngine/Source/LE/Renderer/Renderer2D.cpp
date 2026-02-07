#include "LEpch.h"
#include "Renderer2D.h"

#include "LE/Renderer/VertexArray.h"
#include "LE/Renderer/Shader.h"
#include "LE/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace LE
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex = 0.f;
		float TilingFactor = 0.f;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: Analyze GPU for this

		TSharedPtr<VertexArray> VertexArray;
		TSharedPtr<VertexBuffer> VertexBuffer;
		TSharedPtr<Shader> TextureShader;
		TSharedPtr<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<TSharedPtr<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		LE_PROFILE_FUNCTION();

		s_Data.VertexArray = VertexArray::Create();

		s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		VertexBufferLayout Layout = {
			{ ShaderDataType::Float3, "a_Position", false },
			{ ShaderDataType::Float4, "a_Color", false },
			{ ShaderDataType::Float2, "a_TexCoord", false },
			{ ShaderDataType::Float, "a_TexIndex", false },
			{ ShaderDataType::Float, "a_TilingFactor", false }
		};
		s_Data.VertexBuffer->SetLayout(Layout);
		s_Data.VertexArray->AddVertexBuffer(s_Data.VertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* QuadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t IndexOffset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			QuadIndices[i + 0] = IndexOffset + 0;
			QuadIndices[i + 1] = IndexOffset + 1;
			QuadIndices[i + 2] = IndexOffset + 2;

			QuadIndices[i + 3] = IndexOffset + 2;
			QuadIndices[i + 4] = IndexOffset + 3;
			QuadIndices[i + 5] = IndexOffset + 0;

			IndexOffset += 4;
		}

		TSharedPtr<IndexBuffer> indexBuffer = IndexBuffer::Create(QuadIndices, s_Data.MaxIndices);
		s_Data.VertexArray->SetIndexBuffer(indexBuffer);
		delete[] QuadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t Samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			Samplers[i] = i;
		}

		s_Data.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", Samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer2D::Shutdown()
	{
		LE_PROFILE_FUNCTION();


	}

	void Renderer2D::BeginScene(const OrthographicCamera& Camera)
	{
		LE_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", Camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		LE_PROFILE_FUNCTION();

		uint32_t DataSize = reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferBase);
		s_Data.VertexBuffer->SetData(s_Data.QuadVertexBufferBase, DataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.VertexArray, s_Data.QuadIndexCount);
	}

	// Primitives

	void Renderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Color)
	{
		DrawQuad(glm::vec3(Position, 0.f), Size, Color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Color)
	{
		LE_PROFILE_FUNCTION();

		const float TextureIndex = 0.f; // White texture
		const float TilingFactor = 1.f;

		s_Data.QuadVertexBufferPtr->Position = Position;
		s_Data.QuadVertexBufferPtr->Color = Color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { Position.x + Size.x, Position.y, 0.f };
		s_Data.QuadVertexBufferPtr->Color = Color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { Position.x + Size.x, Position.y + Size.y, 0.f };;
		s_Data.QuadVertexBufferPtr->Color = Color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { Position.x, Position.y + Size.y, 0.f };;
		s_Data.QuadVertexBufferPtr->Color = Color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const TSharedPtr<Texture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
	{
		DrawQuad(glm::vec3(Position, 0.f), Size, Texture, TilingFactor, TintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const TSharedPtr<Texture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
	{
		LE_PROFILE_FUNCTION();

		float TextureIndex = 0.f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			// Getting operator== overload to work
			// TODO: Remove this, looks ugly
			if (*s_Data.TextureSlots[i].get() == *Texture.get())
			{
				TextureIndex = static_cast<float>(i);
				break;
			}
		}

		if (TextureIndex == 0.f)
		{
			TextureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = Texture;
			s_Data.TextureSlotIndex++;
		}

		s_Data.QuadVertexBufferPtr->Position = Position;
		s_Data.QuadVertexBufferPtr->Color = TintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { Position.x + Size.x, Position.y, 0.f };
		s_Data.QuadVertexBufferPtr->Color = TintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { Position.x + Size.x, Position.y + Size.y, 0.f };;
		s_Data.QuadVertexBufferPtr->Color = TintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { Position.x, Position.y + Size.y, 0.f };;
		s_Data.QuadVertexBufferPtr->Color = TintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color)
	{
		DrawRotatedQuad(glm::vec3(Position, 0.f), Size, Rotation, Color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color)
	{
		LE_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.WhiteTexture->Bind(0);

		s_Data.TextureShader->SetFloat4("u_Color", glm::vec4(1.f));
		//s_Data->TextureShader->SetFloat("u_TilingFactor", TilingFactor);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), Position) * 
			glm::rotate(glm::mat4(1.f), Rotation, glm::vec3(0.f, 0.f, 1.f)) *
			glm::scale(glm::mat4(1.f), glm::vec3(Size, 1.f));
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.VertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const TSharedPtr<Texture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
	{
		DrawRotatedQuad(glm::vec3(Position, 0.f), Size, Rotation, Texture, TilingFactor, TintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const TSharedPtr<Texture2D>& Texture, float TilingFactor, const glm::vec4& TintColor)
	{
		LE_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		Texture->Bind(0);

		s_Data.TextureShader->SetFloat4("u_Color", TintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", TilingFactor);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), Position) *
			glm::rotate(glm::mat4(1.f), Rotation, glm::vec3(0.f, 0.f, 1.f)) * 
			glm::scale(glm::mat4(1.f), glm::vec3(Size, 1.f));
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.VertexArray);
	}
}
