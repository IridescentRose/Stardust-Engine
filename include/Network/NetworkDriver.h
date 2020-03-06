#pragma once
#include <Network/NetworkSerialization.h>
#include <Network/Socket.h>
#include <queue>
#include <map>

namespace Stardust::Network {
	typedef int (*PacketHandler)(PacketIn*);

	class NetworkDriver {
	public:
		
		NetworkDriver();
		bool Init();
		bool Connect(unsigned short port, const char* ip);
		void Cleanup();

		void AddPacket(PacketOut* p);
		void ClearPacketQueue();
		void SendPackets();

		void ReceivePacket();
		void HandlePackets();

		void AddPacketHandler(int id, PacketHandler h);
		void ClearPacketHandlers();

		std::queue<PacketOut*> packetQueue;
		std::queue<PacketIn*> unhandledPackets;
		std::map<int, PacketHandler> packetHandlers;

	private:
		Socket m_Socket;
	};
}