#pragma once

#include "LE/Core/Memory/SharedPtr.h"
#include "LE/Renderer/VertexArray.h"

namespace LE
{
	class OpenGLVertexArray : public VertexArray
	{
	public:

		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& VertexBuffer) override;
		virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& IndexBuffer) override;

		virtual inline const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const override
		{
			return m_VertexBuffers;
		}

		virtual inline const SharedPtr<IndexBuffer>& GetIndexBuffer() const override
		{
			return m_IndexBuffer;
		}

	private:

		uint32_t m_RendererId;
		uint32_t m_VertexBufferIndexOffset = 0;

		std::vector<SharedPtr<VertexBuffer>> m_VertexBuffers;
		SharedPtr<IndexBuffer> m_IndexBuffer;
	};
}