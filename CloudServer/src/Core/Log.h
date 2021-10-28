#ifndef LOG_H
#define LOG_H

#include "spdlog/spdlog.h"

//Wrapper class for the spdlog library
class Log {
private:
	static std::shared_ptr<spdlog::logger> coreLogger;
public:
	static void init();

	inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
};

//Macros for easier use of the spdlog library
#define SV_FATAL(...)	::Log::getCoreLogger()->fatal(__VA_ARGS__)
#define SV_ERROR(...)	::Log::getCoreLogger()->error(__VA_ARGS__)
#define SV_WARN(...)	::Log::getCoreLogger()->warn(__VA_ARGS__)
#define SV_TRACE(...)	::Log::getCoreLogger()->trace(__VA_ARGS__)
#define SV_INFO(...)	::Log::getCoreLogger()->info(__VA_ARGS__)

#endif // !LOG_H

