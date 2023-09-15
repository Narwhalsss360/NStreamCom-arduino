#ifndef PacketArray_h
#define PacketArray_h

#include "NStreamComCommon.h"
#include "Packet.h"

class Message;

class PacketArray : public DynamicArray<Packet>
{
public:
	PacketArray();

	PacketArray(const Message& message, const size_t packetSize);

	PacketArray(PacketArray& other);

	PacketArray(PacketArray&& other);

	DynamicArray<DynamicArray<uint8_t>> getStreamsBytes();

	void fastWrite(const __platform_ostream__& stream);

	~PacketArray();

private:
	friend class Message;
};

#endif // !PacketArray_h