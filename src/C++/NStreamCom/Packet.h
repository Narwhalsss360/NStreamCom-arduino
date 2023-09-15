#ifndef Packet_h
#define Packet_h

#include "NStreamComCommon.h"

class Packet;

bool verifyBytes(const Collection<uint8_t>& bytes);

class Packet
{
public:
	Packet();

	Packet(messageid_t id, uint16_t messageSize, const Collection<uint8_t>& data);

	Packet(messageid_t id, uint16_t messageSize, const void * const data, const size_t length);

	Packet(const Collection<uint8_t>& streamBytes);

	DynamicArray<uint8_t> getStreamBytes() const;

	bool isVerified() const;

	DynamicArray<uint8_t> getData() const;

	messageid_t getID() const;

	uint16_t getMessageSize() const;

	uint16_t getDataSize() const;

	bool operator==(const Packet& other) const;

	~Packet();

private:
	messageid_t id;
	uint16_t messageSize;
	DynamicArray<uint8_t> data;
	bool verified;
};

#endif // !Packet_h