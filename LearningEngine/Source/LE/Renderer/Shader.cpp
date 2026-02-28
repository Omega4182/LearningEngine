#include "LEpch.h"
#include "Shader.h"

#include "LE/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace LE
{
	SharedPtr<Shader> Shader::Create(const std::string& FilePath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				LE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return MakeShared<OpenGLShader>(FilePath);
			}
		}

		LE_CORE_ASSERT(false, "Unknown RendererAPI selected!");
		return nullptr;
	}

	SharedPtr<Shader> Shader::Create(const std::string& Name, const std::string& VertexShader, const std::string& FragmentShader)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				LE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return MakeShared<OpenGLShader>(Name, VertexShader, FragmentShader);
			}
		}

		LE_CORE_ASSERT(false, "Unknown RendererAPI selected!");
		return nullptr;
	}

	void ShaderLibrary::Add(const SharedPtr<Shader>& Shader)
	{
		const std::string& shaderName = Shader->GetName();
		LE_CORE_ASSERT(!Exists(shaderName), "Shader already exists");
		m_Shaders[shaderName] = Shader;
	}

	SharedPtr<Shader> ShaderLibrary::Load(const std::string& FilePath)
	{
		SharedPtr<Shader> shader = Shader::Create(FilePath);
		Add(shader);
		return shader;
	}

	SharedPtr<Shader> ShaderLibrary::Get(const std::string& Name)
	{
		LE_CORE_ASSERT(Exists(Name), "Could not find shader with a given name");
		return m_Shaders[Name];
	}

	bool ShaderLibrary::Exists(const std::string& Name) const
	{
		return m_Shaders.find(Name) != m_Shaders.end();
	}
}