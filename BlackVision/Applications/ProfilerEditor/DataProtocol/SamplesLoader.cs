using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ProfilerEditor.DataProtocol
{
	/**@todo What will happen if there will be diffrent number encodings (big endian and little endian).*/
	class SamplesLoader
	{

		public static ProfilerSample[] Load( ReadDataObject data )
		{
			Int32 offset = 0;
			return LoadSamples( data, ref offset );
		}

		/**Future version of loading data.
		 @todo Use this function instead of Load and start sending compatibile data.*/
		public static LoadedData NewLoad( ReadDataObject data )
		{
			LoadedData loadedData = new LoadedData();
			Int32 offset = 0;

			ProtocolHeader header = LoadHeader( data, ref offset );
			loadedData.m_threadID = header.threadID;

			UInt16[] nameStringsLengths = LoadNameStringsLengths( data, header, ref offset );
			loadedData.m_nameIDs = LoadStringsIDs( data, header, ref offset );
			loadedData.m_nameStrings = LoadNameStrings( data, header, nameStringsLengths, ref offset );

			loadedData.m_samples = LoadSamples( data, ref offset );

			return loadedData;
		}

		private static ProfilerSample[] LoadSamples( ReadDataObject data, ref Int32 offset )
		{
			ProfilerSample[] samples;
			ProfilerSample sam = new ProfilerSample();		// This shit is created only to obtain size of ProfilerSample;
			int sampleSize = Marshal.SizeOf( sam );
			long numStructs = data.m_bytesRead / sampleSize;

			samples = new ProfilerSample[ numStructs ];

			for( long i = 0; i < numStructs; ++i )
			{
				ProfilerSample sample = new ProfilerSample();
				samples[ i ] = (ProfilerSample)ByteArrayToStructure( data.m_data, sample, (int)i * sampleSize );
			}

			offset += (int)numStructs * sampleSize;
			return samples;
		}

		private static ProtocolHeader LoadHeader( ReadDataObject data, ref Int32 offset )
		{
			ProtocolHeader header = new ProtocolHeader();
			ByteArrayToStructure( data.m_data, header, 0 );
			
			offset += Marshal.SizeOf( header );
			return header;
		}

		private static UInt16[] LoadNameStringsLengths( ReadDataObject data, ProtocolHeader header, ref Int32 offset )
		{
			UInt16[] nameStringsLengths = LoadUInt16Array( data, header.numNameStrings, ref offset );
			return nameStringsLengths;
		}

		private static UInt16[] LoadStringsIDs( ReadDataObject data, ProtocolHeader header, ref Int32 offset )
		{
			UInt16[] stringIDs = LoadUInt16Array( data, header.numNameStrings, ref offset );
			return stringIDs;
		}

		private static UInt16[] LoadUInt16Array( ReadDataObject data, Int32 numElements, ref Int32 offset )
		{
			UInt16[] UInt16Array = new UInt16[ numElements ];

			for( uint i = 0; i < numElements; ++i )
			{
				UInt16Array[ i ] = BitConverter.ToUInt16( data.m_data, offset );
				offset += 2;
			}

			return UInt16Array;
		}

		private static string[] LoadNameStrings( ReadDataObject data, ProtocolHeader header, UInt16[] nameStringsLenghts, ref Int32 offset )
		{
			string[] nameStrings = new string[ header.numNameStrings ];

			for( uint i = 0; i < nameStringsLenghts.Length; ++i )
			{
				nameStrings[ i ] = System.Text.Encoding.ASCII.GetString( data.m_data, offset, nameStringsLenghts[ i ] );
				offset += nameStringsLenghts[ i ];
			}

			return nameStrings;
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
