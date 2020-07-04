#include <Network/Socket.h>

#if CURRENT_PLATFORM == PLATFORM_PSP 
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspnet.h>
#include <psputility.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
#include <psphttp.h>
#include <pspsdk.h>
#include <pspwlan.h>

#elif CURRENT_PLATFORM == PLATFORM_WIN

#define WIN32_LEAN_AND_MEAN 1
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <Ws2tcpip.h>
#endif

#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_NIX)
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <queue>
#include <fcntl.h>
#endif

#if CURRENT_PLATFORM == PLATFORM_VITA
#include <netinet/in.h>
#include <vitasdk.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <queue>
#include <fcntl.h>
#endif

#if CURRENT_PLATFORM == PLATFORM_NIX
#include <thread>
#endif

namespace Stardust::Network {
	bool Socket::Connect(unsigned short port, const char* ip)
	{
		Utilities::detail::core_Logger->log("Opening Connection [" + std::string(ip) + "]" + "@" + std::to_string(port) + "!", Utilities::LOGGER_LEVEL_INFO);
		m_socket = socket(PF_INET, SOCK_STREAM, 0);
		struct sockaddr_in name;
		name.sin_family = AF_INET;
		name.sin_port = htons(port);

		#if CURRENT_PLATFORM == PLATFORM_VITA
		sceNetInetPton(AF_INET, ip, &name.sin_addr.s_addr);
		#else
		inet_pton(AF_INET, ip, &name.sin_addr.s_addr);
		#endif
		bool b = (connect(m_socket, (struct sockaddr*) & name, sizeof(name)) >= 0);

		if (!b) {
			Utilities::detail::core_Logger->log("Failed to open a connection! (Is the server open?)", Utilities::LOGGER_LEVEL_WARN);
		}

		return b;
	}
	void Socket::Close()
	{
		Utilities::detail::core_Logger->log("Closing socket!");
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_NIX) || (CURRENT_PLATFORM == PLATFORM_VITA)
		close(m_socket);
#elif CURRENT_PLATFORM == PLATFORM_WIN
		closesocket(m_socket);
#endif
	}

	bool Socket::SetBlock(bool blocking)
	{

#if CURRENT_PLATFORM == PLATFORM_PSP || (CURRENT_PLATFORM == PLATFORM_NIX) || (CURRENT_PLATFORM == PLATFORM_VITA)
		return fcntl(m_socket, F_SETFL, O_NONBLOCK) == 0;
#elif CURRENT_PLATFORM == PLATFORM_WIN
		//TODO: NON BLOCKING WINDOWS SOCKET
		return false;
#endif
	}

	void Socket::Send(size_t size, char* buffer)
	{
		int res = send(m_socket, buffer, size, 0);
		if (res < 0) {
			Utilities::detail::core_Logger->log("Failed to send a packet!", Utilities::LOGGER_LEVEL_WARN);
			Utilities::detail::core_Logger->log("Packet Size: " + std::to_string(size), Utilities::LOGGER_LEVEL_WARN);
		}
	}

	bool Socket::isAlive()
	{
		bool connected = false;
		char buffer[32] = { 0 };
		int res = recv(m_socket, buffer, sizeof(buffer), MSG_PEEK);

		if (res != 0) {
			connected = true;
		}

		return connected;
	}

	PacketIn* Socket::Recv(bool extended)
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
					size_t received = recv(m_socket, &data[dataLen] + totalReceived, 1 - totalReceived, 0);
					if (received <= 0) {
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
						sceKernelDelayThread(300);
#elif CURRENT_PLATFORM == PLATFORM_WIN || (CURRENT_PLATFORM == PLATFORM_NIX)
						std::this_thread::sleep_for(std::chrono::milliseconds(300));
#endif
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

			byte *b = new byte[length];
			for(int i = 0; i < length; i++){
				b[i] = 0;
			}
			
			while (totalTaken < length) {
				int res = recv(m_socket, b, length, 0);
				if(res > 0){
					totalTaken += res;
				}
				else {
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
					sceKernelDelayThread(300);
#elif CURRENT_PLATFORM == PLATFORM_WIN || (CURRENT_PLATFORM == PLATFORM_NIX)
					std::this_thread::sleep_for(std::chrono::milliseconds(300));
#endif
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

			return pIn;
		}
		else {
			return NULL;
		}
	}
}