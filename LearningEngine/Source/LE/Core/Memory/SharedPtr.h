#pragma once

#include <atomic>

namespace LE
{
	class SharedFromThis
	{
	public:
		SharedFromThis() = default;
		virtual ~SharedFromThis() = default;

	private:
		void IncrementRef()
		{
			++m_RefCount;
		}

		void DecrementRef()
		{
			--m_RefCount;
		}

		uint32_t GetRefCount() const { return m_RefCount.load(); }

		template<class T>
		friend class SharedPtr;

	private:
		std::atomic<uint32_t> m_RefCount = 0;
	};

	template<typename T>
	class SharedPtr
	{
	public:
		using ObjectType = T;

		SharedPtr() noexcept = default;

		SharedPtr(std::nullptr_t Nullptr) noexcept
			: m_Ptr(nullptr)
		{
		}

		explicit SharedPtr(ObjectType* NewPtr) noexcept
		{
			m_Ptr = NewPtr;
			IncrementRef();
		}

		template<typename U>
		requires std::is_convertible_v<U*, ObjectType*>
		explicit SharedPtr(U* NewPtr) noexcept
		{
			m_Ptr = static_cast<ObjectType*>(NewPtr);
			IncrementRef();
		}

		SharedPtr(const SharedPtr<ObjectType>& Other) noexcept
		{
			m_Ptr = Other.m_Ptr;
			IncrementRef();
		}

		template<typename U>
		requires std::is_convertible_v<U*, ObjectType*>
		SharedPtr(const SharedPtr<U>& Other) noexcept
		{
			m_Ptr = static_cast<ObjectType*>(Other.m_Ptr);
			IncrementRef();
		}

		SharedPtr(SharedPtr<ObjectType>&& Other) noexcept
		{
			m_Ptr = Other.m_Ptr;
			Other.m_Ptr = nullptr;
		}

		template<typename U>
		requires std::is_convertible_v<U*, ObjectType*>
		SharedPtr(SharedPtr<U>&& Other) noexcept
		{
			m_Ptr = static_cast<ObjectType*>(Other.m_Ptr);
			Other.m_Ptr = nullptr;
		}

		~SharedPtr() noexcept
		{
			DecrementRef();
		}

		SharedPtr& operator =(std::nullptr_t Nullptr) noexcept
		{
			if (m_Ptr != nullptr)
			{
				DecrementRef();

				m_Ptr = nullptr;
			}

			return *this;
		}

		SharedPtr& operator =(const SharedPtr<ObjectType>& Other) noexcept
		{
			if (*this != Other)
			{
				DecrementRef();
				Other.IncrementRef();

				m_Ptr = Other.m_Ptr;
			}

			return *this;
		}

		template<typename U>
		requires std::is_convertible_v<U*, ObjectType*>
		SharedPtr& operator =(const SharedPtr<U>& Other) noexcept
		{
			if (*this != Other)
			{
				DecrementRef();
				Other.IncrementRef();

				m_Ptr = static_cast<ObjectType*>(Other.m_Ptr);
			}

			return *this;
		}

		SharedPtr& operator =(SharedPtr<ObjectType>&& Other) noexcept
		{
			if (*this != Other)
			{
				DecrementRef();

				m_Ptr = Other.m_Ptr;
				Other.m_Ptr = nullptr;
			}

			return *this;
		}

		template<typename U>
		requires std::is_convertible_v<U*, ObjectType*>
		SharedPtr& operator =(SharedPtr<U>&& Other) noexcept
		{
			if (*this != Other)
			{
				DecrementRef();

				m_Ptr = static_cast<ObjectType*>(Other.m_Ptr);
				Other.m_Ptr = nullptr;
			}

			return *this;
		}

		bool operator ==(const SharedPtr<ObjectType>& Other) const noexcept
		{
			return m_Ptr == Other.m_Ptr;
		}

		bool operator !=(const SharedPtr<ObjectType>& Other) const noexcept
		{
			return !(*this == Other);
		}

		ObjectType* operator->() noexcept { return m_Ptr; }
		ObjectType* operator->() const noexcept { return m_Ptr; }

		ObjectType& operator*() noexcept { return *m_Ptr; }
		ObjectType& operator*() const noexcept { return *m_Ptr; }

		ObjectType* Get() noexcept { return m_Ptr; }
		ObjectType* Get() const noexcept { return m_Ptr; }

		[[nodiscard]] bool IsValid() const noexcept { return m_Ptr != nullptr; }

		void Reset() noexcept { DecrementRef(); }

	private:
		void IncrementRef() const noexcept
		{
			if (IsValid())
			{
				m_Ptr->IncrementRef();
			}
		}

		void DecrementRef() const noexcept
		{
			if (IsValid())
			{
				m_Ptr->DecrementRef();

				if (m_Ptr->GetRefCount() == 0)
				{
					delete m_Ptr;
					m_Ptr = nullptr;
				}
			}
		}

	private:
		template<class ObjectType>
		friend class SharedPtr;

		mutable ObjectType* m_Ptr = nullptr;
	};

	template<typename T, typename ... Args>
	SharedPtr<T> MakeShared(Args&& ... args)
	{
		return SharedPtr<T>(new T(std::forward<Args>(args) ...));
	}
}
