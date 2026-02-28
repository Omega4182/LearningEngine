#pragma once

namespace LE
{
	enum class ShaderDataType : uint32_t
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType Type)
	{
		switch (Type)
		{
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}

		LE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		BufferElement(ShaderDataType InType, const std::string& InName, bool bInNormalized)
			: Type(InType),  Name(InName), Size(ShaderDataTypeSize(InType)), Offset(0), bNormalized(bInNormalized)
		{
		}

		ShaderDataType Type;
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		bool bNormalized;

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			LE_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:

		VertexBufferLayout() {}
		VertexBufferLayout(const std::initializer_list<BufferElement>& InElements);

		inline const std::vector<BufferElement>& GetElements() const
		{
			return m_Elements;
		}

		inline uint32_t GetStride() const
		{
			return m_Stride;
		}

	private:

		void CalculateOffsetsAndStride();

	private:

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};
}