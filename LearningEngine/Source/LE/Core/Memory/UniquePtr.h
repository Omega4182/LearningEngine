#pragma once

#include <type_traits>

namespace LE
{
	template <typename T>
	class UniquePtr
	{
	public:
		using ObjectType = T;

		UniquePtr() noexcept = default;

		UniquePtr(std::nullptr_t Nullptr)
			: m_Ptr(nullptr)
		{
		}

		explicit UniquePtr(ObjectType* NewPtr) noexcept
			: m_Ptr(NewPtr)
		{
		}

		template<typename U>
		requires std::is_convertible_v<U*, ObjectType*>
		explicit UniquePtr(U* NewPtr) noexcept
		{
			m_Ptr = static_cast<ObjectType*>(NewPtr);
		}

		UniquePtr(const UniquePtr&) = delete;
		UniquePtr& operator =(const UniquePtr&) = delete;

		UniquePtr(UniquePtr<ObjectType>&& NewPtr) noexcept
		{
			m_Ptr = NewPtr.Release();
		}

		template <typename U>
		requires std::is_convertible_v<U*, ObjectType*>
		UniquePtr(UniquePtr<U>&& NewPtr) noexcept
		{
			m_Ptr = static_cast<ObjectType*>(NewPtr.Release());
		}

		~UniquePtr()
		{
			Reset();
		}

		UniquePtr& operator =(std::nullptr_t Nullptr) noexcept
		{
			Reset();
			return *this;
		}

		UniquePtr& operator =(UniquePtr<ObjectType>&& NewPtr) noexcept
		{
			if (*this != NewPtr)
			{
				Reset();
				m_Ptr = NewPtr.Release();
			}

			return *this;
		}

		template <typename U>
		requires std::is_convertible_v<U*, ObjectType*>
		UniquePtr& operator =(UniquePtr<U>&& NewPtr) noexcept
		{
			if (*this != NewPtr)
			{
				Reset();
				m_Ptr = static_cast<ObjectType*>(NewPtr.Release());
			}

			return *this;
		}

		bool operator ==(const UniquePtr<ObjectType>& Other) const noexcept
		{ 
			return m_Ptr == Other.m_Ptr;
		}

		bool operator !=(const UniquePtr<ObjectType>& Other) const noexcept
		{ 
			return !(*this == Other);
		}

		ObjectType* operator->() noexcept { return m_Ptr; }
		ObjectType* operator->() const noexcept { return m_Ptr; }

		ObjectType& operator*() noexcept { return *m_Ptr; }
		ObjectType& operator*() const noexcept { return *m_Ptr; }

		ObjectType* Get() noexcept { return m_Ptr; }
		ObjectType* Get() const noexcept { return m_Ptr; }

		[[nodiscard]] bool IsValid() const noexcept
		{
			return m_Ptr != nullptr;
		}

		[[nodiscard]] ObjectType* Release() const noexcept
		{
			ObjectType* OldPtr = m_Ptr;
			m_Ptr = nullptr;
			return OldPtr;
		}

		void Reset() const noexcept
		{
			if (IsValid())
			{
				delete m_Ptr;
				m_Ptr = nullptr;
			}
		}

	private:
		mutable ObjectType* m_Ptr = nullptr;
	};

	template<typename T, typename ... Args>
	UniquePtr<T> MakeUnique(Args&& ... args)
	{
		return UniquePtr<T>(new T(std::forward<Args>(args) ...));
	}
}