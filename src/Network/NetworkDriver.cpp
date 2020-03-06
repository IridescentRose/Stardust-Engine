#include <Platform/Platform.h>
#include <Network/NetworkDriver.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <pspnet.h>
#include <psputility.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
#include <psphttp.h>
#include <pspsdk.h>
#include <pspwlan.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <queue>
#endif

namespace Stardust::Network {

#if CURRENT_PLATFORM == PLATFORM_PSP
	NetworkDriver::NetworkDriver()
	{
		m_Socket = Socket();
	}
	bool NetworkDriver::Init() {

		Utilities::g_Logger->log("Attempting Network Init");
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

			Utilities::g_Logger->log("Failed sceNetInit", Utilities::LOGGER_LEVEL_WARN);
			return false;
		}
		result = sceNetInetInit(); //Initializes Inet
		if (result < 0) {
			Utilities::g_Logger->log("Failed sceInetInit", Utilities::LOGGER_LEVEL_WARN);
			return false;
		}
		result = sceNetApctlInit(0x10000, 48); //Initializes Access Point Control
		if (result < 0) {
			Utilities::g_Logger->log("Failed sceNetApctlInit", Utilities::LOGGER_LEVEL_WARN);
			return false;
		}
		result = sceNetResolverInit(); //Initializes DNS resolver (unused)
		if (result < 0) {
			Utilities::g_Logger->log("Failed sceNetResolverInit", Utilities::LOGGER_LEVEL_WARN);
			return false;
		}

		result = sceNetApctlConnect(1);	//Connects to your first (primary) internet connection.

		//Displays connection status
		int err;
		while (1) {
			int state;
			err = sceNetApctlGetState(&state);
			if (err != 0)
			{
				Utilities::g_Logger->log("Failed to autoconnect!");
				return false;
			}
			if (state == 4)
				return true;  // connected!

			sceKernelDelayThread(1000 * 50); //50 MS Delay;
		}
	}

	void NetworkDriver::Cleanup() {
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

		Utilities::g_Logger->log("Cleaning up Networking Driver");
	}

#endif

	bool NetworkDriver::Connect(unsigned short port, const char* ip) {
		return m_Socket.Connect(port, ip);
	}

	void NetworkDriver::AddPacket(PacketOut* p)
	{
		Utilities::g_Logger->log("Clearing Packet Queue");
		packetQueue.push(p);
	}
	void NetworkDriver::ClearPacketQueue()
	{
		Utilities::g_Logger->log("Clearing Packet Queue");
		for (int i = 0; i < packetQueue.size(); i++) {
			delete packetQueue.front();
			packetQueue.pop();
		}
	}

	void NetworkDriver::SendPackets()
	{

		Utilities::g_Logger->log("Sending Network Packet Queue");
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

			Utilities::g_Logger->log("Sending packet with ID: " + std::to_string(packetQueue.front()->ID), Utilities::LOGGER_LEVEL_DEBUG);
			//Send over socket
			m_Socket.Send(endByteBuffer.size(), endByteBuffer.data());

			delete packetQueue.front();
			packetQueue.pop();
		}

	}

	void NetworkDriver::ReceivePacket()
	{
		PacketIn p = m_Socket.Recv();
		unhandledPackets.push(&p);
	}

	void NetworkDriver::HandlePackets()
	{
		Utilities::g_Logger->log("Handling Packets...");

		while(!unhandledPackets.empty()){

			if (packetHandlers.find(unhandledPackets.front()->ID) != packetHandlers.end()) {
				packetHandlers[unhandledPackets.front()->ID](unhandledPackets.front());
			}

		}
	}

	void NetworkDriver::AddPacketHandler(int id, PacketHandler h)
	{
		Utilities::g_Logger->log("Added Packet Handler for ID: " + std::to_string(id));
		packetHandlers.emplace(id, h);
	}

	void NetworkDriver::ClearPacketHandlers()
	{
		Utilities::g_Logger->log("Clearing Packet Handlers");
		packetHandlers.clear();
	}

}