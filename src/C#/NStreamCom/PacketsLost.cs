using System;

namespace NStreamCom
{

	[Serializable]
	public class PacketsLost : Exception
	{
		public PacketsLost() { }
		public PacketsLost(string message) : base(message) { }
		public PacketsLost(string message, Exception inner) : base(message, inner) { }
		protected PacketsLost(
		  System.Runtime.Serialization.SerializationInfo info,
		  System.Runtime.Serialization.StreamingContext context) : base(info, context) { }
	}
}
