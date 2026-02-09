
#pragma once

#include "LE/Core/Core.h"

namespace LE
{
	class Input
	{
	public:
		static bool IsKeyPressed(int32_t Keycode);
		static bool IsMouseButtonPressed(int32_t Button);

		static std::pair<float, float> GetMouseXY();
		static float GetMouseX();

		static float GetMouseY();
	};
}