#include <Network/Socket.h>
#include <iostream>
#include <zlib/zlib.h>	

namespace Stardust::Network {
	ClientSocket::ClientSocket()
	{
		m_socket = 0;
		compression = false;
		threshold = 0;
	}
	bool ClientSocket::Connect(unsigned short port, const char* ip)
	{
		Utilities::detail::core_Logger->log("Opening Connection [" + std::string(ip) + "]" + "@" + std::to_string(port) + "!", Utilities::LOGGER_LEVEL_INFO);
		m_socket = static_cast<int>(socket(PF_INET, SOCK_STREAM, 0));
		struct sockaddr_in name;
		name.sin_family = AF_INET;
		name.sin_port = htons(port);

		inet_pton(AF_INET, ip, &name.sin_addr.s_addr);
		
		bool b = (connect(m_socket, (struct sockaddr*) & name, sizeof(name)) >= 0);

		if (!b) {
			Utilities::detail::core_Logger->log("Failed to open a connection! (Is the server open?)", Utilities::LOGGER_LEVEL_WARN);
		}

		return b;
	}
	
	void ClientSocket::Close()
	{
		Utilities::detail::core_Logger->log("Closing socket!");
		return Platform::detail::closeSockets(m_socket);
	}

	Socket::~Socket(){
	
	}
	ClientSocket::~ClientSocket(){
	
	}

	ServerSocket::~ServerSocket(){
	
	}

	ServerSocket::ServerSocket(unsigned short port)
	{
		m_socket = static_cast<int>(socket(AF_INET, SOCK_STREAM, 0));

		if (m_socket == -1) {
			throw std::runtime_error("Fatal: Could not open socket! Errno: " + std::to_string(errno));
		}
		m_PortNo = port;



		Utilities::detail::core_Logger->log("Socket Created!", Utilities::LOGGER_LEVEL_DEBUG);

		sockaddr_in sockaddr;
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = INADDR_ANY;
		sockaddr.sin_port = htons(port);

		if (bind(m_socket, (struct sockaddr*) & sockaddr, sizeof(sockaddr)) < 0) {
			throw std::runtime_error("Fatal: Could not bind socket address! Port: " + std::to_string(port) + ". Errno: " + std::to_string(errno));
		}

		Utilities::detail::core_Logger->log("Socket Bound!", Utilities::LOGGER_LEVEL_DEBUG);

		Utilities::detail::core_Logger->log("Listening on socket...", Utilities::LOGGER_LEVEL_DEBUG);
		if (listen(m_socket, 1) < 0) {
			throw std::runtime_error("Fatal: Could not listen on socket. Errno: " + std::to_string(errno));
		}

		auto addrlen = sizeof(sockaddr);
		m_Connection = static_cast<int>(accept(m_socket, (struct sockaddr*) & sockaddr, (socklen_t*)&addrlen));
		Utilities::detail::core_Logger->log("Found potential connection...", Utilities::LOGGER_LEVEL_DEBUG);

		if (m_Connection < 0) {
			throw std::runtime_error("Fatal: Could not accept connection. Errno: " + std::to_string(errno));
		}

#if CURRENT_PLATFORM == PLATFORM_PSP
		int yes = 1;
		if (setsockopt(m_Connection, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
			throw std::runtime_error("Fatal: Could not set no delay! Errno " + std::to_string(errno));
		}
#endif

		Utilities::detail::core_Logger->log("New Connection from " + std::string(inet_ntoa(sockaddr.sin_addr)) + " on port " + std::to_string(ntohs(sockaddr.sin_port)), Utilities::LOGGER_LEVEL_INFO);
		SetBlock(false);

		connected = true;
	}

	void ServerSocket::ListenState()
	{
		Platform::detail::closeSockets(m_Connection);
		
		sockaddr_in sockaddr;
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = INADDR_ANY;
		sockaddr.sin_port = htons(m_PortNo);

		Utilities::detail::core_Logger->log("Listening on socket...", Utilities::LOGGER_LEVEL_DEBUG);
		if (listen(m_socket, 1) < 0) {
			throw std::runtime_error("Fatal: Could not listen on socket. Errno: " + std::to_string(errno));
		}

		auto addrlen = sizeof(sockaddr);
		m_Connection = static_cast<int>(accept(m_socket, (struct sockaddr*) & sockaddr, (socklen_t*)&addrlen));
		Utilities::detail::core_Logger->log("Found potential connection...", Utilities::LOGGER_LEVEL_DEBUG);

		if (m_Connection < 0) {
			throw std::runtime_error("Fatal: Could not accept connection. Errno: " + std::to_string(errno));
		}

		Utilities::detail::core_Logger->log("New Connection from " + std::string((inet_ntoa(sockaddr.sin_addr))) + " on port " + std::to_string(ntohs(sockaddr.sin_port)), Utilities::LOGGER_LEVEL_INFO);
		SetBlock(false);
		connected = true;
	}

	void ServerSocket::Close()
	{
		Utilities::detail::core_Logger->log("Closing connection!");
		connected = false;
		return Platform::detail::closeSockets(m_Connection);
	}

	bool ClientSocket::SetBlock(bool blocking)
	{
		return Platform::detail::setBlocking(m_socket, blocking);
	}

	bool ServerSocket::SetBlock(bool blocking)
	{
		return Platform::detail::setBlocking(m_Connection, blocking);
	}

	void ClientSocket::Send(size_t size, char* buffer)
	{
		int res = send(m_socket, buffer, static_cast<int>(size), 0);
		if (res < 0) {
			Utilities::detail::core_Logger->log("Failed to send a packet!", Utilities::LOGGER_LEVEL_WARN);
			Utilities::detail::core_Logger->log("Packet Size: " + std::to_string(size), Utilities::LOGGER_LEVEL_WARN);
		}
	}

	void ServerSocket::Send(size_t size, char* buffer)
	{
		int res = send(m_Connection, buffer, static_cast<int>(size), 0);
		if (res < 0) {
			Utilities::detail::core_Logger->log("Failed to send a packet!", Utilities::LOGGER_LEVEL_WARN);
			Utilities::detail::core_Logger->log("Packet Size: " + std::to_string(size), Utilities::LOGGER_LEVEL_WARN);
		}
	}

	bool ClientSocket::isAlive()
	{
		bool connected = false;
		char buffer[32] = { 0 };
		int res = recv(m_socket, buffer, sizeof(buffer), MSG_PEEK);

		if (res != 0) {
			connected = true;
		}

		return connected;
	}
	bool ServerSocket::isAlive()
	{
		bool aconnected = false;
		char buffer[32] = { 0 };
		int res = recv(m_Connection, buffer, sizeof(buffer), MSG_PEEK);

		if (res != 0) {
			aconnected = true;
		}
		else {
			Close();
		}

		return aconnected && connected;
	}

	PacketIn* ClientSocket::Recv(bool extended)
	{

		std::vector<byte> len;
		byte newByte;
		int res = recv(m_socket, &newByte, 1, MSG_PEEK);

		if (res > 0) {
			char data[5] = { 0 };
			size_t dataLen = 0;
			do {
				size_t totalReceived = 0;
				while (1 > totalReceived) {
					size_t received = recv(m_socket, &data[dataLen] + totalReceived, static_cast < int>(1 - totalReceived), 0);
					if (received <= 0) {
						Platform::delayForMS(1);
					}
					else {
						totalReceived += received;
					}
				}
			} while ((data[dataLen++] & 0x80) != 0);

			int readed = 0;
			int result = 0;
			char read;
			do {
				read = data[readed];
				int value = (read & 0b01111111);
				result |= (value << (7 * readed));
				readed++;
			} while ((read & 0b10000000) != 0);


			int length = result;

			PacketIn* pIn = new PacketIn(length);
			Utilities::detail::core_Logger->log("LENGTH: " + std::to_string(length), Utilities::LOGGER_LEVEL_DEBUG);

			int totalTaken = 0;

			byte* b = new byte[length];
			for (int i = 0; i < length; i++) {
				b[i] = 0;
			}

			while (totalTaken < length) {
				int res = recv(m_socket, b, length, 0);
				if (res > 0) {
					totalTaken += res;
				}
				else {
					Platform::delayForMS(1);
				}
			}


			for (int i = 0; i < length; i++) {
				pIn->buffer->WriteBEUInt8(b[i]);
			}

			if (compression) {
				uint32_t compressedSize = 0;
				pIn->buffer->ReadVarInt32(compressedSize);


				char byteBuffer[200 KiB];


				if(compressedSize > threshold){
					ByteBuffer* bbuf2 = new ByteBuffer(pIn->buffer->GetUsedSpace());

					for (size_t i = 0; i < compressedSize; i++){
						uint8_t byte = 0;
						pIn->buffer->ReadBEUInt8(byte);
						bbuf2->WriteBEUInt8(byte);
					}

					delete pIn->buffer;

					z_stream infstream;
					infstream.zalloc = Z_NULL;
					infstream.zfree = Z_NULL;
					infstream.opaque = Z_NULL;
					infstream.avail_in = static_cast<uInt>(bbuf2->GetUsedSpace()); // size of input
					infstream.next_in = (Bytef*)bbuf2->m_Buffer; // input char array
					infstream.avail_out = 200 KiB; // size of output
					infstream.next_out = (Bytef*)byteBuffer; // output char array

					int stat = inflateInit(&infstream);

					stat = inflate(&infstream, Z_NO_FLUSH);
					
					if (stat < 0){
						std::cout << stat << std::endl;
						throw std::runtime_error("FICK");
					}

					inflateEnd(&infstream);
					
					delete bbuf2;

					pIn->buffer = new ByteBuffer(compressedSize);

					for(size_t i = 0; i < compressedSize; i++){
						pIn->buffer->WriteBEUInt8(byteBuffer[i]);
					}

				}
			}

			if (pIn != NULL && pIn->buffer->GetUsedSpace() > 0) {
				if (extended) {
					pIn->buffer->ReadBEUInt16(pIn->ID);
				}
				else {
					uint8_t t = 0;
					pIn->buffer->ReadBEUInt8(t);
					pIn->ID = t;
				}
			}
			else {
				pIn->ID = -1;
			}

			

			Utilities::detail::core_Logger->log("Received Packet!", Utilities::LOGGER_LEVEL_DEBUG);
			Utilities::detail::core_Logger->log("Packet ID: " + std::to_string(pIn->ID), Utilities::LOGGER_LEVEL_DEBUG);

			return pIn;
		}
		else {
			return NULL;
		}
	}

	PacketIn* ServerSocket::Recv(bool extended)
	{

		std::vector<byte> len;
		byte newByte;
		int res = recv(m_Connection, &newByte, 1, MSG_PEEK);

		if (res > 0) {
			char data[5] = { 0 };
			size_t dataLen = 0;
			do {
				size_t totalReceived = 0;
				while (1 > totalReceived) {
					size_t received = recv(m_Connection, &data[dataLen] + totalReceived, static_cast<int>(1 - totalReceived), 0);
					if (received <= 0) {
						Platform::delayForMS(1);
					}
					else {
						totalReceived += received;
					}
				}
			} while ((data[dataLen++] & 0x80) != 0);

			int readed = 0;
			int result = 0;
			char read;
			do {
				read = data[readed];
				int value = (read & 0b01111111);
				result |= (value << (7 * readed));
				readed++;
			} while ((read & 0b10000000) != 0);


			int length = result;

			PacketIn* pIn = new PacketIn(length);
			Utilities::detail::core_Logger->log("LENGTH: " + std::to_string(length), Utilities::LOGGER_LEVEL_DEBUG);

			int totalTaken = 0;

			byte* b = new byte[length];
			for (int i = 0; i < length; i++) {
				b[i] = 0;
			}

			while (totalTaken < length) {
				int res = recv(m_Connection, b, length, 0);
				if (res > 0) {
					totalTaken += res;
				}
				else {
					Platform::delayForMS(1);
				}
			}


			for (int i = 0; i < length; i++) {
				pIn->buffer->WriteBEUInt8(b[i]);
			}

			if (pIn != NULL && pIn->buffer->GetUsedSpace() > 0) {
				if (extended) {
					pIn->buffer->ReadBEUInt16(pIn->ID);
				}
				else {
					uint8_t t = 0;
					pIn->buffer->ReadBEUInt8(t);
					pIn->ID = t;
				}
			}
			else {
				pIn->ID = -1;
			}

			Utilities::detail::core_Logger->log("Received Packet!", Utilities::LOGGER_LEVEL_DEBUG);
			Utilities::detail::core_Logger->log("Packet ID: " + std::to_string(pIn->ID), Utilities::LOGGER_LEVEL_DEBUG);

			delete[] b;
			return pIn;
		}
		else {
			return NULL;
		}
	}
}