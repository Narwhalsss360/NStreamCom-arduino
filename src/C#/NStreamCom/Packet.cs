using System;
using System.IO;

namespace NStreamCom
{
    public class Packet
    {
        public ushort ID;

        public ushort MessageSize;

        public byte[] Data;

        public int StreamBytesSize
        {
            get => Data.Length + Protocol.MetadataSize;
        }

        public Packet(ushort id, ushort messageSize, byte[] data)
        {
            ID = id;
            MessageSize = messageSize;
            Data = data;
        }

        public Packet(ushort id, ushort messageSize, Stream data)
        {
            ID = id;
            MessageSize = messageSize;
            Data = data.ReadAllBytes();
        }

        public Packet(byte[] data)
        {
            Protocol.VerifyBytes(data);
            ID = BitConverter.ToUInt16(data, 0);
            MessageSize = BitConverter.ToUInt16(data, 2);
            Data = new byte[BitConverter.ToUInt16(data, 4)];
            Array.Copy(data, 6, Data, 0, Data.Length);
        }

        public Packet(Stream stream)
            : this(stream.ReadAllBytes())
        {
        }

        public byte[] GetStreamBytes()
        {
            byte[] streamBytes = new byte[StreamBytesSize];
            Array.Copy(BitConverter.GetBytes(ID), 0, streamBytes, 0, 2);
            Array.Copy(BitConverter.GetBytes(MessageSize), 0, streamBytes, 2, 2);
            Array.Copy(BitConverter.GetBytes((ushort)Data.Length), 0, streamBytes, 4, 2);
            Array.Copy(Data, 0, streamBytes, 6, Data.Length);
            return streamBytes;
        }
    }
}
