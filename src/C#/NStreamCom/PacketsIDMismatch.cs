using System;

namespace NStreamCom
{
    public class PacketsIDMismatch : Exception
    {
        public PacketsIDMismatch(string Message = "")
            : base(Message)
        {
        }
    }
}