#pragma once
#include <Network/NetworkSerialization.h>
#include <Network/Socket.h>

namespace Stardust::Network {

	class NetworkDriver {
	public:
		
		NetworkDriver();
		bool Init();
		bool Connect(unsigned short port, const char* ip);
		void Cleanup();

		void AddPacket(PacketOut* p);
		void ClearPacketQueue();
		void SendPackets();

		std::queue<PacketOut*> packetQueue;

	private:
		Socket m_Socket;
	};
}