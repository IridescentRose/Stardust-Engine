#pragma once
#include <string>
#include <lua/lua.hpp>

namespace Stardust::Scripting {
	void initScripting();
	void loadScript(std::string path);
	void callScript();
	void cleanupScripting();

	void initScriptInputLib(lua_State* L);
	void initScriptLoggingLib(lua_State* L);
	void initScriptRenderLib(lua_State* L);
}