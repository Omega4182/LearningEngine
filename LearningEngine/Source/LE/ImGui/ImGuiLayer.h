#pragma once

#include "LE/Core/Layer.h"
#include "LE/Events/MouseEvent.h"
#include "LE/Events/ApplicationEvent.h"
#include "LE/Events/KeyEvent.h"

namespace LE
{
	class ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& Event) override;

		void Begin();
		void End();

		void SetBlockEvents(bool bBlockEvents) { m_BlockEvents = bBlockEvents; }

	private:
		float m_Time = 0.f;
		bool m_BlockEvents = true;
	};
}