
#include "LEpch.h"
#include "LE/Core/Input.h"

#include "LE/Core/Application.h"

#include <GLFW/glfw3.h>

namespace LE
{
	bool Input::IsKeyPressed(int32_t Keycode)
	{
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int keyResult = glfwGetKey(nativeWindow, Keycode);
		return keyResult == GLFW_PRESS || keyResult == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int32_t Button)
	{
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int mouseButtonResult = glfwGetMouseButton(nativeWindow, Button);
		return mouseButtonResult == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMouseXY()
	{
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(nativeWindow, &xpos, &ypos);

		return std::make_pair<float, float>(static_cast<float>(xpos), static_cast<float>(ypos));
	}

	float Input::GetMouseX()
	{
		auto[x, y] = GetMouseXY();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMouseXY();
		return y;
	}
}
