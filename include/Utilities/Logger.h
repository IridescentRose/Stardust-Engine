#pragma once
#include <string>
#include <fstream>


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
		Logger(std::string path = "stardust_log.log");
		~Logger();

		void flushLog();

		void log(std::string message, LoggerLevel level = LOGGER_LEVEL_INFO);
	private:
		std::ofstream m_file;
	};

	extern Logger* g_Logger;
}