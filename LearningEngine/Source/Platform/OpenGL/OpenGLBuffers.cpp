#include "LEpch.h"
#include "OpenGLBuffers.h"

#include <glad/glad.h>

namespace LE
{
	/* Vertex Buffer */

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t Size)
	{
		LE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, Size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* Vertices, uint32_t Size)
	{
		LE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, Size, Vertices, GL_STATIC_DRAW);
	}
	
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		LE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		LE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		LE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* Data, uint32_t Size)
	{
		LE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, Size, Data);
	}

	/* Index Buffer */

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* Indices, uint32_t Count)
		: m_Count(Count)
	{
		LE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(uint32_t), Indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		LE_PROFILE_FUNCTION();

		glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_RendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		LE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		LE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}