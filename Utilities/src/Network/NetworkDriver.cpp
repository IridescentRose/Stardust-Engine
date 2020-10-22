#include <Platform/Platform.h>
#include <Network/NetworkDriver.h>


namespace Stardust::Network {

	NetworkDriver::NetworkDriver()
	{
		m_Socket = nullptr;
	}

	bool NetworkDriver::Init() {
		netEnabled = true;
		return Platform::detail::initNetworks();
	}

	void NetworkDriver::Cleanup() {
		netEnabled = false;
		Platform::detail::closeNetworks();
	}

	void NetworkDriver::setSocket(Socket* socket)
	{
		m_Socket = socket;
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


		size_t len = packetQueue.size();
		for (size_t i = 0; i < len; i++) {
			size_t packetLength;

			if(extendedID){
				packetLength = packetQueue.front()->buffer->GetUsedSpace() + 2;
			}
			else {
				packetLength = packetQueue.front()->buffer->GetUsedSpace() + 1;
			}

			ByteBuffer* bbuf = new ByteBuffer(packetLength + 5); //512 KB

			//Header
			bbuf->WriteVarInt32(static_cast<uint32_t>(packetLength));

			if(extendedID){
				bbuf->WriteBEUInt16(packetQueue.front()->ID);
			}
			else {
				bbuf->WriteBEUInt8(static_cast<uint8_t>(packetQueue.front()->ID));
			}
			//Add body
			for (int i = 0; i < packetQueue.front()->buffer->GetUsedSpace(); i++) {
				uint8_t temp;
				packetQueue.front()->buffer->ReadBEUInt8(temp);
				bbuf->WriteBEUInt8(temp);
			}

			Utilities::detail::core_Logger->log("Sending packet with ID: " + std::to_string(packetQueue.front()->ID), Utilities::LOGGER_LEVEL_DEBUG);
			//Send over socket
			m_Socket->Send(bbuf->GetUsedSpace(), bbuf->m_Buffer);

			delete bbuf;
			delete packetQueue.front()->buffer;
			delete packetQueue.front();
			packetQueue.pop();
		}

	}

	void NetworkDriver::ReceivePacket(bool extendedID)
	{
		PacketIn* p = m_Socket->Recv(extendedID);
		if (p != NULL) {
			unhandledPackets.push(p);
		}
	}

	void NetworkDriver::HandlePackets()
	{
		Utilities::detail::core_Logger->log("Handling Packets...", Utilities::LOGGER_LEVEL_TRACE);

		size_t len = unhandledPackets.size();
		for(size_t i = 0; i < len; i++){
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
		return Platform::detail::getHTTPFile(url, filepath);
	}

	NetworkDriver g_NetworkDriver;
	bool netEnabled = false;
}
