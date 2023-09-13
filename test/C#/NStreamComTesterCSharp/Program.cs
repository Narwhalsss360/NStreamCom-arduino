using NStreamCom;

SerializeDeserializeTest();

void SerializeDeserializeTest()
{
    ushort ID = 4;
    byte[] Data = new byte[97];
    ushort PacketSize = 7;

    for (int i = 0; i <  Data.Length; i++)
        Data[i] = (byte)i;

    Message TXMessage = new Message(ID, Data);
    Packet[] TXPackets = TXMessage.GetPackets(PacketSize);
    Stream[] Streams = TXPackets.GetStreams();
    Packet[] RXPackets = new Packet[Streams.Length];
    for (int i = 0; i < Streams.Length; i++)
        RXPackets[i] = new Packet(Streams[i]);
    Message RXMessage = new Message(RXPackets);

    if (ID != RXMessage.ID)
    {

    }

    for (int i = 0;i < RXMessage.Data.Length; i++)
    {
        RXMessage.Data.Seek(0, SeekOrigin.Begin);
        if (Data[i] != RXMessage.Data.ReadByte())
        {
        }
    }
}