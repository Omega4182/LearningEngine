
#pragma once

#include <memory>

// ------

#ifdef _WIN32
	#ifdef _WIN64
		#define LE_PLATFORM_WINDOWS
	#else
		#error "x86 builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define LE_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define LE_PLATFORM_MAC
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define LE_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define LE_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif

// ------

#define BIT(x) (1 << x)

// ------

#ifdef LE_DEBUG
	#define LE_ENABLE_ASSERTS
#endif

#ifdef LE_ENABLE_ASSERTS
	#define LE_ASSERT(x, ...) { if ((x) == false) { LE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define LE_CORE_ASSERT(x, ...) { if ((x) == false) { LE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define LE_ASSERT(x, ...)
	#define LE_CORE_ASSERT(x, ...)
#endif

// ------

#define LE_BIND(ptr, fn) std::bind(fn, ptr, std::placeholders::_1)

// ------

namespace LE
{
// ------
	template<typename T>
	using TUniquePtr = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr TUniquePtr<T> MakeUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using TWeakPtr = std::weak_ptr<T>;

	template<typename T, typename ... Args>
	constexpr TWeakPtr<T> MakeWeak(Args&& ... args)
	{
		return std::weak_ptr<T>(std::forward<Args>(args)...);
	}

// ------

	enum {INDEX_NONE = -1 };
}