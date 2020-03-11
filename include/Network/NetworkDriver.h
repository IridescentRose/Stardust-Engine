#pragma once
#include <Network/NetworkSerialization.h>
#include <Platform/Platform.h>
#include <Network/Socket.h>
#include <queue>
#include <map>
#include <Utilities/Thread.h>

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

#if CURRENT_PLATFORM == PLATFORM_PSP
		static int ReceiveThread(SceSize args, void* argp);
#endif
		Utilities::Thread* thr;
	};

	extern NetworkDriver g_NetworkDriver;
}