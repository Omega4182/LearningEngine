#pragma once

#include "VertexBufferLayout.h"

namespace LE
{
	class VertexBuffer
	{
	public:

		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* Data, uint32_t Size) = 0;

		virtual inline const VertexBufferLayout& GetBufferLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& Layout) = 0;

		static TSharedPtr<VertexBuffer> Create(uint32_t Size);
		static TSharedPtr<VertexBuffer> Create(float* Vertices, uint32_t Size);
	};

	class IndexBuffer
	{
	public:

		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static TSharedPtr<IndexBuffer> Create(uint32_t* Indices, uint32_t Count);
	};
}