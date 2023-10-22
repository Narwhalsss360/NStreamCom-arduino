#ifndef Message_h
#define Message_h

#include "NStreamComCommon.h"
#include "Packet.h"
#include "PacketArray.h"

bool verifyPackets(const Collection<Packet>& packets);

class PacketArray;

class Message
{
public:
	Message(Message& other);

	Message(Message&& other);

	Message(const messageid_t id, const Collection<uint8_t>& data);

	Message(const messageid_t id, const void* const data, const size_t length);

	Message(const PacketArray& packets);

	messageid_t getID();

	uint16_t getSize();

	PacketArray getPackets(const uint16_t packetSize) const;

	DynamicArray<uint8_t> getData();

	void fastWrite(const uint16_t packetSize, __platform_ostream__& stream) const;

	bool isVerified();

	~Message();

private:
	messageid_t id;
	DynamicArray<uint8_t> data;
	bool verified;
};

#endif // !Message_h