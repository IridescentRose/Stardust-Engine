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

	byte* Socket::Recv()
	{
	}
}