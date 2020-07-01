#include <Platform/Platform.h>
#include <Network/NetworkDriver.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <Graphics/Dialogs.h>
#endif

namespace Stardust::Network {

#if CURRENT_PLATFORM == PLATFORM_PSP
	NetworkDriver::NetworkDriver()
	{
		m_Socket = Socket();
		thr = new Utilities::Thread(ReceiveThread);
	}
	bool NetworkDriver::Init() {

		Utilities::detail::core_Logger->log("Attempting Network Init");
		sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
		sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
		sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEURI);
		sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEHTTP);
		sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);

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
		sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
		sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
		sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEURI);
		sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEHTTP);
		sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);

		Utilities::detail::core_Logger->log("Cleaning up Networking Driver");
	}

#elif CURRENT_PLATFORM == PLATFORM_WIN
	NetworkDriver::NetworkDriver() {
		m_Socket = Socket();
		thr = NULL;
	}
	bool NetworkDriver::Init() {
		WSAData data;
		int res = WSAStartup(MAKEWORD(2, 2), &data);
		if (res != 0) {
			throw std::runtime_error("WSAStartup Failed: " + res);
			return false;
		}

		return true;
	}

	void NetworkDriver::Cleanup() {

	}
#endif

	bool NetworkDriver::Connect(unsigned short port, const char* ip, bool threaded) {
		bool res = m_Socket.Connect(port, ip);

		if (threaded && res) {
			m_Socket.SetBlock(false);
			thr->Kill(); //Check
			thr->Start(0);

#if CURRENT_PLATFORM == PLATFORM_PSP
			sceKernelDelayThread(50 * 1000);
#else
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
#endif
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
		

		int len = packetQueue.size();
		for (int i = 0; i < len; i++) {
			int packetLength;

			if(extendedID){
				packetLength = packetQueue.front()->buffer->GetUsedSpace() + 2;
			}
			else {
				packetLength = packetQueue.front()->buffer->GetUsedSpace() + 1;
			}

			ByteBuffer* bbuf = new ByteBuffer(packetLength + 5); //512 KB

			//Header
			bbuf->WriteVarInt32(packetLength);

			if(extendedID){
				bbuf->WriteBEUInt16(packetQueue.front()->ID);
			}
			else {
				bbuf->WriteBEUInt8(packetQueue.front()->ID);
			}
			//Add body
			for (int i = 0; i < packetQueue.front()->buffer->GetUsedSpace(); i++) {
				uint8_t temp;
				packetQueue.front()->buffer->ReadBEUInt8(temp);
				bbuf->WriteBEUInt8(temp);
			}

			Utilities::detail::core_Logger->log("Sending packet with ID: " + std::to_string(packetQueue.front()->ID), Utilities::LOGGER_LEVEL_DEBUG);
			//Send over socket
			m_Socket.Send(bbuf->GetUsedSpace(), bbuf->m_Buffer);

			delete bbuf;
			delete packetQueue.front()->buffer;
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
			delete p->buffer;
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

	bool NetworkDriver::GetFileHTTP(const char* url, const char* filepath)
	{

#if CURRENT_PLATFORM == PLATFORM_PSP
		int templ, connection, request, ret, status, dataend, fd, byteswritten;
		SceULong64 contentsize;
		unsigned char readbuffer[8192];

		ret = sceHttpInit(20000);

		if (ret < 0) {
			Utilities::detail::core_Logger->log("HTTP INIT FAIL");
			return false;
		}

		templ = sceHttpCreateTemplate((char*)"xxx-agent/0.0.1 libhttp/1.0.0", 1, 1);

		if (templ < 0) {
			Utilities::detail::core_Logger->log("HTTP TEMPLATE FAIL");
			return false;
		}

		ret = sceHttpSetResolveTimeOut(templ, 3000000);

		if (ret < 0) {
			Utilities::detail::core_Logger->log("HTTP RESOLVE FAIL");
			return false;
		}

		ret = sceHttpSetRecvTimeOut(templ, 60000000);

		if (ret < 0) {
			Utilities::detail::core_Logger->log("HTTP RECV FAIL");
			return false;
		}

		ret = sceHttpSetSendTimeOut(templ, 60000000);

		if (ret < 0) {
			Utilities::detail::core_Logger->log("HTTP SEND FAIL");
			return false;
		}

		connection = sceHttpCreateConnectionWithURL(templ, url, 0);

		if (ret < 0) {
			Utilities::detail::core_Logger->log("HTTP CONNECTION FAIL");
			return false;
		}

		request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_GET, (char*)url, 0);

		if (ret < 0) {
			Utilities::detail::core_Logger->log("HTTP REQUEST URL FAIL");
			return false;
		}

		ret = sceHttpSendRequest(request, 0, 0);

		if (ret < 0) {
			Utilities::detail::core_Logger->log("HTTP SEND REQUEST FAIL");
			return false;
		}


		ret = sceHttpGetStatusCode(request, &status);

		if (ret < 0) {
			Utilities::detail::core_Logger->log("HTTP STATUS FAIL");
			return false;
		}

		if (status != 200)
			return false;

		ret = sceHttpGetContentLength(request, &contentsize);

		if (ret < 0) {
			Utilities::detail::core_Logger->log("HTTP LENGTH FAIL");
			return false;
		}

		dataend = 0;
		byteswritten = 0;

		fd = sceIoOpen(filepath, PSP_O_WRONLY | PSP_O_CREAT, 0777);

		while (dataend == 0)
		{
			ret = sceHttpReadData(request, readbuffer, 8192);

			if (ret < 0)
			{
				sceIoWrite(fd, filepath, 4);
				sceIoClose(fd);
				return false;


				if (ret < 0) {
					Utilities::detail::core_Logger->log("HTTP WRITE ERR");
					return false;
				}
			}

			if (ret == 0)
				dataend = 1;

			if (ret > 0)
			{
				byteswritten += ret;
				sceIoWrite(fd, readbuffer, ret);
			}
		}

		sceIoClose(fd);
		sceHttpDeleteRequest(request);
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(templ);
		sceHttpEnd();

		return true;
#else
		//TODO: GET HTTP LIBRARY
		return false;
#endif
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