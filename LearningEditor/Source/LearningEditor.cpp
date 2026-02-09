
#include <LearningEngine.h>
#include <LE/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace LE
{
	class LearningEditor : public Application
	{
	public:

		LearningEditor()
		{
			PushLayer(new EditorLayer());
		}
	};

	Application* CreateApplication()
	{
		return new LearningEditor();
	}
}
