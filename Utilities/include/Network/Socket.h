#pragma once
#include "NetworkTypes.h"

namespace Stardust::Network {

	/**
	 * Abstract Socket interface
	**/
	class Socket {
	public:
		virtual ~Socket();

		/**
		 * Send raw data buffers
		 * 
		 * @param size - Size of buffer
		 * @param buffer - Mutable data buffer
		**/
		virtual void Send(size_t size, char* buffer) = 0;

		/**
		 * Receives a PacketIn
		 * 
		 * @param extended - Whether or not the packet ID is a short or a byte
		**/
		virtual PacketIn* Recv(bool extended) = 0;

		/**
		 * Sets the socket to be blocking / non-blocking
		 * 
		 * @param blocking - Whether or not to block.
		**/
		virtual bool SetBlock(bool blocking) = 0;

		/**
		 * Closes the socket.
		**/
		virtual void Close() = 0;
		
		/**
		 * Returns whether or not the connection is alive?
		**/
		virtual bool isAlive() = 0;

	protected:
		int m_socket;
	};

	/**
	 * Socket for Client side connections
	**/
	class ClientSocket : public Socket {
	public:
		ClientSocket();
		virtual ~ClientSocket();

		/**
		 * Connects to an IP at a port number.
		**/
		bool Connect(unsigned short port, const char* ip);

		void Close();
		bool SetBlock(bool blocking);
		void Send(size_t size, char* buffer);
		bool isAlive();

		PacketIn* Recv(bool extended);

		/**
		 * Is zlib compression enabled?
		**/
		bool compression;

		/**
		 * Threshold for zlib compression.
		**/
		int threshold;
	};

	/**
	 * Socket for server-hosting connections
	**/
	//TODO: MULTIPLE CLIENT CONNECTIONS!
	class ServerSocket : public Socket {
	public:

		/**
		 * Creates socket and binds it and listens to it.
		 * 
		 * @param port - Port to listen to.
		**/
		ServerSocket(unsigned short port);
		virtual ~ServerSocket();

		/**
		 * Listen for a connection
		**/
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