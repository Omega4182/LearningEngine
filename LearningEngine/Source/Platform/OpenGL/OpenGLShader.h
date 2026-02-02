#pragma once

#include "LE/Renderer/Shader.h"

#include <glm/glm.hpp>

#include <string>

// TODO: Remove ASAP
typedef unsigned int GLenum;

namespace LE
{
	class OpenGLShader : public Shader
	{
	public:

		OpenGLShader(const std::string& FilePath);
		OpenGLShader(const std::string& Name, const std::string& VertexShader, const std::string& FragmentShader);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& Name, int32_t Value) override;
		virtual void SetFloat(const std::string& Name, float Value) override;
		virtual void SetFloat3(const std::string& Name, const glm::vec3& Value) override;
		virtual void SetFloat4(const std::string& Name, const glm::vec4& Value) override;
		virtual void SetMat4(const std::string& Name, const glm::mat4& Value) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string& Name, int32_t Integer) const;

		void UploadUniformFloat(const std::string& Name, float Value) const;
		void UploadUniformFloat2(const std::string& Name, const glm::vec2& Value) const;
		void UploadUniformFloat3(const std::string& Name, const glm::vec3& Value) const;
		void UploadUniformFloat4(const std::string& Name, const glm::vec4& Value) const;

		void UploadUniformMat3(const std::string& Name, const glm::mat3& Matrix) const;
		void UploadUniformMat4(const std::string& Name, const glm::mat4& Matrix) const;

	private:

		std::string ReadFile(const std::string& FilePath) const;
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& Source);
		void Compile(const std::unordered_map<GLenum, std::string>& ShaderSrc);

	private:

		uint32_t m_RendererId = 0;

		std::string m_Name;
	};
}