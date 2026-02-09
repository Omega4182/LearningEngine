#pragma once

#include "LE/Core/Core.h"
#include <string>

namespace LE
{
	class Texture
	{
	public:

		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererId() const = 0;

		virtual void SetData(void* Data, size_t Size) = 0;

		virtual void Bind(uint32_t Slot) const = 0;

		virtual bool operator==(const Texture& Other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:

		virtual ~Texture2D() = default;

		static TSharedPtr<Texture2D> Create(uint32_t Width, uint32_t Height);
		static TSharedPtr<Texture2D> Create(const std::string& Path);
	};
}