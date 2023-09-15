#include "Message.h"

bool verifyPackets(const Collection<Packet>& packets)
{
	if (packets.Length() == 0)
		return false;

	uint16_t id = packets[0].getID();
	uint16_t messageSize = packets[0].getMessageSize();
	uint16_t messageSizeSum = 0;

	for (const Packet& packet : packets)
	{
		if (!packet.isVerified())
			return false;

		if (packet.getID() != id)
			return false;

		if (packet.getMessageSize() != messageSize)
			return false;

		messageSizeSum += packet.getDataSize();
	}

	return messageSizeSum == packets[0].getMessageSize();
}

Message::Message(Message& other)
	: id(other.id), data(other.data), verified(other.verified)
{
}

Message::Message(Message&& other)
	: id(other.id), data(other.data), verified(other.verified)
{
}

Message::Message(const messageid_t id, const Collection<uint8_t>& data)
	: id(id), data(DynamicArray<uint8_t>()), verified(true)
{
	this->data += data;
}

Message::Message(const messageid_t id, const void* const data, const size_t length)
	: id(id), data(DynamicArray<uint8_t>()), verified(true)
{
	for (size_t i = 0; i < length; i++)
		this->data += ((const uint8_t* const)data)[i];
}

Message::Message(const PacketArray& packets)
	: id(0), data(DynamicArray<uint8_t>()), verified(false)
{
	verified = verifyPackets(packets);
	if (!verified)
		return;

	id = packets[0].getID();

	for (const Packet& packet : packets)
		data += packet.getData();
}

messageid_t Message::getID()
{
	return id;
}

uint16_t Message::getSize()
{
	return data.Length();
}

PacketArray Message::getPackets(const uint16_t packetSize) const
{
	PacketArray packets;
	if (data.Length() <= packetSize)
	{
		packets += Packet(id, data.Length(), data);
		return packets;
	}

	uint8_t packetCount = (data.Length() + packetSize - 1) / packetSize;
	packets.SetCapacity(packetCount);

	for (index_t iPacket = 0; iPacket < packetCount; iPacket++)
		packets += Packet(id, data.Length(), &(data[iPacket * packetSize]), (size_t)(iPacket == packetCount - 1 ? data.Length() - (iPacket * packetSize) : packetSize));

	return packets;
}

DynamicArray<uint8_t> Message::getData()
{
	return data;
}

void Message::fastWrite(const uint16_t packetSize, const __platform_ostream__& stream) const
{
	getPackets(packetSize).fastWrite(stream);
}

bool Message::isVerified()
{
	return verified;
}

Message::~Message()
{
}