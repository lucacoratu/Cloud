#include "cloudpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::coreLogger;

void Log::init()
{
	spdlog::set_pattern("%^[%d %b %Y] [%T] %n: %v%$");
	coreLogger = spdlog::stdout_color_mt("SERVER");
	coreLogger->set_level(spdlog::level::trace);
}
