namespace NStreamCom
{
    public class PacketsReadyEventArgs
    {
        public readonly Packet[] Packets;

        public PacketsReadyEventArgs(Packet[] packets)
        {
            Packets = packets;
        }
    }

    public delegate void PacketsReadyEventHandler(object sender, PacketsReadyEventArgs e);
}
