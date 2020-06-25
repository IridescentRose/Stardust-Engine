#pragma once
#include "NetworkTypes.h"

namespace Stardust::Network {

	class Socket {
	public:

		bool Connect(unsigned short port, const char* ip);
		void Close();

		bool SetBlock(bool blocking);

		void Send(size_t size, char* buffer);
		bool isAlive();

		PacketIn* Recv(bool extended);
	private:
		int m_socket;
	};

}