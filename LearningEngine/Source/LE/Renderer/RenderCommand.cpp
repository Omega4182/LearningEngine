#include "LEpch.h"
#include "RenderCommand.h"

namespace LE
{
	UniquePtr<RendererAPI> RenderCommand::s_RenderAPI = RendererAPI::Create();
}