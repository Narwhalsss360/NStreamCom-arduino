using System;

namespace NStreamCom
{

	[Serializable]
	public class SizeMismatch : Exception
	{
		public SizeMismatch() { }
		public SizeMismatch(string message) : base(message) { }
		public SizeMismatch(string message, Exception inner) : base(message, inner) { }
		protected SizeMismatch(
		  System.Runtime.Serialization.SerializationInfo info,
		  System.Runtime.Serialization.StreamingContext context) : base(info, context) { }
	}
}
