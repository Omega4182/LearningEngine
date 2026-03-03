#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

#include <filesystem>

// Resulting JSON file can be loaded into chrome://tracing. Open it in GoogleChrome browser

// TODO: Add mutex to WriteProfile function to lock multiple threads writing into the same file at the same time
// TODO: Support multiple debug sessions
// TODO: Replace chrono clock with platform-specific clock systems for more precision
// TODO: Move LE_PROFILING to configuration

#define LE_PROFILING

#ifdef LE_PROFILING
	#define LE_PROFILE_BEGIN_SESSION(filename)	::LE::Instrumentor::Get().BeginSession(filename);
	#define LE_PROFILE_END_SESSION()	::LE::Instrumentor::Get().EndSession();
	#define LE_PROFILE_SCOPE(name)	::LE::InstrumentationTimer timer##__LINE__(name);
	#define LE_PROFILE_FUNCTION()	LE_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define LE_PROFILE_BEGIN_SESSION()
	#define LE_PROFILE_END_SESSION()
	#define LE_PROFILE_SCOPE(name)
	#define LE_PROFILE_FUNCTION()
#endif

#ifdef LE_PROFILING

namespace LE
{
	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadId;
	};

	class Instrumentor
	{
	private:

		Instrumentor()
			: m_IsSessionActive(false), m_ProfileCount(0)
		{
		}

	public:

		~Instrumentor()
		{
			EndSession();
		}

		void BeginSession(const std::string& FileName = "ProfileResults.json")
		{
			if (m_IsSessionActive == false)
			{
				m_IsSessionActive = true;

				// TODO: find SolutionDirectory better? Make SOLUTION_DIR macro? What is the best way?
				std::filesystem::path solDir = std::filesystem::current_path().parent_path();
				std::filesystem::path profilingDir = solDir / "Saved\\Profiling";
				std::filesystem::create_directories(profilingDir);

				m_OutputStream.open(profilingDir / FileName);
				WriteHeader();
			}
		}

		void EndSession()
		{
			if (m_IsSessionActive)
			{
				WriteFooter();
				m_OutputStream.close();
				m_IsSessionActive = false;
				m_ProfileCount = 0;
			}
		}

		void WriteProfile(const ProfileResult& Result)
		{
			if (m_ProfileCount > 0)
			{
				m_OutputStream << ",";
			}
			m_ProfileCount++;

			std::string name = Result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');
			
			size_t callingConvStrLoc = name.find("__cdecl");
			if (callingConvStrLoc != std::string::npos)
			{
				name.replace(callingConvStrLoc, sizeof("__cdecl"), "");
			}

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << Result.End - Result.Start << ",";
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << Result.ThreadId << ",";
			m_OutputStream << "\"ts\":" << Result.Start;
			m_OutputStream << "}";
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:

		bool m_IsSessionActive;
		std::ofstream m_OutputStream;
		uint32_t m_ProfileCount;
	};

	class InstrumentationTimer
	{
	public:

		InstrumentationTimer(const char* Name)
			: m_Stopped(false)
		{
			m_ProfileResult.Name = Name;
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (m_Stopped == false)
			{
				Stop();
			}
		}

		void Stop()
		{
			std::chrono::time_point<std::chrono::steady_clock> endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());

			m_ProfileResult.Start = start;
			m_ProfileResult.End = end;
			m_ProfileResult.ThreadId = static_cast<uint32_t>(threadId);

			Instrumentor::Get().WriteProfile(m_ProfileResult);

			m_Stopped = true;
		}

	private:

		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
		ProfileResult m_ProfileResult;
	};
}

#endif