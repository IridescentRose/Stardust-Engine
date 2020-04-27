#include <Network/Socket.h>

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
#include <fcntl.h>
#endif

namespace Stardust::Network {
	bool Socket::Connect(unsigned short port, const char* ip)
	{
		Utilities::detail::core_Logger->log("Opening Connection [" + std::string(ip) + "]" + "@" + std::to_string(port) + "!", Utilities::LOGGER_LEVEL_INFO);
		m_socket = socket(PF_INET, SOCK_STREAM, 0);
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
	void Socket::Close()
	{
		Utilities::detail::core_Logger->log("Closing socket!");
		close(m_socket);
	}

	bool Socket::SetBlock(bool blocking)
	{
		int flags = fcntl(m_socket, F_GETFL, 0);
		if (flags == -1) return false;
		flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
		return (fcntl(m_socket, F_SETFL, flags) == 0) ? true : false;

		return false;
	}

	void Socket::Send(size_t size, byte* buffer)
	{
		int res = send(m_socket, buffer, size, 0);
		if (res < 0) {
			Utilities::detail::core_Logger->log("Failed to send a packet!", Utilities::LOGGER_LEVEL_WARN);
			Utilities::detail::core_Logger->log("Packet Size: " + std::to_string(size), Utilities::LOGGER_LEVEL_WARN);
		}
	}

	PacketIn* Socket::Recv()
	{
		PacketIn* pIn = new PacketIn();

		std::vector<byte> len;
		byte newByte;
		int res = recv(m_socket, &newByte, 1, 0);
		
		if (res > 0) {

		while (newByte & 128) {
			if(res > 0){
				len.push_back(newByte);
				res = recv(m_socket, &newByte, 1, 0);
			}
			else {
				sceKernelDelayThread(300);
			}
		}
		len.push_back(newByte);

		//We now have the length stored in len
		int length = decodeVarInt(len);

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
					sceKernelDelayThread(300);
				}
			}
			

			for (int i = 0; i < length; i++) {
				pIn->bytes.push_back(b[i]);
			}

			pIn->pos = 0;

			pIn->ID = decodeShort(*pIn);

			Utilities::detail::core_Logger->log("Received Packet!", Utilities::LOGGER_LEVEL_DEBUG);
			Utilities::detail::core_Logger->log("Packet ID: " + std::to_string(pIn->ID), Utilities::LOGGER_LEVEL_DEBUG);

			return pIn;
		}
		else {
			return NULL;
		}
	}
}