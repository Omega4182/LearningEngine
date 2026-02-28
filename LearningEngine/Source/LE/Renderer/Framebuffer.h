#pragma once

#include "LE/Core/Memory/SharedPtr.h"

namespace LE
{
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool bSwapChainTarget = false;
	};

	class Framebuffer : public SharedFromThis
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t Width, uint32_t Height) = 0;

		virtual uint32_t GetColorAttachmentRendererId() const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static SharedPtr<Framebuffer> Create(const FramebufferSpecification& Spec);
	};
}