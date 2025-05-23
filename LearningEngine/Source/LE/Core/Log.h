
#pragma once

#include "LE/Core/Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace LE
{
	class Log
	{
	public:

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() 
		{ 
			return s_CoreLogger;
		};

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger; 
		};

	private:

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// TODO: Make this similar to UE macro, like LE_LOG(LogTemp, Warning, TEXT("message"));
//		 Client/Core should be determined by LE_BUILD_DLL macro, see Core.h

#define LE_CORE_ERROR(...)   LE::Log::GetCoreLogger()->error(__VA_ARGS__);
#define LE_CORE_WARN(...)    LE::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define LE_CORE_INFO(...)    LE::Log::GetCoreLogger()->info(__VA_ARGS__);
#define LE_CORE_TRACE(...)   LE::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define LE_CORE_CRITICAL(...)   LE::Log::GetCoreLogger()->critical(__VA_ARGS__);

#define LE_ERROR(...)        LE::Log::GetClientLogger()->error(__VA_ARGS__);
#define LE_WARN(...)         LE::Log::GetClientLogger()->warn(__VA_ARGS__);
#define LE_INFO(...)         LE::Log::GetClientLogger()->info(__VA_ARGS__);
#define LE_TRACE(...)        LE::Log::GetClientLogger()->trace(__VA_ARGS__);
#define LE_CRITICAL(...)        LE::Log::GetClientLogger()->critical(__VA_ARGS__);