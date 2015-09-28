using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;

namespace ProfilerEditor.DataProtocol
{
	[StructLayout( LayoutKind.Explicit, Size = 8, Pack = 1 )]
	public struct ProfilerSample
	{
		[MarshalAs( UnmanagedType.I2 )]
		[FieldOffset( 0 )]
		public Int16	name;


		[MarshalAs( UnmanagedType.I2 )]
		[FieldOffset( 2 )]
		public UInt16	depth;


		[MarshalAs( UnmanagedType.R4 )]
		[FieldOffset( 4 )]
		public float	durationSecs;
	};

}
