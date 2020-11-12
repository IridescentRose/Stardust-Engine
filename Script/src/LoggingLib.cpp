#include <StardustScript.h>
#include <Utilities/Logger.h>

namespace Stardust::Scripting {

	static int lua_logging_log(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 2)
			return luaL_error(L, "Argument error: Log.log() takes two arguments (message, level)");

		auto msg = luaL_checkstring(L, 1);
		auto lvl = luaL_checkinteger(L, 2);
		if (lvl > 2 || lvl < -2) {
			lvl = 0;
		}

		Utilities::app_Logger->log(msg, (Utilities::LoggerLevel)lvl);
		return 0;
	}

	static int lua_logging_trace(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Log.trace() takes one argument (message)");

		auto msg = luaL_checkstring(L, 1);
		Utilities::app_Logger->log(msg, Utilities::LOGGER_LEVEL_TRACE);
		return 0;
	}

	static int lua_logging_debug(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Log.debug() takes one argument (message)");

		auto msg = luaL_checkstring(L, 1);
		Utilities::app_Logger->log(msg, Utilities::LOGGER_LEVEL_DEBUG);
		return 0;
	}

	static int lua_logging_info(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Log.info() takes one argument (message)");

		auto msg = luaL_checkstring(L, 1);
		Utilities::app_Logger->log(msg, Utilities::LOGGER_LEVEL_INFO);
		return 0;
	}


	static int lua_logging_warn(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Log.warn() takes one argument (message)");

		auto msg = luaL_checkstring(L, 1);
		Utilities::app_Logger->log(msg, Utilities::LOGGER_LEVEL_WARN);
		return 0;
	}


	static int lua_logging_error(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Log.error() takes one argument (message)");

		auto msg = luaL_checkstring(L, 1);
		Utilities::app_Logger->log(msg, Utilities::LOGGER_LEVEL_ERROR);
		return 0;
	}

	static const luaL_Reg loggingLib[]{
		{"log", lua_logging_log},
		{"trace", lua_logging_trace},
		{"debug", lua_logging_debug},
		{"info", lua_logging_info},
		{"warn", lua_logging_warn},
		{"error", lua_logging_error},
		{0, 0}
	};


	void initScriptLoggingLib(lua_State* L) {
		lua_getglobal(L, "Log");
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
		}
		luaL_setfuncs(L, loggingLib, 0);
		lua_setglobal(L, "Log");
	}
}