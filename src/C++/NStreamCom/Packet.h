#ifndef Packet_h
#define Packet_h

#include "NStreamComCommon.h"

class Packet;

bool VerifyBytes(uint8_t bytes[], size_t length);
bool VerifyPackets(Packet packets[], size_t length);

class Packet
{
public:
	Packet(messageid_t id, uint16_t messageSize, const Collection<uint8_t>& data);

	DynamicArray<uint8_t> getStreamBytes();

	~Packet();

private:
	messageid_t id;
	uint16_t messageSize;
	DynamicArray<uint8_t> data;
};

#endif // !Packet_h