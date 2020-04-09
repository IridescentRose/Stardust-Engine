#include <Platform/Platform.h>
#include <Network/NetworkDriver.h>
#include <Graphics/Dialogs.h>

namespace Stardust::Network {

#if CURRENT_PLATFORM == PLATFORM_PSP
	NetworkDriver::NetworkDriver()
	{
		m_Socket = Socket();
		thr = new Utilities::Thread(ReceiveThread);
	}
	bool NetworkDriver::Init() {

		Utilities::detail::core_Logger->log("Attempting Network Init");
		sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);
		sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEHTTP);
		sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEURI);
		sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
		sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
		sceUtilityLoadNetModule(PSP_NET_MODULE_SSL);

		//Result stores our codes from the initialization process
		int result = 0;
		result = sceNetInit(128 * 1024, 42, 0, 42, 0); //Creates the network manager with a buffer
		if (result < 0) { //These If Blocks close the game on an error

			Utilities::detail::core_Logger->log("Failed sceNetInit", Utilities::LOGGER_LEVEL_WARN);
			return false;
		}
		result = sceNetInetInit(); //Initializes Inet
		if (result < 0) {
			Utilities::detail::core_Logger->log("Failed sceInetInit", Utilities::LOGGER_LEVEL_WARN);
			return false;
		}
		result = sceNetApctlInit(0x10000, 48); //Initializes Access Point Control
		if (result < 0) {
			Utilities::detail::core_Logger->log("Failed sceNetApctlInit", Utilities::LOGGER_LEVEL_WARN);
			return false;
		}
		result = sceNetResolverInit(); //Initializes DNS resolver (unused)
		if (result < 0) {
			Utilities::detail::core_Logger->log("Failed sceNetResolverInit", Utilities::LOGGER_LEVEL_WARN);
			return false;
		}
		
		
		return Graphics::ShowNetworkDialog();
	}

	void NetworkDriver::Cleanup() {
		thr->Kill();

		//NET
		sceNetApctlTerm();
		sceNetResolverTerm();
		sceNetInetTerm();
		sceNetTerm();

		//Modules
		sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);
		sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEHTTP);
		sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEURI);
		sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
		sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);

		Utilities::detail::core_Logger->log("Cleaning up Networking Driver");
	}

#endif

	bool NetworkDriver::Connect(unsigned short port, const char* ip) {
		bool res = m_Socket.Connect(port, ip);

		if (res){
			m_Socket.SetBlock(false);
			thr->Start(0);
		}

		return res;
	}

	void NetworkDriver::AddPacket(PacketOut* p)
	{
		Utilities::detail::core_Logger->log("Clearing Packet Queue", Utilities::LOGGER_LEVEL_DEBUG);
		packetQueue.push(p);
	}
	void NetworkDriver::ClearPacketQueue()
	{
		Utilities::detail::core_Logger->log("Clearing Packet Queue", Utilities::LOGGER_LEVEL_DEBUG);
		for (int i = 0; i < packetQueue.size(); i++) {
			delete packetQueue.front();
			packetQueue.pop();
		}
	}

	void NetworkDriver::SendPackets()
	{

		Utilities::detail::core_Logger->log("Sending Network Packet Queue");
		std::vector<byte> endByteBuffer;

		for (int i = 0; i < packetQueue.size(); i++) {
			endByteBuffer.clear();

			int packetLength = packetQueue.front()->bytes.size() + 2;

			//Header
			encodeVarInt(packetLength, endByteBuffer);
			encodeShort(packetQueue.front()->ID, endByteBuffer);

			//Add body
			for (int x = 0; x < packetQueue.front()->bytes.size(); x++) {
				endByteBuffer.push_back(packetQueue.front()->bytes[x]);
			}

			Utilities::detail::core_Logger->log("Sending packet with ID: " + std::to_string(packetQueue.front()->ID), Utilities::LOGGER_LEVEL_DEBUG);
			//Send over socket
			m_Socket.Send(endByteBuffer.size(), endByteBuffer.data());

			delete packetQueue.front();
			packetQueue.pop();
		}

	}

	void NetworkDriver::ReceivePacket()
	{
		PacketIn* p = m_Socket.Recv();
		if (p != NULL) {
			unhandledPackets.push(p);
		}
	}

	void NetworkDriver::HandlePackets()
	{
		Utilities::detail::core_Logger->log("Handling Packets...", Utilities::LOGGER_LEVEL_TRACE);
		
		int len = unhandledPackets.size();
		for(int i = 0; i < len; i++){
			PacketIn* p = unhandledPackets.front();

			if (packetHandlers.find(p->ID) != packetHandlers.end()) {
				packetHandlers[p->ID](p);
			}
			delete p;
			unhandledPackets.pop();
		}
	}

	void NetworkDriver::AddPacketHandler(int id, PacketHandler h)
	{
		Utilities::detail::core_Logger->log("Added Packet Handler for ID: " + std::to_string(id));
		packetHandlers.emplace(id, h);
	}

	void NetworkDriver::ClearPacketHandlers()
	{
		Utilities::detail::core_Logger->log("Clearing Packet Handlers");
		packetHandlers.clear();
	}

#if CURRENT_PLATFORM == PLATFORM_PSP
	int NetworkDriver::ReceiveThread(SceSize args, void* argp)
	{
		while (1) {
			g_NetworkDriver.ReceivePacket();
			sceKernelDelayThread(1000 * 50);
		}
		
		return 0;
	}
#endif

	NetworkDriver g_NetworkDriver;
}