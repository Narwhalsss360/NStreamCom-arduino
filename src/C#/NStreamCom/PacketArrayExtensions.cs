using System;
using System.IO;

namespace NStreamCom
{
    public static class PacketArrayExtensions
    {
        public static void VerifyPacketsForMessage(this Packet[] Packets)
        {
            if (Packets.Length == 0)
            {
                throw new ArgumentException("Argument \"Packets\" was empty");
            }

            ushort ID = Packets[0].ID;
            ushort MessageSize = Packets[0].MessageSize;
            ushort MessageSizeTotal = 0;

            foreach (Packet Packet in Packets)
            {
                if (Packet.ID != ID)
                    throw new PacketsIDMismatch("Packets did not have same ID");
                if (Packet.MessageSize != MessageSize)
                    throw new PacketsMessageSizeMismatch("Packets did not have the same MessageSize");
                MessageSizeTotal += Packet.PacketSize;
            }

            if (MessageSizeTotal != Packets[0].MessageSize)
                throw new PacketsMessageSizeMismatch("Sum of packets did not match MessageSize");
        }

        public static Stream[] GetStreams(this Packet[] Packets)
        {
            MemoryStream[] Streams = new MemoryStream[Packets.Length];
            for (int i = 0; i < Packets.Length; i++)
                Streams[i] = Packets[i].Data;
            return Streams;
        }
    }
}