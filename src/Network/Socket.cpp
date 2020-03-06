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
#endif

namespace Stardust::Network {
	bool Socket::Connect(unsigned short port, const char* ip)
	{
		m_socket = socket(PF_INET, SOCK_STREAM, 0);
		struct sockaddr_in name;
		name.sin_family = AF_INET;
		name.sin_port = htons(port);

		inet_pton(AF_INET, ip, &name.sin_addr.s_addr);
		return (connect(m_socket, (struct sockaddr*) & name, sizeof(name)) >= 0);
	}
	void Socket::Close()
	{
		close(m_socket);
	}

	void Socket::Send(size_t size, byte* buffer)
	{
		send(m_socket, buffer, size, 0);
	}

	PacketIn Socket::Recv()
	{
		PacketIn pIn;

		std::vector<byte> len;
		byte newByte;
		recv(m_socket, &newByte, 1, 0);
		
		while (newByte & 128) {
			len.push_back(newByte);
			recv(m_socket, &newByte, 1, 0);
		}
		len.push_back(newByte);

		//We now have the length stored in len
		int length = decodeVarInt(len);

		int totalTaken = 0;

		byte *b = new byte[length];
		totalTaken += recv(m_socket, b, length, 0);

		for (int i = 0; i < length; i++) {
			pIn.bytes.push_back(b[i]);
		}

		pIn.pos = 0;

		pIn.ID = decodeShort(pIn);

		return pIn;
	}
}