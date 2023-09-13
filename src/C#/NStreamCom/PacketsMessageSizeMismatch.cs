using System;

namespace NStreamCom
{
    internal class PacketsMessageSizeMismatch : Exception
    {
        public PacketsMessageSizeMismatch(string Message = "")
            : base(Message)
        {
        }
    }
}