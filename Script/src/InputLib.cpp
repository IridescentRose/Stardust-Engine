#include <StardustScript.h>
#include <Utilities/Input.h>

namespace Stardust::Scripting {

	static int lua_input_isPressed(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Input.isPressed() takes 1 argument (key).");

		int x = static_cast<int>(luaL_checkinteger(L, 1));
		auto r = Utilities::KeyPressed(x);

		lua_pushboolean(L, r);
		return 1;
	}

	static int lua_input_isHeld(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Input.isHeld() takes 1 argument (key).");

		int x = static_cast<int>(luaL_checkinteger(L, 1));
		auto r = Utilities::KeyHold(x);

		lua_pushboolean(L, r);
		return 1;
	}

	static int lua_input_strength(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Input.strength() takes 1 argument (key).");

		int x = static_cast<int>(luaL_checkinteger(L, 1));
		auto r = Utilities::KeyStrength(x);

		lua_pushnumber(L, r);
		return 1;
	}

	static int lua_input_update(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Input.update() takes no arguments");

		Utilities::updateInputs();
		return 0;
	}

#define GET_CTRL(NAME, BIT) \
	static int NAME(lua_State *L) \
{ \
	int argc = lua_gettop(L); \
	if(argc == 0) { \
	lua_pushinteger(L, BIT); \
	return(1); \
	} \
	return 0; \
}
	GET_CTRL(lua_input_select, PSP_CTRL_SELECT)
	GET_CTRL(lua_input_start, PSP_CTRL_START)
	GET_CTRL(lua_input_up, PSP_CTRL_UP)
	GET_CTRL(lua_input_right, PSP_CTRL_RIGHT)
	GET_CTRL(lua_input_down, PSP_CTRL_DOWN)
	GET_CTRL(lua_input_left, PSP_CTRL_LEFT)
	GET_CTRL(lua_input_l, PSP_CTRL_LTRIGGER)
	GET_CTRL(lua_input_r, PSP_CTRL_RTRIGGER)
	GET_CTRL(lua_input_triangle, PSP_CTRL_TRIANGLE)
	GET_CTRL(lua_input_circle, PSP_CTRL_CIRCLE)
	GET_CTRL(lua_input_cross, PSP_CTRL_CROSS)
	GET_CTRL(lua_input_square, PSP_CTRL_SQUARE)
	GET_CTRL(lua_input_home, PSP_CTRL_HOME)
	GET_CTRL(lua_input_hold, PSP_CTRL_HOLD)
	GET_CTRL(lua_input_note, PSP_CTRL_NOTE)

	static const luaL_Reg inputLib[]{
		{"isPressed", lua_input_isPressed},
		{"isHeld", lua_input_isHeld},
		{"strength", lua_input_strength},
		{"update", lua_input_update},
		{"select",    lua_input_select},
		{"start",     lua_input_start},
		{"up",        lua_input_up},
		{"right",     lua_input_right},
		{"down",      lua_input_down},
		{"left",      lua_input_left},
		{"l",         lua_input_l},
		{"r",         lua_input_r},
		{"triangle",  lua_input_triangle},
		{"circle",    lua_input_circle},
		{"cross",     lua_input_cross},
		{"square",    lua_input_square},
		{"home",      lua_input_home},
		{"hold",      lua_input_hold},
		{"note",      lua_input_note},
		{0, 0}
	};

	void initScriptInputLib(lua_State* L) {
		lua_getglobal(L, "Input");
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
		}
		luaL_setfuncs(L, inputLib, 0);
		lua_setglobal(L, "Input");
	}
}