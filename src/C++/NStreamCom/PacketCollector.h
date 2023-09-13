#ifndef PacketCollector_h
#define PacketCollector_h

#include "NStreamComCommon.h"
#include "Packet.h"

using PacketsReadyEventHandler = void (*)(const Collection<Packet>&);

class PacketCollector
{
public:
	PacketCollector();

	void onPacketsReady(PacketsReadyEventHandler handler);

	bool collect(uint8_t bytes[], size_t length);

	bool collect(const Collection<uint8_t>& bytes);

	~PacketCollector();

private:
	void discard();

	DynamicArray<Packet> packetsCollected;
	uint16_t bytesCollected;
	PacketsReadyEventHandler packetsReadyHandler;
};

#endif // !PacketCollector_h