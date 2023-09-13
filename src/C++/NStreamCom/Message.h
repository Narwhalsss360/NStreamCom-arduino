#ifndef Message_h
#define Message_h

#include "NStreamComCommon.h"
#include "Packet.h"

class Message
{
public:
	Message(messageid_t id, const Collection<uint8_t>& data);

	DynamicArray<Packet> getPackets(uint16_t packetSize);

	void fastWrite(uint16_t packetSize, __platform_ostream__& stream);

	~Message();

private:
	messageid_t id;
	DynamicArray<uint8_t> data;
};

#endif // !Message_h