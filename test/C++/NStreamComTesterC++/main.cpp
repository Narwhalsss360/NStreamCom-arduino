#include "NStreamCom.h"
#include <iostream>

#define SET_BYTE_INTEGRAL(number, index, value) ((uint8_t*)&number)[index] = value;

void serializeDeserializeTest()
{
	constexpr uint16_t packetSize = 2;
	uint64_t n;
	
	for (size_t i = 0; i < sizeof(n); i++)
		SET_BYTE_INTEGRAL(n, i, i);

	Message TXMessage = Message(1, &n, sizeof(n));
	Packet* packets;
	uint16_t packetCount = TXMessage.getPackets(packets, packetSize);

	Message RXMessage = Message(packets, packetCount);
	if (!RXMessage.isVerified())
	{
		__debugbreak();
	}
	void* recv = RXMessage.getData();

	if (RXMessage.getSize() != sizeof(n))
	{
		__debugbreak();
	}

	uint64_t nRecv = *(uint64_t*)recv;

	if (n != *(uint64_t*)RXMessage.getData())
	{
		__debugbreak();
	}

	delete[] packets;
}

constexpr uint64_t longNumberValue = 0xDEADBEEFC0FFEEAA;

void collectorTest()
{
	constexpr uint16_t packetSize = 2;
	uint64_t longNumber = longNumberValue;

	PacketCollector collector;
	collector.messageReady
	(
		[](const uint16_t id, void* data, uint16_t size)
		{
			if (size != sizeof(longNumber))
				__debugbreak();
			if (longNumberValue != *(uint64_t*)data)
				__debugbreak();
		}
	);

	Message TXMessage = Message(1, &longNumber, sizeof(longNumber));
	Packet* packets;
	uint16_t packetCount = TXMessage.getPackets(packets, 2);
	for (int i = 0; i < packetCount; i++)
	{
		uint8_t* streamBytes;
		uint32_t streamBytesSize = packets[i].getStreamBytes(streamBytes);
		collector.collect(streamBytes, streamBytesSize);
		delete[] streamBytes;
	}

	delete[] packets;
}

int main()
{
	serializeDeserializeTest();
	collectorTest();
	std::cout << "Press enter to exit.";
	std::cin.get();
}