using System;

namespace NStreamCom
{
    public delegate void PacketsReadyEventHandler(object Sender, PacketsReadyEventArgs Args);

    public class PacketsReadyEventArgs : EventArgs
    {
        public readonly Packet[] Packets;

        public PacketsReadyEventArgs(Packet[] Packets)
            : base()
        {
            this.Packets = Packets;
        }
    }
}