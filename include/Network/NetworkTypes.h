#pragma once
#include <vector>

namespace Stardust::Network {
	typedef unsigned char byte;

	typedef std::vector<byte> PacketOut;
	struct PacketIn {
		short int pos;
		std::vector<byte> bytes;
	};

	inline std::vector<byte> encodeVarInt(int value) {
		std::vector<byte> vec;
		
		while (value > 127) {
			vec.push_back(((byte)(value & 127)) | 128);

			value >> 7;
		}
		vec.push_back((byte)value & 127);
		return vec;
	}

	inline int decodeVarInt(std::vector<byte> input) {
		int ret = 0;

		for (int i = 0; i < input.size(); i++) {
			ret |= (input[i] & 127) << (7 * i);

			if (!input[i] & 128) {
				break;
			}
		}

		return ret;
	}

	inline int decodeVarInt(PacketIn& p) {
		int ret = 0;

		for (int i = p.pos; i < p.bytes.size(); i++) {
			p.pos++;
			ret |= (p.bytes[i] & 127) << (7 * i);

			if (!p.bytes[i] & 128) {
				break;
			}
		}

		return ret;
	}
}