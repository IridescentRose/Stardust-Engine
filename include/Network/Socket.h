#pragma once
#include "NetworkSerialization.h"

namespace Stardust::Network {

	class Socket {
	public:

		bool Connect(unsigned short port, const char* ip);
		void Close();

		bool SetBlock(bool blocking);

		void Send(size_t size, byte* buffer);
		bool isAlive();

		PacketIn* Recv(bool extended);
	private:
		int m_socket;
	};

}