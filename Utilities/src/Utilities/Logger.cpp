#include <Utilities/Logger.h>
#include <iostream>
#include <Utilities/Timer.h>
namespace Stardust::Utilities {
	Logger::Logger(std::string name, std::string path) {
		m_file = std::ofstream(path, std::ios::trunc);
		m_filebuf = std::stringstream();
		currentLevel = 0;
		m_name = name;
		autoFlush = false;
		toConsole = false;
	}
	
	Logger::~Logger() {
		if (!autoFlush) {
			flushLog();
		}
	}

	void Logger::flushLog() {
		m_file << m_filebuf.str();
		m_file.flush();
	}

	void Logger::log(std::string message, LoggerLevel level) {
		if (level < currentLevel)
			return;

		std::stringstream ss;
		ss << "[" << g_AppTimer.elapsed() << "]";
		ss << "[" << m_name << "]";

		switch (level) {
		case LOGGER_LEVEL_TRACE: {
			ss << "[TRACE]: ";
			break;
		}
		case LOGGER_LEVEL_DEBUG: {
			ss << "[DEBUG]: ";
			break;
		}
		case LOGGER_LEVEL_INFO: {
			ss << "[INFO]: ";
			break;
		}
		case LOGGER_LEVEL_WARN: {
			ss << "[WARN]: ";
			break;
		}
		case LOGGER_LEVEL_ERROR: {
			ss << "[ERROR]: ";
			break;
		}

		}

		ss << message;

		m_filebuf << ss.str() << std::endl;
		if (autoFlush) {
			m_file << ss.str() << std::endl;
		}

		if (toConsole) {
			std::cerr << ss.str() << std::endl;
		}
	}
	namespace detail {
		Logger* core_Logger;
	}

	Logger* app_Logger;
}