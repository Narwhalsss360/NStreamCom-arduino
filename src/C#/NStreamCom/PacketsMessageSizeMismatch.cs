using System;

namespace NStreamCom
{
    public class PacketsMessageSizeMismatch : Exception
    {
        public PacketsMessageSizeMismatch(string Message = "")
            : base(Message)
        {
        }
    }
}