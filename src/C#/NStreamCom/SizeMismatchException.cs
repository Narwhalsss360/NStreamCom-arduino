using System;

namespace NStreamCom
{

	[Serializable]
	public class SizeMismatchException : Exception
	{
		public SizeMismatchException() { }
		public SizeMismatchException(string message) : base(message) { }
		public SizeMismatchException(string message, Exception inner) : base(message, inner) { }
		protected SizeMismatchException(
		  System.Runtime.Serialization.SerializationInfo info,
		  System.Runtime.Serialization.StreamingContext context) : base(info, context) { }
	}
}
