#include "LEpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace LE
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         LE_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       LE_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          LE_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LE_CORE_TRACE(message); return;
		}

		LE_CORE_ASSERT(false, "Unknown severity level!");
	}

	OpenGLRendererAPI::OpenGLRendererAPI()
	{

	}

	void OpenGLRendererAPI::Init()
	{
#ifdef LE_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
	{
		glViewport(X, Y, Width, Height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& Color)
	{
		glClearColor(Color.r, Color.g, Color.b, Color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const SharedPtr<VertexArray>& VertexArray, uint32_t IndexCount)
	{
		if (IndexCount == 0)
		{
			glDrawElements(GL_TRIANGLES, VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
		}
	}
}