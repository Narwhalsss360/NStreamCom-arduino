using System;
using System.IO;
using System.Linq;

namespace NStreamCom
{
    public class Message
    {
        public ushort ID;

        public byte[] Data;

        public Message(ushort id, byte[] data)
        {
            ID = id;
            Data = data;
        }

        public Message(ushort id, Stream data)
            : this(id, data.ReadAllBytes())
        {
        }

        public Message(Packet[] packets)
        {
            Protocol.VerifyPackets(packets);
            ID = packets[0].ID;
            Data = new byte[packets[0].MessageSize];

            int sum = 0;
            foreach (Packet packet in packets)
            {
                Array.Copy(packet.Data, 0, Data, sum, packet.Data.Length);
                sum += packet.Data.Length;
            }
        }

        public Packet[] GetPackets(ushort packetSize)
        {
            if (packetSize >= Data.Length)
                return new Packet[] { new Packet(ID, (ushort)Data.Length, Data) };

            int packetCount = (int)Math.Ceiling((double)Data.Length / packetSize);
            Packet[] packets = new Packet[packetCount];

            for (int i = 0; i < packetCount; i++)
            {
                if (i == packetCount - 1)
                    packetSize = (ushort)(Data.Length - (i * packetSize));
                packets[i] = new Packet(ID, (ushort)Data.Length, Data.Skip(i == packetCount - 1 ? Data.Length - packetSize : packetSize).Take(packetSize).ToArray());
            }

            return packets;
        }
    }
}
