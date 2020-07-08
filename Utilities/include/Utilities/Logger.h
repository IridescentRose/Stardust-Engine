#pragma once
#include <Platform/Platform.h>
#include <string>
#include <fstream>
#include <sstream>

namespace Stardust::Utilities {
	
	enum LoggerLevel {
		LOGGER_LEVEL_TRACE = -2,
		LOGGER_LEVEL_DEBUG = -1,
		LOGGER_LEVEL_INFO = 0,
		LOGGER_LEVEL_WARN = 1,
		LOGGER_LEVEL_ERROR = 2,
	};

	class Logger {
	public:
		Logger(std::string name, std::string path = "stardust_log.log");
		~Logger();

		void flushLog();

		void log(std::string message, LoggerLevel level = LOGGER_LEVEL_INFO);

		int currentLevel;
	private:
		std::ofstream m_file;
		std::stringstream m_filebuf;
		std::string m_name;
	};

	namespace detail {
		extern Logger* core_Logger;
	}
	extern Logger* app_Logger;
}