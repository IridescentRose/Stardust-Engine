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
		int m_socket = 0;
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
		size_t threshold;
	};

	/**
	 * Creates a Connection.
	 * Used with a raw connection socket.
	 * Provides us packet methods.
	 */
	class Connection {
	public:
		/**
		 * Constructs a new Connection.
		 *
		 * \param sock - accepted TCP connection
		 */
		Connection(int sock);

		/**
		 * Destruction.
		 *
		 */
		~Connection();

		/**
		 * Turns on or off non-blocking mode.
		 *
		 * \param blocking - True if blocking, false if non-blocking. By default is blocking.
		 * \return - Whether or not operation was successful
		 */
		bool SetBlock(bool blocking);

		/**
		 * Checks if connection is valid.
		 *
		 * \return - Connection validity.
		 */
		bool isAlive();

		/**
		 * Send a buffer of some sort.
		 *
		 * \param size - size of data
		 * \param buffer - data buffer
		 */
		void Send(size_t size, char* buffer);

		/**
		 * Packet Receive.
		 *
		 * \param extended - If we use the u16 ID or u8 ID.
		 * \return - Packet
		 */
		PacketIn* Recv(bool extended);

		/**
		 * Closes our connection.
		 * Is also done at end of scope if not already done.
		 */
		void Close();

	private:
		int m_socket;
		bool connected;
	};

	/**
	 * Creates a server socket for binding.
	 */
	class ServerSocket {
	public:

		/**
		 * Creates socket and binds it and listens to it.
		 *
		 * @param port - Port to listen to.
		**/
		ServerSocket(unsigned short port);
		~ServerSocket();

		/**
		 * Listen for a connection
		**/
		Connection* ListenState();

		/**
		 * Closes our socket.
		 */
		void Close();

	private:
		int m_socket;
		unsigned short m_PortNo;
	};

}
