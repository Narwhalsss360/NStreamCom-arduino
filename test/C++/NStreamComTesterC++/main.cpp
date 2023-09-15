#include "NStreamCom.h"
#include <iostream>

#define SET_BYTE_INTEGRAL(number, index, value) ((uint8_t*)&number)[index] = value;

void showPacketsBytes(PacketArray packets)
{
	for (size_t i = 0; i < packets.Length(); i++)
	{
		std::cout << "Packet " << i << ":\n";
		auto d = packets[i].getData();

		for (size_t i2 = 0; i2 < d.Length(); i2++)
		{
			std::cout << "    [" << i2 << "]:" << (int)d[i2] << '\n';
		}
	}
}

void serializeDeserializeTest()
{
	constexpr uint16_t packetSize = 2;
	uint64_t n;
	
	for (size_t i = 0; i < sizeof(n); i++)
		SET_BYTE_INTEGRAL(n, i, i);

	Message TXMessage = Message(1, &n, sizeof(n));
	PacketArray TXPackets = TXMessage.getPackets(packetSize);
	showPacketsBytes(TXPackets);
	DynamicArray<DynamicArray<uint8_t>> packetsStreamsBytes = TXPackets.getStreamsBytes();

	PacketArray RXPackets = PacketArray();
	for (const Collection<uint8_t>& streamBytes : packetsStreamsBytes)
		RXPackets += Packet(streamBytes);
	{
		Message RXMessage = Message(RXPackets);
		DynamicArray<uint8_t> RXData = RXMessage.getData();
		void* rxptr = &RXData[0];

		if (n != *(uint64_t*)rxptr)
		{
			__debugbreak();
		}
	}
}

int main()
{
	serializeDeserializeTest();
	std::cout << "Press enter to exit.";
	std::cin.get();
}