
#pragma once

#include "LE/Core/Core.h"

namespace LE
{
	class Input
	{
	protected:

		Input() = default;

	public:

		virtual ~Input() = default;

		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(int32_t Keycode)
		{
			return s_Instance->IsKeyPressedImpl(Keycode);
		}

		inline static bool IsMouseButtonPressed(int32_t Button)
		{
			return s_Instance->IsMouseButtonPressedImpl(Button);
		}

		inline static std::pair<float, float> GetMouseXY()
		{
			return s_Instance->GetMouseXYImpl();
		}

		inline static float GetMouseX()
		{
			return GetMouseXY().first;
		}

		inline static float GetMouseY()
		{
			return GetMouseXY().second;
		}

	protected:

		virtual bool IsKeyPressedImpl(int32_t Keycode) const = 0;
		virtual bool IsMouseButtonPressedImpl(int32_t Button) const = 0;

		virtual std::pair<float, float> GetMouseXYImpl() const = 0;

	private:

		static TUniquePtr<Input> s_Instance;
	};
}