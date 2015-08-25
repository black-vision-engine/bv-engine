using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ProfilerEditor.DataProtocol
{

	[StructLayout( LayoutKind.Explicit, Size = 6, Pack = 1 )]
	struct ProtocolHeader
	{
		[MarshalAs( UnmanagedType.I2 )]
		[FieldOffset( 0 )]
		public Int16	threadID;

		[MarshalAs( UnmanagedType.I2 )]
		[FieldOffset( 2 )]
		public UInt16	numSamples;

		[MarshalAs( UnmanagedType.I2 )]
		[FieldOffset( 4 )]
		public UInt16	numNameStrings;
	}
}
