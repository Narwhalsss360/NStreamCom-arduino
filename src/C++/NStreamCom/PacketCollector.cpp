#include "PacketCollector.h"

PacketCollector::PacketCollector()
	: recycle(false), packetsCollected(PacketArray()), bytesCollected(0), packetsReady(nullptr)
{
}

void PacketCollector::packetsReadyEvent(PacketsReadyEventHandler handler)
{
	packetsReady = handler;
}

void PacketCollector::discard()
{
	recycle = false;
	packetsCollected.SetCapacity(0);
	bytesCollected = 0;
}

bool PacketCollector::collect(const uint8_t* const bytes, size_t length)
{
	DynamicArray<uint8_t> collectionBytes;
	for (size_t i = 0; i < length; i++)
		collectionBytes += bytes[i];
	return collect(collectionBytes);
}

bool PacketCollector::collect(const Collection<uint8_t>& bytes)
{
	recycle = false;
	Packet collected(bytes);
	if (!collected.isVerified())
	{
		discard();
		return false;
	}

	bytesCollected += collected.getDataSize();

	if (packetsCollected.Length() == 0)
	{
		packetsCollected += collected;
		if (collected.getDataSize() != collected.getMessageSize())
			return true;
	}
	else
	{
		if (collected.getID() == packetsCollected[packetsCollected.Length() - 1].getID())
		{
			packetsCollected += collected;
			if (bytesCollected != collected.getMessageSize())
				return true;
		}
		else
		{
			if (bytesCollected != collected.getMessageSize())
			{
				discard();
				return false;
			}
			recycle = true;
		}
	}

	if (packetsReady)
		packetsReady(packetsCollected);
	discard();
	if (recycle)
		return collect(bytes);
	return true;
}

PacketCollector::~PacketCollector()
{
}