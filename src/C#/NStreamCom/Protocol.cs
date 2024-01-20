using System;

namespace NStreamCom
{
    public static class Protocol
    {
        public static readonly int MetadataSize = 6;

        public static void VerifyBytes(byte[] bytes)
        {
            if (bytes.Length < MetadataSize)
                throw new SizeMismatch();

            if (bytes.Length - MetadataSize != BitConverter.ToUInt16(bytes, 4))
                throw new SizeMismatch();
        }

        public static void VerifyPackets(this Packet[] packets)
        {
            if (packets.Length == 0)
                throw new ArgumentException($"{nameof(packets)} was empty.");

            ushort id = packets[0].ID;
            ushort messageSize = packets[0].MessageSize;
            ushort sizeSum = 0;

            foreach (Packet packet in packets)
            {
                if (packet.ID != id)
                    throw new IDMismatch();
                if (packet.MessageSize != messageSize)
                    throw new SizeMismatch();
                sizeSum += (ushort)packet.Data.Length;
            }

            if (sizeSum != messageSize)
                throw new SizeMismatch();
        }

        public static byte[][] GetPacketsBytes(this Packet[] packets)
        {
            byte[][] packetsBytes = new byte[packets.Length][];
            for (int i = 0; i < packets.Length; i++)
                packetsBytes[i] = packets[i].GetStreamBytes();
            return packetsBytes;
        }
    }
}
