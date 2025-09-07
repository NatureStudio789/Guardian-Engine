#include "GLogger.h"

namespace GE
{
	std::shared_ptr<spdlog::logger> GLogger::EngineLogger;
	std::shared_ptr<spdlog::logger> GLogger::ClientLogger;


	void GLogger::InitializeLogger()
	{
		spdlog::set_pattern("%^[%T] - [%n] (%l) : %v%$");

		EngineLogger = spdlog::stdout_color_mt("Engine");
		EngineLogger->set_level(spdlog::level::trace);

		ClientLogger = spdlog::stdout_color_mt("Client");
		ClientLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> GLogger::GetEngineLogger()
	{
		return EngineLogger;
	}

	std::shared_ptr<spdlog::logger> GLogger::GetClientLogger()
	{
		return ClientLogger;
	}
}
