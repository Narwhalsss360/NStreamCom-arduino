using System.IO;

namespace NStreamCom
{
    public static class StreamExtensions
    {
        public static byte[] ToBytes(this Stream Stream)
        {
            byte[] Bytes = new byte[Stream.Length];
            Stream.Seek(0, SeekOrigin.Begin);
            Stream.Read(Bytes, 0, Bytes.Length);
            return Bytes;
        }
    }
}