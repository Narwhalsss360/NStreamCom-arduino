#include "Packet.h"

#define SET_BYTE_INTEGRAL(number, index, value) ((uint8_t*)&number)[index] = value;
#define GET_BYTE_INTEGRAL(number, index) ((uint8_t*)&number)[index];

bool verifyBytes(const Collection<uint8_t>& bytes)
{
    if (bytes.Length() < NSTREAMCOM_PROTOCOLSIZE)
        return false;

    if (bytes.Length() - NSTREAMCOM_PROTOCOLSIZE != *(uint16_t*)&bytes[4])
        return false;

    return true;
}

Packet::Packet()
    : id(0), messageSize(0), data(DynamicArray<uint8_t>()), verified(false)
{
}

Packet::Packet(messageid_t id, uint16_t messageSize, const Collection<uint8_t>& data)
    :id(id), messageSize(messageSize), data(DynamicArray<uint8_t>()), verified(true)
{
    for (const uint8_t& byte : data)
        this->data += byte;
}

Packet::Packet(messageid_t id, uint16_t messageSize, const void* const data, const size_t length)
    :id(id), messageSize(messageSize), data(DynamicArray<uint8_t>()), verified(true)
{
    for (size_t i = 0; i < length; i++)
        this->data += ((const uint8_t* const)data)[i];
}

Packet::Packet(const Collection<uint8_t>& streamBytes)
    : id(0), messageSize(0), data(DynamicArray<uint8_t>()), verified(false)
{
    verified = verifyBytes(streamBytes);
    if (!verified)
        return;
    
    SET_BYTE_INTEGRAL(id, 0, streamBytes[0]);
    SET_BYTE_INTEGRAL(id, 1, streamBytes[1]);

    SET_BYTE_INTEGRAL(messageSize, 0, streamBytes[2]);
    SET_BYTE_INTEGRAL(messageSize, 1, streamBytes[3]);

    uint16_t lengthOfPacket = 0;
    SET_BYTE_INTEGRAL(lengthOfPacket, 0, streamBytes[4]);
    SET_BYTE_INTEGRAL(lengthOfPacket, 1, streamBytes[5]);

    for (size_t i = NSTREAMCOM_PROTOCOLSIZE; i < streamBytes.Length(); i++)
        data += streamBytes[i];
}

DynamicArray<uint8_t> Packet::getStreamBytes() const
{
    DynamicArray<uint8_t> streamBytes;
    streamBytes += GET_BYTE_INTEGRAL(id, 0);
    streamBytes += GET_BYTE_INTEGRAL(id, 1);

    streamBytes += GET_BYTE_INTEGRAL(messageSize, 0);
    streamBytes += GET_BYTE_INTEGRAL(messageSize, 1);

    uint16_t lengthOfPacket = data.Length();
    streamBytes += GET_BYTE_INTEGRAL(lengthOfPacket, 0);
    streamBytes += GET_BYTE_INTEGRAL(lengthOfPacket, 1);

    streamBytes += data;

    return streamBytes;
}

DynamicArray<uint8_t> Packet::getData() const
{
    return DynamicArray<uint8_t>(data);
}

messageid_t Packet::getID() const
{
    return id;
}

uint16_t Packet::getMessageSize() const
{
    return messageSize;
}

uint16_t Packet::getDataSize() const
{
    return data.Length();
}

bool Packet::isVerified() const
{
    return verified;
}

bool Packet::operator==(const Packet& other) const
{
    return id == other.id && messageSize == other.messageSize && data == other.data;
}

Packet::~Packet()
{
}