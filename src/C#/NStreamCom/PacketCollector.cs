using System.Collections.Generic;
using System.IO;
using System.Linq;

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
                    goto Check;
            }

            if (Collected.ID == PacketsCollected.Last().ID)
            {
                PacketsCollected.Add((Packet)Collected);
            }
            else
            {
                if (BytesCollected != PacketsCollected.Last().MessageSize)
                {
                    Discard();

                }
                RecycleStream = true;
                goto Ready;
            }

        Check:
            if (Collected.MessageSize != (ushort)Collected.Data.Length)
                return;
            Ready:
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