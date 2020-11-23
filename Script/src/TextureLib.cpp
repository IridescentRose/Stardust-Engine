#include <StardustScript.h>
#include <GFX/GFXWrapper.h>

namespace Stardust::Scripting {

	static int lua_texture_bind(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: TextureManager.bindTex(tex) takes a texture ID!");


		auto num = luaL_checkinteger(L, 1);
		GFX::g_TextureManager->bindTex(num);

		return 0;
	}

	static int lua_texture_delete(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: TextureManager.deleteTex(tex) takes a texture ID!");


		auto num = luaL_checkinteger(L, 1);
		GFX::g_TextureManager->deleteTex(num);

		return 0;
	}

	static int lua_texture_load(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 4)
			return luaL_error(L, "Argument error: TextureManager.loadTex(path, filterMag, filterMin, repeat) takes 4 arguments!");


		auto path = luaL_checkstring(L, 1);
		auto filterMag = luaL_checkinteger(L, 2);
		auto filterMin = luaL_checkinteger(L, 3);

		auto repeat = false;
		if (lua_isboolean(L, 4)) {
			repeat = lua_toboolean(L, 4);
		}

		auto id = GFX::g_TextureManager->loadTex(path, filterMag, filterMin, repeat);

		lua_pushinteger(L, id);
		return 1;
	}

	static const luaL_Reg textureLib[]{
		{"loadTex", lua_texture_load},
		{"bindTex", lua_texture_bind},
		{"deleteTex", lua_texture_delete},
		{0, 0}
	};

	void initScriptTextureLib(lua_State* L) {
		lua_getglobal(L, "TextureManager");
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
		}
		luaL_setfuncs(L, textureLib, 0);
		lua_setglobal(L, "TextureManager");
	}
}