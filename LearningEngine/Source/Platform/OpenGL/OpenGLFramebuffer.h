#pragma once

#include "LE/Renderer/Framebuffer.h"

namespace LE
{
	class OpenGLFramebuffer final : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& Spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t Width, uint32_t Height) override;

		virtual uint32_t GetColorAttachmentRendererId() const override { return m_ColorAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Spec; }

	private:
		void Invalidate();

	private:
		uint32_t m_RendererId = INDEX_NONE;
		uint32_t m_ColorAttachment = INDEX_NONE;
		uint32_t m_DepthAttachment = INDEX_NONE;
		FramebufferSpecification m_Spec;
	};
}