#ifndef PacketCollector_h
#define PacketCollector_h

#include "NStreamComCommon.h"
#include "Packet.h"
#include "PacketArray.h"

using PacketsReadyEventHandler = void (*)(const PacketArray&);

class PacketCollector
{
public:
	PacketCollector();

	void packetsReadyEvent(PacketsReadyEventHandler handler);

	void discard();

	bool collect(const uint8_t* const bytes, size_t length);

	bool collect(const Collection<uint8_t>& bytes);

	~PacketCollector();

private:

	bool recycle;
	PacketArray packetsCollected;
	uint16_t bytesCollected;
	PacketsReadyEventHandler packetsReady;
};

#endif // !PacketCollector_h