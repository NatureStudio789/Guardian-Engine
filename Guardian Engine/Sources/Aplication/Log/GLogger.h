#ifndef _GE_GLOGGER_H_
#define _GE_GLOGGER_H_
#include "../Util/GUtil.h"

namespace GE
{
	class GUARDIAN_API GLogger
	{
	public:
		enum Category
		{
			GE_ENGINE_LOG,
			GE_CLIENT_LOG,
		};

		enum Level
		{
			GE_LOG_TRACE = SPDLOG_LEVEL_TRACE,
			GE_LOG_DEBUG = SPDLOG_LEVEL_DEBUG,
			GE_LOG_INFO = SPDLOG_LEVEL_INFO,
			GE_LOG_WARN = SPDLOG_LEVEL_WARN,
			GE_LOG_ERROR = SPDLOG_LEVEL_ERROR,
			GE_LOG_CIRITICAL = SPDLOG_LEVEL_CRITICAL,
			GE_LOG_OFF = SPDLOG_LEVEL_OFF,
		};

	public:
		static void InitializeLogger();

		template<typename... Args>
		static void LogMessage(Category category, Level level, spdlog::format_string_t<Args...> fmt, Args &&... args);
		template<typename... Args>
		static void LogTrace(Category category, spdlog::format_string_t<Args...> fmt, Args &&... args);
		template<typename... Args>
		static void LogDebug(Category category, spdlog::format_string_t<Args...> fmt, Args &&... args);
		template<typename... Args>
		static void LogInformation(Category category, spdlog::format_string_t<Args...> fmt, Args &&... args);
		template<typename... Args>
		static void LogWarning(Category category, spdlog::format_string_t<Args...> fmt, Args &&... args);
		template<typename... Args>
		static void LogError(Category category, spdlog::format_string_t<Args...> fmt, Args &&... args);
		template<typename... Args>
		static void LogCritical(Category category, spdlog::format_string_t<Args...> fmt, Args &&... args);

		static std::shared_ptr<spdlog::logger> GetEngineLogger();
		static std::shared_ptr<spdlog::logger> GetClientLogger(); 
		
	private:
		static std::shared_ptr<spdlog::logger> EngineLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;
	};

	template<typename ...Args>
	inline void GLogger::LogMessage(Category category, Level level, spdlog::format_string_t<Args...> fmt, Args && ...args)
	{
		switch (category)
		{
			case GE_ENGINE_LOG:
			{
				EngineLogger->log((spdlog::level::level_enum)level, fmt, std::forward<Args>(args)...);
				break;
			}

			case GE_CLIENT_LOG:
			{
				ClientLogger->log((spdlog::level::level_enum)level, fmt, std::forward<Args>(args)...);
				break;
			}
		};
	}

	template<typename ...Args>
	inline void GLogger::LogTrace(Category category, spdlog::format_string_t<Args...> fmt, Args && ...args)
	{
		switch (category)
		{
		case GE_ENGINE_LOG:
		{
			EngineLogger->trace(fmt, std::forward<Args>(args)...);
			break;
		}

		case GE_CLIENT_LOG:
		{
			ClientLogger->trace(fmt, std::forward<Args>(args)...);
			break;
		}
		};
	}

	template<typename ...Args>
	inline void GLogger::LogDebug(Category category, spdlog::format_string_t<Args...> fmt, Args && ...args)
	{
		switch (category)
		{
		case GE_ENGINE_LOG:
		{
			EngineLogger->debug(fmt, std::forward<Args>(args)...);
			break;
		}

		case GE_CLIENT_LOG:
		{
			ClientLogger->debug(fmt, std::forward<Args>(args)...);
			break;
		}
		};
	}

	template<typename ...Args>
	inline void GLogger::LogInformation(Category category, spdlog::format_string_t<Args...> fmt, Args && ...args)
	{
		switch (category)
		{
		case GE_ENGINE_LOG:
		{
			EngineLogger->info(fmt, std::forward<Args>(args)...);
			break;
		}

		case GE_CLIENT_LOG:
		{
			ClientLogger->info(fmt, std::forward<Args>(args)...);
			break;
		}
		};
	}

	template<typename ...Args>
	inline void GLogger::LogWarning(Category category, spdlog::format_string_t<Args...> fmt, Args && ...args)
	{
		switch (category)
		{
		case GE_ENGINE_LOG:
		{
			EngineLogger->warn(fmt, std::forward<Args>(args)...);
			break;
		}

		case GE_CLIENT_LOG:
		{
			ClientLogger->warn(fmt, std::forward<Args>(args)...);
			break;
		}
		};
	}

	template<typename ...Args>
	inline void GLogger::LogError(Category category, spdlog::format_string_t<Args...> fmt, Args && ...args)
	{
		switch (category)
		{
		case GE_ENGINE_LOG:
		{
			EngineLogger->error(fmt, std::forward<Args>(args)...);
			break;
		}

		case GE_CLIENT_LOG:
		{
			ClientLogger->error(fmt, std::forward<Args>(args)...);
			break;
		}
		};
	}

	template<typename ...Args>
	inline void GLogger::LogCritical(Category category, spdlog::format_string_t<Args...> fmt, Args && ...args)
	{
		switch (category)
		{
		case GE_ENGINE_LOG:
		{
			EngineLogger->critical(fmt, std::forward<Args>(args)...);
			break;
		}

		case GE_CLIENT_LOG:
		{
			ClientLogger->critical(fmt, std::forward<Args>(args)...);
			break;
		}
		};
	}

#define GUARDIAN_ENGINE_MESSAGE(level, ...) GLogger::LogMessage(GLogger::GE_ENGINE_LOG, level, __VA_ARGS__)
#define GUARDIAN_ENGINE_TRACE(...) GLogger::LogTrace(GLogger::GE_ENGINE_LOG, __VA_ARGS__)
#define GUARDIAN_ENGINE_DEBUG(...) GLogger::LogDebug(GLogger::GE_ENGINE_LOG, __VA_ARGS__)
#define GUARDIAN_ENGINE_INFO(...) GLogger::LogInformation(GLogger::GE_ENGINE_LOG, __VA_ARGS__)
#define GUARDIAN_ENGINE_WARNING(...) GLogger::LogWarning(GLogger::GE_ENGINE_LOG, __VA_ARGS__)
#define GUARDIAN_ENGINE_ERROR(...) GLogger::LogError(GLogger::GE_ENGINE_LOG, __VA_ARGS__)
#define GUARDIAN_ENGINE_CRITICAL(...) GLogger::LogCritical(GLogger::GE_ENGINE_LOG, __VA_ARGS__)

#define GUARDIAN_CLIENT_MESSAGE(level, ...) GLogger::LogMessage(GLogCLIENTE_ENGINE_LOG, level, __VA_ARGS__)
#define GUARDIAN_CLIENT_TRACE(...) GLogger::LogTrace(GLogger::GE_CLIENT_LOG, __VA_ARGS__)
#define GUARDIAN_CLIENT_DEBUG(...) GLogger::LogDebug(GLogger::GE_CLIENT_LOG, __VA_ARGS__)
#define GUARDIAN_CLIENT_INFO(...) GLogger::LogInformation(GLogger::GE_CLIENT_LOG, __VA_ARGS__)
#define GUARDIAN_CLIENT_WARNING(...) GLogger::LogWarning(GLogger::GE_CLIENT_LOG, __VA_ARGS__)
#define GUARDIAN_CLIENT_ERROR(...) GLogger::LogError(GLogger::GE_CLIENT_LOG, __VA_ARGS__)
#define GUARDIAN_CLIENT_CRITICAL(...) GLogger::LogCritical(GLogger::GE_CLIENT_LOG, __VA_ARGS__)
}

#endif