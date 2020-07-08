#pragma once
#include <Platform/Platform.h>
#include <Network/Socket.h>
#include <queue>
#include <map>
#include <Utilities/Thread.h>
#include "NetworkTypes.h"

namespace Stardust::Network {
	typedef int (*PacketHandler)(PacketIn*);

	class NetworkDriver {
	public:
		
		NetworkDriver();
		bool Init();
		void Cleanup();

		void setSocket(Socket* socket);
		
		void AddPacket(PacketOut* p);
		void ClearPacketQueue();
		void SendPackets(bool extendedID = true);

		void ReceivePacket(bool extendedID = true);
		void HandlePackets();

		void AddPacketHandler(int id, PacketHandler h);
		void ClearPacketHandlers();

		bool GetFileHTTP(const char* url, const char* filepath);

		std::queue<PacketOut*> packetQueue;
		std::queue<PacketIn*> unhandledPackets;
		std::map<int, PacketHandler> packetHandlers;

	private:
		Socket* m_Socket;
	};

	extern NetworkDriver g_NetworkDriver;
}