using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ProfilerEditor.DataProtocol
{

	[StructLayout( LayoutKind.Explicit, Size = 8, Pack = 1 )]
	public struct ProtocolHeader
	{
		[MarshalAs( UnmanagedType.I2 )]
		[FieldOffset( 0 )]
		public UInt16	threadID;

		[MarshalAs( UnmanagedType.I2 )]
		[FieldOffset( 2 )]
		public UInt16	numSamples;

		[MarshalAs( UnmanagedType.I2 )]
		[FieldOffset( 4 )]
		public UInt16	numNameStrings;
	}
}
