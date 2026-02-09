#pragma once

#include "LE/Renderer/Texture.h"
#include <glad/glad.h>

namespace LE
{
	class OpenGLTexture2D : public Texture2D
	{
	public:

		OpenGLTexture2D(uint32_t Width, uint32_t Height);
		OpenGLTexture2D(const std::string& Path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererId() const override { return m_RendererId; }

		virtual void SetData(void* Data, size_t Size) override;

		virtual void Bind(uint32_t Slot) const override;

		virtual bool operator==(const Texture& Other) const override
		{
			return static_cast<const OpenGLTexture2D&>(Other).m_RendererId == m_RendererId;
		}

	private:

		std::string m_Path;

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		uint32_t m_RendererId;

		GLenum m_InternalFormat;
		GLenum m_DataFormat;
	};
}