#include "LEpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace LE
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		LE_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		LE_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		LE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		LE_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	static GLenum ShaderDataTypeToOpenGLDataType(ShaderDataType Type)
	{
		switch (Type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		LE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	void OpenGLVertexArray::AddVertexBuffer(const SharedPtr<VertexBuffer>& VertexBuffer)
	{
		LE_PROFILE_FUNCTION();

		LE_CORE_ASSERT(VertexBuffer->GetBufferLayout().GetElements().size() > 0, "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererId);
		VertexBuffer->Bind(); 

		const uint32_t layoutElementsCount = (uint32_t)VertexBuffer->GetBufferLayout().GetElements().size();
		for (uint32_t i = 0; i < layoutElementsCount; i++)
		{
			const BufferElement& element = VertexBuffer->GetBufferLayout().GetElements()[i];
			glEnableVertexAttribArray(i + m_VertexBufferIndexOffset);
			glVertexAttribPointer(
				i + m_VertexBufferIndexOffset,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLDataType(element.Type),
				element.bNormalized ? GL_TRUE : GL_FALSE,
				VertexBuffer->GetBufferLayout().GetStride(),
				reinterpret_cast<const void*>((uintptr_t)element.Offset));
		}

		m_VertexBuffers.push_back(VertexBuffer);
		m_VertexBufferIndexOffset += layoutElementsCount;
	}

	void OpenGLVertexArray::SetIndexBuffer(const SharedPtr<IndexBuffer>& IndexBuffer)
	{
		LE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererId);
		IndexBuffer->Bind();
		m_IndexBuffer = IndexBuffer;
	}
}