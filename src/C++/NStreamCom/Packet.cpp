#include "Packet.h"

#define SET_BYTE_INTEGRAL(number, index, value) ((uint8_t*)&number)[index] = value;

Packet::Packet(messageid_t id, uint16_t messageSize, const Collection<uint8_t>& data)
    :id(id), messageSize(messageSize), data(DynamicArray<uint8_t>()), verified(true)
{
    for (const uint8_t& byte : data)
        this->data += byte;
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

    for (index_t iSource = NSTREAMCOM_PROTOCOLSIZE; iSource < streamBytes.Length(); iSource++)
        data += streamBytes[iSource];
}

bool Packet::isVerified()
{
    return verified;
}