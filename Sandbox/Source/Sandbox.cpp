
#include <LearningEngine.h>
#include <LE/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "LE/Events/ApplicationEvent.h"

#include <glm/gtc/matrix_transform.hpp>

// TEMP:
#include "Platform/OpenGL/OpenGLShader.h"

class Sandbox : public LE::Application
{
public:

	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
};

LE::Application* LE::CreateApplication()
{
	return new Sandbox();
}