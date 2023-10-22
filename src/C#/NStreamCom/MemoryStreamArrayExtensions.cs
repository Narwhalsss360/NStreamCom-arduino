using System.IO;

namespace NStreamCom
{
    public static class MemoryStreamArrayExtensions
    {
        public static void WriteStreamsTo(this Stream[] Source, Stream Destination)
        {
            foreach (var Stream in Source)
                Destination.Write(Stream);
        }
    }
}
