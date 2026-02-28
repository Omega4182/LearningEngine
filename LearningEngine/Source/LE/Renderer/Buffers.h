#pragma once

#include "LE/Core/Memory/SharedPtr.h"
#include "VertexBufferLayout.h"

namespace LE
{
	class VertexBuffer : public SharedFromThis
	{
	public:

		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* Data, uint32_t Size) = 0;

		virtual inline const VertexBufferLayout& GetBufferLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& Layout) = 0;

		static SharedPtr<VertexBuffer> Create(uint32_t Size);
		static SharedPtr<VertexBuffer> Create(float* Vertices, uint32_t Size);
	};

	class IndexBuffer : public SharedFromThis
	{
	public:

		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static SharedPtr<IndexBuffer> Create(uint32_t* Indices, uint32_t Count);
	};
}