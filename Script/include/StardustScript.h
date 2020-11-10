#pragma once
#include <string>
#include <lua/lua.hpp>

namespace Stardust::Scripting {
	void initScripting();
	void loadScript(std::string path);
	void callScript();
	void cleanupScripting();

	void initScriptInputLib(lua_State* L);
}