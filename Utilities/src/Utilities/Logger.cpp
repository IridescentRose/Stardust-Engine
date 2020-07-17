#include <Utilities/Logger.h>
#include <iostream>
namespace Stardust::Utilities {
	Logger::Logger(std::string name, std::string path) {
		m_file = std::ofstream(path, std::ios::trunc);
		m_filebuf = std::stringstream();
		currentLevel = 0;
		m_name = name;
	}
	
	Logger::~Logger() {
		flushLog();
	}

	void Logger::flushLog() {
		m_file << m_filebuf.str();
		m_file.flush();
	}

	void Logger::log(std::string message, LoggerLevel level) {
		if (level < currentLevel)
			return;

		m_filebuf << "[" << m_name << "]";

		switch (level) {
		case LOGGER_LEVEL_TRACE: {
			m_filebuf << "[TRACE]: ";
			break;
		}
		case LOGGER_LEVEL_DEBUG: {
			m_filebuf << "[DEBUG]: ";
			break;
		}
		case LOGGER_LEVEL_INFO: {
			m_filebuf << "[INFO]: ";
			break;
		}
		case LOGGER_LEVEL_WARN: {
			m_filebuf << "[WARN]: ";
			break;
		}
		case LOGGER_LEVEL_ERROR: {
			m_filebuf << "[ERROR]: ";
			break;
		}

		}

		m_filebuf << message << std::endl;
	}
	namespace detail {
		Logger* core_Logger;
	}

	Logger* app_Logger;
}