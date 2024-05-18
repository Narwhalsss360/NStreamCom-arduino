#ifndef Message_h
#define Message_h

#include "NStreamComCommon.h"
#include "Packet.h"

bool verifyPackets(const Packet* const packets, uint16_t length);
void fastWrite(__platform_ostream__& stream, const Packet* const packets, uint16_t length, uint32_t waitTimems);

class Message
{
public:
	Message(Message& other);

	Message(Message&& other);

	Message(const messageid_t id, const void* const data, const uint16_t length);

	Message(const Packet* const packets, const uint16_t length);

	messageid_t getID() const;

	uint16_t getSize() const;

	uint16_t getPackets(Packet*& packets, uint16_t packetSize) const;

	void* getData() const;

	void fastWrite(const uint16_t packetSize, __platform_ostream__& stream, uint32_t waitTimems = 0) const;

	bool isVerified() const;

	~Message();

private:
	messageid_t id;
	uint8_t* data;
	uint16_t dataLength;
	bool verified;
};

#endif // !Message_h