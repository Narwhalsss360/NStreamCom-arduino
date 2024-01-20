using System;
using System.IO;
using System.Threading;

namespace NStreamCom
{
    public static class StreamExtensions
    {
        public static byte[] ReadAllBytes(this Stream stream)
        {
            using (BinaryReader reader = new BinaryReader(stream))
                return reader.ReadBytes((int)stream.Length);
        }

        public static void WriteAllPacketsTo(this byte[][] bytes, Stream stream, TimeSpan? interval = null)
        {
            foreach (var packetBytes in bytes)
            {
                stream.Write(packetBytes, 0, packetBytes.Length);
                if (interval is null)
                    continue;
                Thread.Sleep(interval.Value);
            }
        }
    }
}
