using System;

namespace NStreamCom
{
    [Serializable]
    public class IDMismatchException : Exception
    {
        public IDMismatchException() { }
        public IDMismatchException(string message) : base(message) { }
        public IDMismatchException(string message, Exception inner) : base(message, inner) { }
        protected IDMismatchException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context) : base(info, context) { }
    }
}
