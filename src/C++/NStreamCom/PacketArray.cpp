#include "PacketArray.h"
#include "Message.h"

PacketArray::PacketArray(const Message& message, const size_t packetSize)
	: DynamicArray<Packet>(message.getPackets(packetSize))
{
}

PacketArray::PacketArray(PacketArray& other)
	: DynamicArray<Packet>(other)
{
}

PacketArray::PacketArray(PacketArray&& other)
	: DynamicArray<Packet>(other)
{
}

DynamicArray<DynamicArray<uint8_t>> PacketArray::getStreamsBytes()
{
	DynamicArray<DynamicArray<uint8_t>> streamsBytes;

	for (index_t iPacket = 0; iPacket < Length(); iPacket++)
		streamsBytes += (this->operator[](iPacket)).getStreamBytes();

	return streamsBytes;
}

void PacketArray::fastWrite(const __platform_ostream__& stream)
{
	for (const Collection<uint8_t>& packetBytes : getStreamsBytes())
		for (const uint8_t& byte : packetBytes)
			__platform_ostream__write(stream, byte);
}

PacketArray::~PacketArray()
{
}

PacketArray::PacketArray()
	: DynamicArray<Packet>()
{
}