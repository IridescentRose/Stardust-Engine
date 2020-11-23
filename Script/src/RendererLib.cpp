#include <StardustScript.h>
#include <GFX/RenderCore.h>

namespace Stardust::Scripting {

	static int lua_render_clear(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Renderer.clear() takes no arguments!");

		GFX::g_RenderCore->clear();

		return 0;
	}


	static int lua_render_set_clear_color(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 4)
			return luaL_error(L, "Argument error: Renderer.setClearColor(r, g, b, a) takes 4 arguments!");


		auto r = static_cast<uint8_t>(luaL_checknumber(L, 1));
		auto g = static_cast<uint8_t>(luaL_checknumber(L, 2));
		auto b = static_cast<uint8_t>(luaL_checknumber(L, 3));
		auto a = static_cast<uint8_t>(luaL_checknumber(L, 4));

		GFX::g_RenderCore->setClearColor(r, g, b, a);

		return 0;
	}

	static int lua_render_set_default_2D(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Renderer.setDefault2D() takes no arguments!");

		GFX::g_RenderCore->setDefault2DMode();

		return 0;
	}

	static int lua_render_set_2D_mode(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 6)
			return luaL_error(L, "Argument error: Renderer.set2DMode(l, r, b, t, zn, zf) takes 6 arguments!");


		auto l = static_cast<float>(luaL_checknumber(L, 1));
		auto r = static_cast<float>(luaL_checknumber(L, 2));
		auto b = static_cast<float>(luaL_checknumber(L, 3));
		auto t = static_cast<float>(luaL_checknumber(L, 4));
		auto zn = static_cast<float>(luaL_checknumber(L, 5));
		auto zf = static_cast<float>(luaL_checknumber(L, 6));

		GFX::g_RenderCore->set2DMode(l, r, b, t, zn, zf);

		return 0;
	}

	static int lua_render_begin_frame(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Renderer.beginFrame() takes no arguments!");

		GFX::g_RenderCore->beginFrame();

		return 0;
	}

	static int lua_render_end_frame(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Renderer.endFrame() takes no arguments!");

		GFX::g_RenderCore->endFrame();

		return 0;
	}


	static int lua_render_set_3D_mode(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Renderer.set3DMode() takes no arguments!");

		GFX::g_RenderCore->set3DMode();

		return 0;
	}


	static int lua_render_bind_camera(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Renderer.bindCamera() takes a Camera!");

		//TODO: CAMERA

		return 0;
	}

	static const luaL_Reg renderLib[]{
		{"clear", lua_render_clear},
		{"setClearColor", lua_render_set_clear_color},
		{"setDefault2D", lua_render_set_default_2D},
		{"set2DMode", lua_render_set_2D_mode},
		{"beginFrame", lua_render_begin_frame},
		{"endFrame", lua_render_end_frame},
		{"bindCamera", lua_render_bind_camera},
		{"set3DMode", lua_render_set_3D_mode},
		{0, 0}
	};

	void initScriptRenderLib(lua_State* L) {
		lua_getglobal(L, "Renderer");
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
		}
		luaL_setfuncs(L, renderLib, 0);
		lua_setglobal(L, "Renderer");
	}
}