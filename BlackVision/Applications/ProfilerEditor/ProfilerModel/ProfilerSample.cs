﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;

namespace ProfilerEditor
{
	[StructLayout( LayoutKind.Explicit, Size = 32, Pack = 1 )]
	public struct ProfilerSample
	{
		[MarshalAs( UnmanagedType.I8 )]
		[FieldOffset( 0 )]
		public Int64	name;

		[MarshalAs( UnmanagedType.I8 )]
		[FieldOffset( 8 )]
		public Int64	duration;

		[MarshalAs( UnmanagedType.R8 )]
		[FieldOffset( 16 )]
		public double	durationSecs;

		[MarshalAs( UnmanagedType.I4 )]
		[FieldOffset( 24 )]
		public Int32	type;

		[MarshalAs( UnmanagedType.I4 )]
		[FieldOffset( 28 )]
		public UInt32	depth;

	};

}
