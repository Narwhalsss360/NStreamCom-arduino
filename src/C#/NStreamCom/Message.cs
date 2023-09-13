using System;
using System.IO;

namespace NStreamCom
{
    public class Message
    {
        public ushort ID { get; private set; }
        public MemoryStream Data;

        public Message(ushort ID, MemoryStream Data)
        {
            this.ID = ID;
            this.Data = Data;
        }

        public Message(ushort ID, byte[] Data)
        {
            this.ID = ID;
            this.Data = new MemoryStream(Data);
        }

        public Message(Packet[] Packets)
        {
            Packets.VerifyPacketsForMessage();
            ID = Packets[0].ID;
            Data = new MemoryStream();
            foreach (Packet Packet in Packets)
                Data.Write(Packet.Data.ToArray(), 0, (int)Packet.Data.Length);
        }

        public Packet[] GetPackets(ushort PacketSize)
        {
            if (PacketSize >= Data.Length)
                return new Packet[] { new Packet(ID, (ushort)Data.Length, (ushort)Data.Length, Data) };

            byte PacketCount = (byte)Math.Ceiling((double)Data.Length / PacketSize);
            Packet[] Packets = new Packet[PacketCount];

            for (int iPacket = 0; iPacket < PacketCount; iPacket++)
            {
                PacketSize = (ushort)(iPacket == PacketCount - 1 ? Data.Length - (iPacket * PacketSize): PacketSize);
                Packets[iPacket] = new Packet(ID, (ushort)Data.Length, PacketSize, new MemoryStream(Data.ToArray(), (iPacket == PacketCount - 1) ? (ushort)Data.Length - PacketSize : iPacket * PacketSize, PacketSize));
            }

            return Packets;
        }

        void WriteTo(ushort PacketSize, Stream Stream)
        {
            GetPackets(PacketSize).GetStreams().WriteStreamsTo(Stream);
        }
    }
}