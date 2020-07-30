#pragma once
#include <Platform/Platform.h>
#include <Network/Socket.h>
#include <queue>
#include <map>
#include <Utilities/Thread.h>
#include "NetworkTypes.h"

namespace Stardust::Network {
	/**
	 * Handles a specific packet with an ID.
	**/
	typedef int (*PacketHandler)(PacketIn*);

	/**
	 * Basic network interface for Stardust applications.
	**/
	class NetworkDriver {
	public:
		
		/**
		 * Default constructor
		**/
		NetworkDriver();

		/**
		 * Starts system-specific network API
		**/
		bool Init();

		/**
		 * Closes system-specific network API
		**/
		void Cleanup();

		/**
		 * Sets our target socket to a Client or Server side socket. THIS IS REQUIRED!
		 * 
		 * @param socket - A ClientSocket or ServerSocket
		**/
		void setSocket(Socket* socket);
		
		/**
		 * Adds a packet to be sent out
		 * This takes ownership of the PacketOut object.
		 * The memory freeing is handled and PacketOut should not be modified after this!
		 * 
		 * @param p - Packet input.
		**/
		void AddPacket(PacketOut* p);

		/**
		 * Clears the entire packet queue.
		**/
		void ClearPacketQueue();

		/**
		 * Sends packets over the network.
		 * 
		 * @param extendedID - Whether to use a short or char as packetID (default to short)/
		**/
		void SendPackets(bool extendedID = true);

		/**
		 * Recieves packets over the network.
		 * 
		 * @param extendedID - Whether to use a short or char as packetID (default to short)/
		**/
		void ReceivePacket(bool extendedID = true);

		/**
		 * Handles all received packets.
		**/
		void HandlePackets();

		/**
		 * Adds a packet handler for a give packet ID.
		 * 
		 * @param id - Packet ID for the handler
		 * @param h - Packet Handler for the ID.
		**/
		void AddPacketHandler(int id, PacketHandler h);

		/**
		 * Clears all packets!
		**/
		void ClearPacketHandlers();

		/**
		 * Receives a file over HTTP (PSP Only for now)
		 * 
		 * @param url - URL to get it
		 * @param filepath - File to output to.
		**/
		bool GetFileHTTP(const char* url, const char* filepath);

		std::queue<PacketOut*> packetQueue;
		std::queue<PacketIn*> unhandledPackets;
		std::map<int, PacketHandler> packetHandlers;

	private:
		Socket* m_Socket;
	};

	extern NetworkDriver g_NetworkDriver;
}