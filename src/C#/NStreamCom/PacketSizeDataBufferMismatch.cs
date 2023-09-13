using System;

namespace NStreamCom
{
    public class PacketSizeDataBufferMismatch : Exception
    {
        public PacketSizeDataBufferMismatch(string Message = "")
            : base(Message)
        {
        }
    }
}
