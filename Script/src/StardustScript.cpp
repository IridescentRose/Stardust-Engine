#include <StardustScript.h>
#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <pspdebug.h>
#define pspDebugScreenPrintf printf
#else
#include <stdio.h>
#endif
#include <iostream>

namespace Stardust::Scripting {

	static int lua_plat_init(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Platform.init() takes no argument.");

		Platform::initPlatform();
		return 0;
	}

	static int lua_plat_exit(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Platform.exit() takes no argument.");

		Platform::exitPlatform();
		return 0;
	}

#if CURRENT_PLATFORM == PLATFORM_PSP
	static int lua_print(lua_State* L)
	{
		pspDebugScreenInit();
		pspDebugScreenSetXY(0, 0);

		int argc = lua_gettop(L);
		int n;
		for (n = 1; n <= argc; n++) pspDebugScreenPrintf("%s\n", lua_tostring(L, n));
		return 0;
	}
#endif

	static int lua_plat_update(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Platform.update() takes no argument.");

		Platform::platformUpdate();
		return 0;
	}

	static int lua_plat_delay(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Platform.delay() takes 1 argument (millis).");
		
		int x = luaL_checkinteger(L, 1);
		Platform::delayForMS(x);

		return 0;
	}

	static const luaL_Reg platformLib[] {
		{"init", lua_plat_init},
		{"update", lua_plat_update},
		{"exit", lua_plat_exit},
		{"delay", lua_plat_delay},
		{0, 0}
	};

	void initPlatformLib(lua_State* L) {
		lua_getglobal(L, "Platform");
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
		}
		luaL_setfuncs(L, platformLib, 0);
		lua_setglobal(L, "Platform");
	}

	int status;
	lua_State* L;
	void initScripting() {
		status = 0;
		L = luaL_newstate();
		luaL_openlibs(L);
#if CURRENT_PLATFORM == PLATFORM_PSP
		lua_register(L, "print", lua_print);
#endif
		initPlatformLib(L);
		initScriptInputLib(L);
	}

	void loadScript(std::string path) {
		status = luaL_loadfile(L, path.c_str());
	}

	void callScript() {
		status = lua_pcall(L, 0, LUA_MULTRET, 0);

		if (status != 0) {
			printf("error: %s\n", lua_tostring(L, -1));
#if CURRENT_PLATFORM == PLATFORM_PSP
			pspDebugScreenInit();
			pspDebugScreenSetXY(0, 0);
			pspDebugScreenPrintf("error: %s\n", lua_tostring(L, -1));
#endif
			lua_pop(L, 1);  // remove error message
		}
	}

	void cleanupScripting() {
		lua_close(L);
	}
}