using System.IO;

namespace NStreamCom
{
    public static class StreamExtensions
    {
        public static byte[] ReadAll(this Stream Stream)
        {
            byte[] Bytes = new byte[Stream.Length];
            Stream.Seek(0, SeekOrigin.Begin);
            Stream.Read(Bytes, 0, Bytes.Length);
            return Bytes;
        }

        public static void Write(this Stream Destination, Stream Source)
        {
            Destination.Write(Source.ReadAll(), 0, (int)Source.Length);
        }
    }
}