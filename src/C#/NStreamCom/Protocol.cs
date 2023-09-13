using System;
using System.IO;

namespace NStreamCom
{
    public static class Protocol
    {
        public static readonly byte StreamProtocolSize = 6;

        public static void VerifyStream(Stream Stream)
        {
            byte[] StreamBytes = Stream.ToBytes();
            if (StreamBytes.Length < StreamProtocolSize)
                throw new PacketSizeDataBufferMismatch("Length of Stream was less than minimum");

            if (StreamBytes.Length - StreamProtocolSize != BitConverter.ToUInt16(StreamBytes, 4))
                throw new PacketSizeDataBufferMismatch("Length of Stream did not match its size with protocol");
        }
    }
}