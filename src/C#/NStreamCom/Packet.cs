using System;
using System.IO;
using System.Runtime.CompilerServices;

namespace NStreamCom
{
    public class Packet
    {
        public ushort ID { get; private set; }

        public ushort MessageSize { get; private set; }

        public MemoryStream Data { get; private set; }

        public int StreamBytesSize
        {
            get
            {
                return (int)Data.Length + Protocol.StreamProtocolSize;
            }
        }

        public Packet(ushort ID, ushort MessageSize, ushort PacketSize, byte[] Data)
            : this(ID, MessageSize, PacketSize, new MemoryStream(Data))
        {
        }

        public Packet(ushort ID, ushort MessageSize, ushort PacketSize, Stream Stream)
        {
            this.ID = ID;
            this.MessageSize = MessageSize;
            Data = new MemoryStream();
            Stream.CopyTo(Data);
        }

        public Packet(Stream Stream)
        {
            Protocol.VerifyStream(Stream);
            byte[] StreamBytes = Stream.ReadAll();
            ID = BitConverter.ToUInt16(StreamBytes, 0);
            MessageSize = BitConverter.ToUInt16(StreamBytes, 2);
            ushort PacketSize = BitConverter.ToUInt16(StreamBytes, 4);
            Data = new MemoryStream(StreamBytes, Protocol.StreamProtocolSize, PacketSize);
        }

        public Packet(byte[] Buffer)
            : this(new MemoryStream(Buffer))
        {
        }

        public Stream GetStreamBytes()
        {
            MemoryStream StreamBytes = new MemoryStream();
            StreamBytes.Write(BitConverter.GetBytes(ID), 0, 2);
            StreamBytes.Write(BitConverter.GetBytes(MessageSize), 0, 2);
            StreamBytes.Write(BitConverter.GetBytes((ushort)Data.Length), 0, 2);
            StreamBytes.Write(Data.ToArray(), 0, (int)Data.Length);
            return StreamBytes;
        }
    }
}