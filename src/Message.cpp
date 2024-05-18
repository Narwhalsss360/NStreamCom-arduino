#include "Message.h"

bool verifyPackets(const Packet* const packets, uint16_t length)
{
	if (length == 0)
		return false;

	uint16_t id = packets[0].getID();
	uint16_t messageSize = packets[0].getMessageSize();
	uint16_t messageSizeSum = 0;

	for (uint16_t i = 0; i < length; i++)
	{
		const Packet& packet = packets[i];

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

void fastWrite(__platform_ostream__& stream, const Packet* const packets, uint16_t length, uint32_t waitTimems = 0)
{
	for (uint16_t iPacket = 0; iPacket < length; iPacket++)
	{
		uint8_t* bytes;
		uint32_t byteCount = packets[iPacket].getStreamBytes(bytes);
		__platform_ostream_write_bytes(stream, bytes, byteCount);
		delete[] bytes;
		__platform_sleeper(waitTimems);
	}
}

Message::Message(Message& other)
	: id(other.id), data(nullptr), dataLength(other.dataLength), verified(other.verified)
{
	if (dataLength == 0)
		return;

	data = new uint8_t[dataLength];
	for (int i = 0; i < dataLength; i++)
		data[i] = other.data[i];
}

Message::Message(Message&& other)
	: id(other.id), data(other.data), dataLength(other.dataLength), verified(other.verified)
{
}

Message::Message(const messageid_t id, const void* const data, const uint16_t length)
	: id(id), data(nullptr), dataLength(length), verified(true)
{
	if (data == nullptr)
		dataLength = 0;
	if (dataLength == 0)
		return;

	this->data = new uint8_t[dataLength];

	for (int i = 0; i < dataLength; i++)
		this->data[i] = ((uint8_t*)data)[i];
}

Message::Message(const Packet* const packets, const uint16_t length)
	: id(0), data(nullptr), dataLength(0), verified(false)
{
	verified = verifyPackets(packets, length);
	if (!verified)
		return;

	id = packets[0].getID();
	dataLength = packets[0].getMessageSize();
	if (dataLength == 0)
		return;

	data = new uint8_t[dataLength];

	for (uint16_t iDestination = 0, iPacket = 0, iSource = 0; iDestination < dataLength; iDestination++, iSource++)
	{
		if (packets[iPacket].getDataSize() == iSource)
		{
			iSource = 0;
			iPacket++;
		}
		if (dataLength != 0)
			data[iDestination] = ((uint8_t*)packets[iPacket].getData())[iSource];
	}
}

messageid_t Message::getID() const
{
	return id;
}

uint16_t Message::getSize() const
{
	return dataLength;
}

uint16_t Message::getPackets(Packet*& packets, uint16_t packetSize) const
{
	if (dataLength <= packetSize)
	{
		packets = new Packet(id, dataLength, data, dataLength);
		return 1;
	}

	uint8_t packetCount = (dataLength + packetSize - 1) / packetSize;
	packets = new Packet[packetCount];

	for (uint16_t iPacket = 0; iPacket < packetCount; iPacket++)
	{
		packets[iPacket] = Packet(id,
		dataLength,
		data ? &(data[iPacket * packetSize]) : nullptr,
		(iPacket == packetCount - 1 ? dataLength - (iPacket * packetSize) : packetSize));
	}

	return packetCount;
}

void* Message::getData() const
{
	return data;
}

void Message::fastWrite(const uint16_t packetSize, __platform_ostream__& stream, uint32_t waitTimems) const
{
	Packet* packets;
	uint16_t packetCount = getPackets(packets, packetSize);
	if (packets == nullptr)
		return;

	::fastWrite(stream, packets, packetCount, waitTimems);

	if (packetCount > 1)
		delete[] packets;
	else
		delete packets;
}

bool Message::isVerified() const
{
	return verified;
}

Message::~Message()
{
	if (data)
		delete[] data;
	data = nullptr;
}
