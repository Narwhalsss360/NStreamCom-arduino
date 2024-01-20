using System;

namespace NStreamCom
{
    [Serializable]
    public class IDMismatch : Exception
    {
        public IDMismatch() { }
        public IDMismatch(string message) : base(message) { }
        public IDMismatch(string message, Exception inner) : base(message, inner) { }
        protected IDMismatch(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context) : base(info, context) { }
    }
}
