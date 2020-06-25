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

	bool NetworkDriver::Connect(unsigned short port, const char* ip, bool threaded) {
		bool res = m_Socket.Connect(port, ip);

		if (threaded && res) {
			m_Socket.SetBlock(false);
			thr->Kill(); //Check
			thr->Start(0);
			sceKernelDelayThread(50 * 1000);
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

	void NetworkDriver::SendPackets(bool extendedID)
	{

		Utilities::detail::core_Logger->log("Sending Network Packet Queue");
		
		ByteBuffer* bbuf = new ByteBuffer(512 * 1024); //512 KB

		int len = packetQueue.size();
		for (int i = 0; i < len; i++) {
			int packetLength;

			if(extendedID){
				packetLength = packetQueue.front()->buffer.GetUsedSpace() + 2;
			}
			else {
				packetLength = packetQueue.front()->buffer.GetUsedSpace() + 1;
			}

			//Header
			bbuf->WriteVarInt32(packetLength);

			if(extendedID){
				bbuf->WriteBEUInt16(packetQueue.front()->ID);
			}
			else {
				bbuf->WriteBEUInt8(packetQueue.front()->ID);
			}
			//Add body

			packetQueue.front()->buffer.ReadToByteBuffer(*bbuf, packetQueue.front()->buffer.GetUsedSpace());

			Utilities::detail::core_Logger->log("Sending packet with ID: " + std::to_string(packetQueue.front()->ID), Utilities::LOGGER_LEVEL_DEBUG);
			//Send over socket
			m_Socket.Send(packetQueue.front()->buffer.GetUsedSpace(), packetQueue.front()->buffer.m_Buffer);

			delete packetQueue.front();
			packetQueue.pop();
		}

	}

	void NetworkDriver::ReceivePacket(bool extendedID)
	{
		PacketIn* p = m_Socket.Recv(extendedID);
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
			g_NetworkDriver.SendPackets();
			g_NetworkDriver.ReceivePacket();
			g_NetworkDriver.HandlePackets();

			sceKernelDelayThread(1000 * 50);
		}
		
		return 0;
	}
#endif

	NetworkDriver g_NetworkDriver;
}