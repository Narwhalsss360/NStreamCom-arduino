using System.Threading;
using System.IO;

namespace NStreamCom
{
    public static class MemoryStreamArrayExtensions
    {
        public static void WriteStreamsTo(this Stream[] Source, Stream Destination, int WaitTime = 0)
        {
            foreach (var Stream in Source)
            {
                Destination.Write(Stream);
                Thread.Sleep(WaitTime);
            }
        }
    }
}
