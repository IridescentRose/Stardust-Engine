#pragma once
#include <vector>
#include "ByteBuffer.h"
#include <Utilities/Logger.h>

#include <NonCopy.h>
#include <NonMovable.h>

namespace Stardust::Network {
	/**
	 * Network bytes are uint8_t / char
	**/
	typedef char byte;

	/**
	 * Packet Output structure.
	 * Requires an ID.
	 * 
	 * Packet Output should always be a heap allocated pointer...
	 * This should be gotten by new PacketOut(size) where size is the estimated total size of the packet.
	 * It is always better to be more liberal with this sizing as the send process truncates this.
	 * 
	 * Your data should be written onto a ByteBuffer object (buffer).
	 * Upon being sent through the driver interface this packet is deleted.
	**/
	struct PacketOut : public NonCopyable{
		PacketOut(size_t size) {
			buffer = new ByteBuffer(size);
		}
		uint16_t ID;
		ByteBuffer* buffer;
	};

	/**
	 * You will not manually construct a PacketIn - as it is read by Recv.
	 * Packets received will always have an ID and its data stored in the ByteBuffer.
	**/
	struct PacketIn : public NonCopyable{
		PacketIn(size_t size) {
			buffer = new ByteBuffer(size);
		}
		uint16_t ID;
		ByteBuffer* buffer;
	};
}
