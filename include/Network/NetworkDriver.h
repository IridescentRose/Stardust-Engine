#pragma once
#include <Network/NetworkSerialization.h>

namespace Stardust::Network {

	class NetworkDriver {
	public:
		
		NetworkDriver();
		void Init();
		bool Connect();
		void Cleanup();

		void AddPacket(PacketOut* p);
		void ClearPacketQueue();
		void SendPackets();

		std::queue<PacketOut*> packetQueue;
	};
}