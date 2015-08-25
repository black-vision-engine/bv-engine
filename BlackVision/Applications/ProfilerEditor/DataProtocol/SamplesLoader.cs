using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ProfilerEditor.DataProtocol
{
	class SamplesLoader
	{

		public static ProfilerSample[] Load( ReadDataObject data )
		{
			UInt32 offset = 0;
			return LoadSamples( data, offset );
		}

		public static LoadedData NewLoad( ReadDataObject data )
		{
			LoadedData loadedData = new LoadedData();


			return loadedData;
		}

		public static ProfilerSample[] LoadSamples( ReadDataObject data, UInt32 offset )
		{
			ProfilerSample[] samples;
			ProfilerSample sam = new ProfilerSample();		// This shit is only to obtain size of ProfilerSample;
			int sampleSize = Marshal.SizeOf( sam );
			long numStructs = data.m_bytesRead / sampleSize;

			samples = new ProfilerSample[ numStructs ];

			for( long i = 0; i < numStructs; ++i )
			{
				ProfilerSample sample = new ProfilerSample();
				samples[ i ] = (ProfilerSample)ByteArrayToStructure( data.m_data, sample, (int)i * sampleSize );
			}

			return samples;
		}

		private static object ByteArrayToStructure( byte[] bytearray, object structureObj, int position )
		{
			int length = Marshal.SizeOf( structureObj );
			IntPtr ptr = Marshal.AllocHGlobal( length );
			Marshal.Copy( bytearray, 0, ptr, length );
			structureObj = Marshal.PtrToStructure( Marshal.UnsafeAddrOfPinnedArrayElement( bytearray, position ), structureObj.GetType() );
			Marshal.FreeHGlobal( ptr );
			return structureObj;
		}
	}
}
