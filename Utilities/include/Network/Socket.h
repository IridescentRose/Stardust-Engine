#pragma once
#include "NetworkTypes.h"

namespace Stardust::Network {

	class Socket {
	public:
		virtual ~Socket();

		virtual void Send(size_t size, char* buffer) = 0;
		virtual PacketIn* Recv(bool extended) = 0;

		virtual bool SetBlock(bool blocking) = 0;
		virtual void Close() = 0;
		virtual bool isAlive() = 0;

	protected:
		int m_socket;
	};

	class ClientSocket : public Socket {
	public:
		ClientSocket();
		virtual ~ClientSocket();

		bool Connect(unsigned short port, const char* ip);
		void Close();
		bool SetBlock(bool blocking);
		void Send(size_t size, char* buffer);
		bool isAlive();

		PacketIn* Recv(bool extended);

	};

	//TODO: MULTIPLE CLIENT CONNECTIONS!
	class ServerSocket : public Socket {
	public:
		ServerSocket(unsigned short port);
		virtual ~ServerSocket();
		void ListenState();

		void Close();

		bool SetBlock(bool blocking);
		void Send(size_t size, char* buffer);
		bool isAlive();

		PacketIn* Recv(bool extended);

	private:
		int m_Connection;
		unsigned short m_PortNo;
		bool connected;
	};

}