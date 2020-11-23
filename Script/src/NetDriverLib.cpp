#include <StardustScript.h>
#include <Network/NetworkDriver.h>

namespace Stardust::Scripting {

	static int lua_net_init(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Net.init() takes 0 arguments!");


		Network::g_NetworkDriver.Init();

		return 0;
	}


	static int lua_net_cleanup(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Net.cleanup() takes 0 arguments!");


		Network::g_NetworkDriver.Cleanup();

		return 0;
	}

	static int lua_net_set_socket(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Net.setSocket(sock) takes 1 arguments!");

		//TODO: SOCKET
		//Network::g_NetworkDriver.setSocket();

		return 0;
	}

	static int lua_net_add_packet(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 1)
			return luaL_error(L, "Argument error: Net.addPacket(pack) takes 1 arguments!");

		//TODO: PACKET
		//Network::g_NetworkDriver.addPacket();

		return 0;
	}

	static int lua_net_send_packets(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Net.sendPackets() takes 0 arguments!");


		Network::g_NetworkDriver.SendPackets();

		return 0;
	}

	static int lua_net_receive_packets(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Net.receivePacket() takes 0 arguments!");

		//TODO: RETURN OPTIONAL PACKET
		//Network::g_NetworkDriver.ReceivePacket();

		return 0;
	}

	static int lua_net_handle_packets(lua_State* L) {
		int argc = lua_gettop(L);
		if (argc != 0)
			return luaL_error(L, "Argument error: Net.handlePackets() takes 0 arguments!");

		//TODO: RETURN OPTIONAL PACKET
		//Network::g_NetworkDriver.ReceivePacket();

		return 0;
	}


	static const luaL_Reg netLib[]{
		{"init", lua_net_init},
		{"cleanup", lua_net_cleanup},
		{"setSocket", lua_net_set_socket},
		{"addPacket", lua_net_add_packet},
		{"sendPackets", lua_net_send_packets},
		{"receivePacket", lua_net_receive_packets},
		{"handlePackets", lua_net_handle_packets},
		{0, 0}
	};

	void initScriptNetDriverLib(lua_State* L) {
		lua_getglobal(L, "Net");
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
		}
		luaL_setfuncs(L, netLib, 0);
		lua_setglobal(L, "Net");
	}
}