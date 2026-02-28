#pragma once

#include "LE/Core/Memory/SharedPtr.h"
#include "LE/Renderer/Buffers.h"

namespace LE
{
	class VertexArray : public SharedFromThis
	{
	public:

		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& VertexBuffer) = 0;
		virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& IndexBuffer) = 0;

		virtual inline const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual inline const SharedPtr<IndexBuffer>& GetIndexBuffer() const = 0;

		static SharedPtr<VertexArray> Create();
	};
}