#ifndef Packet_h
#define Packet_h

#include "NStreamComCommon.h"

class Packet;

bool verifyBytes(const Collection<uint8_t>& bytes);
bool verifyPackets(const Collection<uint8_t>& packets);

class Packet
{
public:
	Packet(messageid_t id, uint16_t messageSize, const Collection<uint8_t>& data);

	Packet(const Collection<uint8_t>& streamBytes);

	DynamicArray<uint8_t> getStreamBytes();

	bool isVerified();

	~Packet();

private:
	messageid_t id;
	uint16_t messageSize;
	DynamicArray<uint8_t> data;
	bool verified;
};

#endif // !Packet_h