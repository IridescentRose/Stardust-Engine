#include <Network/ByteBuffer.h>
#include <Platform/Platform.h>


#define NEEDBYTES(Num) if (!CanReadBytes(Num))  return false  // Check if at least Num bytes can be read from  the buffer, return false if not
#define PUTBYTES(Num)  if (!CanWriteBytes(Num)) return false  // Check if at least Num bytes can be written to the buffer, return false if not

namespace Stardust::Network{
ByteBuffer::ByteBuffer(size_t a_BufferSize) :
	m_Buffer(new char[a_BufferSize + 1]),
	m_BufferSize(a_BufferSize + 1),
	m_DataStart(0),
	m_WritePos(0),
	m_ReadPos(0)
{

}

#include <cassert>
#define ASSERT assert
#define VERIFY assert



ByteBuffer::~ByteBuffer()
{
	CheckValid();
	delete[] m_Buffer;
	m_Buffer = nullptr;
}


#undef  ntohll
#define ntohll(x) (((static_cast<uint64_t>(ntohl(static_cast<uint32_t>(x)))) << 32) + ntohl(x >> 32))


// Changes endianness
inline uint64_t HostToNetwork8(const void* a_Value)
{
	uint64_t buf;
	memcpy(&buf, a_Value, sizeof(buf));
	buf = (((static_cast<uint64_t>(htonl(static_cast<uint32_t>(buf)))) << 32) + htonl(buf >> 32));
	return buf;
}





inline uint32_t HostToNetwork4(const void* a_Value)
{
	uint32_t buf;
	memcpy(&buf, a_Value, sizeof(buf));
	buf = ntohl(buf);
	return buf;
}





inline double NetworkToHostDouble8(const void* a_Value)
{
	uint64_t buf = 0;
	memcpy(&buf, a_Value, 8);
	buf = ntohll(buf);
	double x;
	memcpy(&x, &buf, sizeof(double));
	return x;
}





inline int64_t NetworkToHostLong8(const void* a_Value)
{
	int64_t buf;
	memcpy(&buf, a_Value, 8);
	buf = ntohll(buf);
	return *reinterpret_cast<int64_t*>(&buf);
}





inline uint64_t NetworkToHostULong8(const void* a_Value)
{
	uint64_t buf;
	memcpy(&buf, a_Value, 8);
	buf = ntohll(buf);
	return buf;
}





inline float NetworkToHostFloat4(const void* a_Value)
{
	uint32_t buf;
	float x;
	memcpy(&buf, a_Value, 4);
	buf = ntohl(buf);
	memcpy(&x, &buf, sizeof(float));
	return x;
}



bool ByteBuffer::Write(const void* a_Bytes, size_t a_Count)
{
	
	CheckValid();

	// Store the current free space for a check after writing:
	size_t CurFreeSpace = GetFreeSpace();
	size_t WrittenBytes = 0;

	if (CurFreeSpace < a_Count)
	{
		return false;
	}
	ASSERT(m_BufferSize >= m_WritePos);
	size_t TillEnd = m_BufferSize - m_WritePos;
	const char* Bytes = static_cast<const char*>(a_Bytes);
	if (TillEnd <= a_Count)
	{
		// Need to wrap around the ringbuffer end
		if (TillEnd > 0)
		{
			memcpy(m_Buffer + m_WritePos, Bytes, TillEnd);
			Bytes += TillEnd;
			a_Count -= TillEnd;
			WrittenBytes = TillEnd;
		}
		m_WritePos = 0;
	}

	// We're guaranteed that we'll fit in a single write op
	if (a_Count > 0)
	{
		memcpy(m_Buffer + m_WritePos, Bytes, a_Count);
		m_WritePos += a_Count;
		WrittenBytes += a_Count;
	}
	return true;
}



#undef GetFreeSpace
size_t ByteBuffer::GetFreeSpace(void) const {
	CheckValid();
	if (m_WritePos >= m_DataStart)
	{
		// Wrap around the buffer end:
		ASSERT(m_BufferSize >= m_WritePos);
		ASSERT((m_BufferSize - m_WritePos + m_DataStart) >= 1);
		return m_BufferSize - m_WritePos + m_DataStart - 1;
	}
	// Single free space partition:
	ASSERT(m_BufferSize >= m_WritePos);
	ASSERT(m_BufferSize - m_WritePos >= 1);
	return m_DataStart - m_WritePos - 1;
}





size_t ByteBuffer::GetUsedSpace(void) const
{
	
		CheckValid();
	ASSERT(m_BufferSize >= GetFreeSpace());
	ASSERT((m_BufferSize - GetFreeSpace()) >= 1);
	return m_BufferSize - GetFreeSpace() - 1;
}





size_t ByteBuffer::GetReadableSpace(void) const
{
	
		CheckValid();
	if (m_ReadPos > m_WritePos)
	{
		// Wrap around the buffer end:
		ASSERT(m_BufferSize >= m_ReadPos);
		return m_BufferSize - m_ReadPos + m_WritePos;
	}
	// Single readable space partition:
	ASSERT(m_WritePos >= m_ReadPos);
	return m_WritePos - m_ReadPos;
}





bool ByteBuffer::CanReadBytes(size_t a_Count) const
{
	
		CheckValid();
	return (a_Count <= GetReadableSpace());
}





bool ByteBuffer::CanWriteBytes(size_t a_Count) const
{
	
		CheckValid();
	return (a_Count <= GetFreeSpace());
}





bool ByteBuffer::ReadBEInt8(int8_t& a_Value)
{
	
		CheckValid();
	NEEDBYTES(1);
	ReadBuf(&a_Value, 1);
	return true;
}





bool ByteBuffer::ReadBEUInt8(uint8_t& a_Value)
{
	
		CheckValid();
	NEEDBYTES(1);
	ReadBuf(&a_Value, 1);
	return true;
}





bool ByteBuffer::ReadBEInt16(int16_t& a_Value)
{
	
		CheckValid();
	NEEDBYTES(2);
	uint16_t val;
	ReadBuf(&val, 2);
	val = ntohs(val);
	memcpy(&a_Value, &val, 2);
	return true;
}





bool ByteBuffer::ReadBEUInt16(uint16_t& a_Value)
{
	
		CheckValid();
	NEEDBYTES(2);
	ReadBuf(&a_Value, 2);
	a_Value = ntohs(a_Value);
	return true;
}





bool ByteBuffer::ReadBEInt32(int32_t& a_Value)
{
	
		CheckValid();
	NEEDBYTES(4);
	uint32_t val;
	ReadBuf(&val, 4);
	val = ntohl(val);
	memcpy(&a_Value, &val, 4);
	return true;
}





bool ByteBuffer::ReadBEUInt32(uint32_t& a_Value)
{
	
	CheckValid();
	NEEDBYTES(4);
	ReadBuf(&a_Value, 4);
	a_Value = ntohl(a_Value);
	return true;
}





bool ByteBuffer::ReadBEInt64(int64_t& a_Value)
{
	
	CheckValid();
	NEEDBYTES(8);
	ReadBuf(&a_Value, 8);
	a_Value = NetworkToHostLong8(&a_Value);
	return true;
}





bool ByteBuffer::ReadBEUInt64(uint64_t& a_Value)
{
	
	CheckValid();
	NEEDBYTES(8);
	ReadBuf(&a_Value, 8);
	a_Value = NetworkToHostULong8(&a_Value);
	return true;
}





bool ByteBuffer::ReadBEFloat(float& a_Value)
{
	
		CheckValid();
	NEEDBYTES(4);
	ReadBuf(&a_Value, 4);
	a_Value = NetworkToHostFloat4(&a_Value);
	return true;
}





bool ByteBuffer::ReadBEDouble(double& a_Value)
{
	
	CheckValid();
	NEEDBYTES(8);
	ReadBuf(&a_Value, 8);
	a_Value = NetworkToHostDouble8(&a_Value);
	return true;
}





bool ByteBuffer::ReadBool(bool& a_Value)
{
	
	CheckValid();
	NEEDBYTES(1);
	uint8_t Value = 0;
	ReadBuf(&Value, 1);
	a_Value = (Value != 0);
	return true;
}





bool ByteBuffer::ReadVarInt32(uint32_t& a_Value)
{
	
	CheckValid();
	uint32_t Value = 0;
	int Shift = 0;
	unsigned char b = 0;
	do
	{
		NEEDBYTES(1);
		ReadBuf(&b, 1);
		Value = Value | ((static_cast<uint32_t>(b & 0x7f)) << Shift);
		Shift += 7;
	} while ((b & 0x80) != 0);
	a_Value = Value;
	return true;
}





bool ByteBuffer::ReadVarInt64(uint64_t& a_Value)
{
	
		CheckValid();
	uint64_t Value = 0;
	int Shift = 0;
	unsigned char b = 0;
	do
	{
		NEEDBYTES(1);
		ReadBuf(&b, 1);
		Value = Value | ((static_cast<uint64_t>(b & 0x7f)) << Shift);
		Shift += 7;
	} while ((b & 0x80) != 0);
	a_Value = Value;
	return true;
}




bool ByteBuffer::ReadLEInt32(int32_t& a_Value)
{
	
		CheckValid();
	NEEDBYTES(4);
	ReadBuf(&a_Value, 4);

#ifdef IS_BIG_ENDIAN
	// Convert:
	a_Value = ((a_Value >> 24) & 0xff) | ((a_Value >> 16) & 0xff00) | ((a_Value >> 8) & 0xff0000) | (a_Value & 0xff000000);
#endif

	return true;
}








bool ByteBuffer::WriteBEInt8(int8_t a_Value)
{
	
		CheckValid();
	PUTBYTES(1);
	return WriteBuf(&a_Value, 1);
}





bool ByteBuffer::WriteBEUInt8(uint8_t a_Value)
{
	
		CheckValid();
	PUTBYTES(1);
	return WriteBuf(&a_Value, 1);
}





bool ByteBuffer::WriteBEInt16(int16_t a_Value)
{
	
	CheckValid();
	PUTBYTES(2);
	uint16_t val;
	memcpy(&val, &a_Value, 2);
	val = htons(val);
	return WriteBuf(&val, 2);
}





bool ByteBuffer::WriteBEUInt16(uint16_t a_Value)
{
	
	CheckValid();
	PUTBYTES(2);
	a_Value = htons(a_Value);
	return WriteBuf(&a_Value, 2);
}





bool ByteBuffer::WriteBEInt32(int32_t a_Value)
{
	
	CheckValid();
	PUTBYTES(4);
	uint32_t Converted = HostToNetwork4(&a_Value);
	return WriteBuf(&Converted, 4);
}





bool ByteBuffer::WriteBEUInt32(uint32_t a_Value)
{
	
	CheckValid();
	PUTBYTES(4);
	uint32_t Converted = HostToNetwork4(&a_Value);
	return WriteBuf(&Converted, 4);
}





bool ByteBuffer::WriteBEInt64(int64_t a_Value)
{
	
		CheckValid();
	PUTBYTES(8);
	uint64_t Converted = HostToNetwork8(&a_Value);
	return WriteBuf(&Converted, 8);
}





bool ByteBuffer::WriteBEUInt64(uint64_t a_Value)
{
	
	CheckValid();
	PUTBYTES(8);
	uint64_t Converted = HostToNetwork8(&a_Value);
	return WriteBuf(&Converted, 8);
}





bool ByteBuffer::WriteBEFloat(float a_Value)
{
	
		CheckValid();
	PUTBYTES(4);
	uint32_t Converted = HostToNetwork4(&a_Value);
	return WriteBuf(&Converted, 4);
}





bool ByteBuffer::WriteBEDouble(double a_Value)
{
	
		CheckValid();
	PUTBYTES(8);
	uint64_t Converted = HostToNetwork8(&a_Value);
	return WriteBuf(&Converted, 8);
}





bool ByteBuffer::WriteBool(bool a_Value)
{
	
		CheckValid();
	uint8_t val = a_Value ? 1 : 0;
	return Write(&val, 1);
}





bool ByteBuffer::WriteVarInt32(uint32_t a_Value)
{
	
		CheckValid();

	// A 32-bit integer can be encoded by at most 5 bytes:
	unsigned char b[5];
	size_t idx = 0;
	do
	{
		b[idx] = (a_Value & 0x7f) | ((a_Value > 0x7f) ? 0x80 : 0x00);
		a_Value = a_Value >> 7;
		idx++;
	} while (a_Value > 0);

	return WriteBuf(b, idx);
}





bool ByteBuffer::WriteVarInt64(uint64_t a_Value)
{
	
		CheckValid();

	// A 64-bit integer can be encoded by at most 10 bytes:
	unsigned char b[10];
	size_t idx = 0;
	do
	{
		b[idx] = (a_Value & 0x7f) | ((a_Value > 0x7f) ? 0x80 : 0x00);
		a_Value = a_Value >> 7;
		idx++;
	} while (a_Value > 0);

	return WriteBuf(b, idx);
}



bool ByteBuffer::ReadBuf(void* a_Buffer, size_t a_Count)
{
	
		CheckValid();
	NEEDBYTES(a_Count);
	char* Dst = static_cast<char*>(a_Buffer);  // So that we can do byte math
	ASSERT(m_BufferSize >= m_ReadPos);
	size_t BytesToEndOfBuffer = m_BufferSize - m_ReadPos;
	if (BytesToEndOfBuffer <= a_Count)
	{
		// Reading across the ringbuffer end, read the first part and adjust parameters:
		if (BytesToEndOfBuffer > 0)
		{
			memcpy(Dst, m_Buffer + m_ReadPos, BytesToEndOfBuffer);
			Dst += BytesToEndOfBuffer;
			a_Count -= BytesToEndOfBuffer;
		}
		m_ReadPos = 0;
	}

	// Read the rest of the bytes in a single read (guaranteed to fit):
	if (a_Count > 0)
	{
		memcpy(Dst, m_Buffer + m_ReadPos, a_Count);
		m_ReadPos += a_Count;
	}
	return true;
}





bool ByteBuffer::WriteBuf(const void* a_Buffer, size_t a_Count)
{
	
		CheckValid();
	PUTBYTES(a_Count);
	const char* Src = static_cast<const char*>(a_Buffer);  // So that we can do byte math
	ASSERT(m_BufferSize >= m_ReadPos);
	size_t BytesToEndOfBuffer = m_BufferSize - m_WritePos;
	if (BytesToEndOfBuffer <= a_Count)
	{
		// Reading across the ringbuffer end, read the first part and adjust parameters:
		memcpy(m_Buffer + m_WritePos, Src, BytesToEndOfBuffer);
		Src += BytesToEndOfBuffer;
		a_Count -= BytesToEndOfBuffer;
		m_WritePos = 0;
	}

	// Read the rest of the bytes in a single read (guaranteed to fit):
	if (a_Count > 0)
	{
		memcpy(m_Buffer + m_WritePos, Src, a_Count);
		m_WritePos += a_Count;
	}
	return true;
}





bool ByteBuffer::ReadString(std::string& a_String, size_t a_Count)
{
	
		CheckValid();
	NEEDBYTES(a_Count);
	a_String.clear();
	a_String.reserve(a_Count);
	ASSERT(m_BufferSize >= m_ReadPos);
	size_t BytesToEndOfBuffer = m_BufferSize - m_ReadPos;
	if (BytesToEndOfBuffer <= a_Count)
	{
		// Reading across the ringbuffer end, read the first part and adjust parameters:
		if (BytesToEndOfBuffer > 0)
		{
			a_String.assign(m_Buffer + m_ReadPos, BytesToEndOfBuffer);
			ASSERT(a_Count >= BytesToEndOfBuffer);
			a_Count -= BytesToEndOfBuffer;
		}
		m_ReadPos = 0;
	}

	// Read the rest of the bytes in a single read (guaranteed to fit):
	if (a_Count > 0)
	{
		a_String.append(m_Buffer + m_ReadPos, a_Count);
		m_ReadPos += a_Count;
	}
	return true;
}





bool ByteBuffer::SkipRead(size_t a_Count)
{
	
		CheckValid();
	if (!CanReadBytes(a_Count))
	{
		return false;
	}
	AdvanceReadPos(a_Count);
	return true;
}





void ByteBuffer::ReadAll(std::string& a_Data)
{
	
		CheckValid();
	ReadString(a_Data, GetReadableSpace());
}





bool ByteBuffer::ReadToByteBuffer(ByteBuffer& a_Dst, size_t a_NumBytes)
{
	
		if (!a_Dst.CanWriteBytes(a_NumBytes) || !CanReadBytes(a_NumBytes))
		{
			// There's not enough source bytes or space in the dest BB
			return false;
		}
	char buf[1024];
	// > 0 without generating warnings about unsigned comparisons where size_t is unsigned
	while (a_NumBytes != 0)
	{
		size_t num = (a_NumBytes > sizeof(buf)) ? sizeof(buf) : a_NumBytes;
		VERIFY(ReadBuf(buf, num));
		VERIFY(a_Dst.Write(buf, num));
		ASSERT(a_NumBytes >= num);
		a_NumBytes -= num;
	}
	return true;
}





void ByteBuffer::CommitRead(void)
{
	
		CheckValid();
	m_DataStart = m_ReadPos;
}





void ByteBuffer::ResetRead(void)
{
	
		CheckValid();
	m_ReadPos = m_DataStart;
}





void ByteBuffer::ReadAgain(std::string& a_Out)
{
	// Return the data between m_DataStart and m_ReadPos (the data that has been read but not committed)
	// Used by ProtoProxy to repeat communication twice, once for parsing and the other time for the remote party
	
		CheckValid();
	size_t DataStart = m_DataStart;
	if (m_ReadPos < m_DataStart)
	{
		// Across the ringbuffer end, read the first part and adjust next part's start:
		ASSERT(m_BufferSize >= m_DataStart);
		a_Out.append(m_Buffer + m_DataStart, m_BufferSize - m_DataStart);
		DataStart = 0;
	}
	ASSERT(m_ReadPos >= DataStart);
	a_Out.append(m_Buffer + DataStart, m_ReadPos - DataStart);
}


bool ByteBuffer::ReadVarUTF8String(std::string& a_Value)
{
	
	CheckValid();
	uint32_t Size = 0;
	if (!ReadVarInt(Size))
	{
		return false;
	}
	if (Size > MAX_STRING_SIZE)
	{
		//LOGWARNING("%s: String too large: %u (%u KiB)", __FUNCTION__, Size, Size / 1024);
	}
	return ReadString(a_Value, static_cast<size_t>(Size));
}

bool ByteBuffer::WriteVarUTF8String(std::string& a_Value)
{
	
		CheckValid();
	PUTBYTES(a_Value.size() + 1);  // This is a lower-bound on the bytes that will be actually written. Fail early.
	bool res = WriteVarInt32(static_cast<uint32_t>(a_Value.size()));
	if (!res)
	{
		return false;
	}
	return WriteBuf(a_Value.data(), a_Value.size());
}


void ByteBuffer::AdvanceReadPos(size_t a_Count)
{
	
		CheckValid();
	m_ReadPos += a_Count;
	if (m_ReadPos >= m_BufferSize)
	{
		m_ReadPos -= m_BufferSize;
	}
}





void ByteBuffer::CheckValid(void) const
{
	ASSERT(m_ReadPos < m_BufferSize);
	ASSERT(m_WritePos < m_BufferSize);
}





size_t ByteBuffer::GetVarIntSize(uint32_t a_Value)
{
	size_t Count = 0;

	do
	{
		// If the value cannot be expressed in 7 bits, it needs to take up another byte
		Count++;
		a_Value >>= 7;
	} while (a_Value != 0);

	return Count;
}
}