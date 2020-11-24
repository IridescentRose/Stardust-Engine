#pragma once
#include <Platform/Platform.h>
#include <string>
#include <fstream>
#include <sstream>

namespace Stardust::Utilities {
	
	/**
	 * Basic logger levels for logging, ranging from Trace -> Error.
	**/
	enum LoggerLevel {
		LOGGER_LEVEL_TRACE = -2,
		LOGGER_LEVEL_DEBUG = -1,
		LOGGER_LEVEL_INFO = 0,
		LOGGER_LEVEL_WARN = 1,
		LOGGER_LEVEL_ERROR = 2,
	};

	/**
	 * The basic logger class for Stardust.
	**/
	class Logger {
	public:
		/**
		 * Constructor - uses the name for message formatting and path for output.
		 * 
		 * @param name - Program / Logger name.
		 * @param path - Output file - default is "stardust_log.log"
		**/
		Logger(std::string name, std::string path = "stardust_log.log");
		
		/**
		 * Force flushes log and destroys the previously accumulated data.
		**/
		~Logger();

		/**
		 * Synchronizes current log to disk. 
		 * Important for low disk-speed systems to avoid calling this as much as possible.
		**/
		auto flushLog() -> void;

		/**
		 * Adds a message (formatted later) onto the log with severity.
		 * If the logger level passed in is less than the currentLevel of this logger, it is not added.
		 * 
		 * @param message - Message to log
		 * @param level - Logger level of the message.
		**/
		auto log(std::string message, LoggerLevel level = LOGGER_LEVEL_INFO) -> void;

		inline auto trace(std::string message) -> void {
			log(message, LOGGER_LEVEL_TRACE);
		}
		inline auto debug(std::string message) -> void {
			log(message, LOGGER_LEVEL_DEBUG);
		}
		inline auto info(std::string message) -> void {
			log(message, LOGGER_LEVEL_INFO);
		}
		inline auto warn(std::string message) -> void {
			log(message, LOGGER_LEVEL_WARN);
		}
		inline auto error(std::string message) -> void {
			log(message, LOGGER_LEVEL_ERROR);
		}

		/**
		 * A LoggerLevel to filter by. If it is lower, then the logged messages lower than this will not be included.
		**/
		int currentLevel;
		/**
		 * Autoflush?
		 */
		bool autoFlush;
		bool toConsole;
	private:
		std::ofstream m_file;
		std::stringstream m_filebuf;
		std::string m_name;
	};

	/**
	 * Core stardust logger! NOT FOR REGULAR USE.
	 * Only acceptable use case is engine debugging.
	 * In which case:
	 * <code>
	 * Stardust::Utilities::detail::core_Logger->currentLevel = LOGGER_LEVEL_TRACE;
	 * </code>
	**/
	namespace detail {
		extern Logger* core_Logger;
	}

	/**
	 * Stardust provides a default logger for your applications
	**/
	extern Logger* app_Logger;
}
