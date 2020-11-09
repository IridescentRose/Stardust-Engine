#include <StardustScript.h>
#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <pspdebug.h>
#define pspDebugScreenPrintf printf
#else
#include <stdio.h>
#endif

namespace Stardust::Scripting {

	int status;
	lua_State* L;
	void initScripting() {
		status = 0;
		L = luaL_newstate();
		luaL_openlibs(L);
	}

	void loadScript(std::string path) {
		status = luaL_loadfile(L, path.c_str());
	}

	void callScript() {
		status = lua_pcall(L, 0, LUA_MULTRET, 0);

		if (status != 0) {
			printf("error: %s\n", lua_tostring(L, -1));
			lua_pop(L, 1);  // remove error message
		}
	}

	void cleanupScripting() {
		lua_close(L);
	}
}