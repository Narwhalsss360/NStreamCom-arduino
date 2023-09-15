using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http.Headers;

namespace NStreamCom
{
    public class PacketCollector
    {
        private List<Packet> PacketsCollected = new List<Packet>();
        private ushort BytesCollected = 0;
        private bool RecycleStream = false;
        public event PacketsReadyEventHandler PacketsReady;

        public PacketCollector()
        {
        }

        public void Discard()
        {
            BytesCollected = 0;
            PacketsCollected.Clear();
        }

        public void Collect(Stream Stream)
        {
            RecycleStream = false;
            Packet Collected;
            try
            {
                Collected = new Packet(Stream);
            }
            catch (PacketSizeDataBufferMismatch)
            {
                Discard();
                throw;
            }

            BytesCollected += (ushort)Collected.Data.Length;

            if (PacketsCollected.Count == 0)
            {
                PacketsCollected.Add(Collected);
                if (Collected.MessageSize != (ushort)Collected.Data.Length)
                    return;
            }
            else
            {
                if (Collected.ID == PacketsCollected.Last().ID)
                {
                    PacketsCollected.Add((Packet)Collected);
                    if (BytesCollected != Collected.MessageSize)
                        return;
                }
                else
                {
                    if (BytesCollected != PacketsCollected.Last().MessageSize)
                    {
                        Discard();
                        throw new PacketsMessageSizeMismatch("Did not receive all bytes");
                    }
                    RecycleStream = true;
                }
            }

            PacketsReady?.Invoke(this, new PacketsReadyEventArgs(PacketsCollected.ToArray()));
            Discard();
            if (RecycleStream)
                Collect(Stream);
        }

        public void Collect(byte[] Data)
        {
            Collect(new MemoryStream(Data));
        }
    }
}