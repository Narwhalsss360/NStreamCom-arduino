using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace NStreamCom
{
    public class PacketCollector
    {
        private List<Packet> _packetsCollected = new List<Packet>();

        private ushort _bytesCollected = 0;

        private bool _recycle = false;

        public event PacketsReadyEventHandler PacketsReady;

        public void Discard()
        {
            _bytesCollected = 0;
            _packetsCollected.Clear();
        }

        public void Collect(byte[] data)
        {
            _recycle = false;
            Packet collected;
            try
            {
                collected = new Packet(data);
            }
            catch (SizeMismatch)
            {
                Discard();
                throw;
            }

            _bytesCollected += (ushort)collected.Data.Length;

            if (_packetsCollected.Count == 0)
            {
                _packetsCollected.Add(collected);
                if (collected.MessageSize != collected.Data.Length)
                    return;
            }
            else
            {
                if (collected.ID == _packetsCollected.Last().ID)
                {
                    _packetsCollected.Add(collected);
                    if (_bytesCollected != collected.MessageSize)
                        return;
                }
                else
                {
                    if (_bytesCollected != _packetsCollected.Last().MessageSize)
                    {
                        Discard();
                        throw new PacketsLost();
                    }
                    _recycle = true;
                }
            }

            PacketsReady?.Invoke(this, new PacketsReadyEventArgs(_packetsCollected.ToArray()));
            Discard();
            if (_recycle)
                Collect(data);
        }

        public void Collect(Stream data)
        {
            Collect(data.ReadAllBytes());
        }
    }
}
