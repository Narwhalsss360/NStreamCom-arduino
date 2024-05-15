using System;

namespace NStreamCom
{

	[Serializable]
	public class PacketsLostException : Exception
	{
		public PacketsLostException() { }
		public PacketsLostException(string message) : base(message) { }
		public PacketsLostException(string message, Exception inner) : base(message, inner) { }
		protected PacketsLostException(
		  System.Runtime.Serialization.SerializationInfo info,
		  System.Runtime.Serialization.StreamingContext context) : base(info, context) { }
	}
}
