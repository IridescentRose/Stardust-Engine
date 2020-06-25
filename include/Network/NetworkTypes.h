#pragma once
#include <vector>
#include "ByteBuffer.h"
#include <Utilities/Logger.h>

namespace Stardust::Network {
	typedef unsigned char byte;

	struct PacketOut {
		PacketOut(size_t size) {
			buffer = new ByteBuffer(size);
		}
		uint16_t ID;
		ByteBuffer* buffer;
	};
	struct PacketIn {
		PacketIn(size_t size) {
			buffer = new ByteBuffer(size);
		}
		uint16_t ID;
		ByteBuffer* buffer;
	};
}