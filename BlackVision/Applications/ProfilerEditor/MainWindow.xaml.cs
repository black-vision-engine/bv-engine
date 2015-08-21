using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;
using System.Threading;
using System.Runtime.InteropServices;

namespace ProfilerEditor
{

	[StructLayout(LayoutKind.Explicit, Size = 32, Pack = 1)]
	struct ProfilerSample
	{
		[FieldOffset( 0 )]
		Int64	name;
		[FieldOffset( 8 )]
		Int64	duration;
		[FieldOffset( 16 )]
		double	durationSecs;
		[FieldOffset( 20 )]
		Int32	type;
		[FieldOffset( 24 )]
		UInt32	depth;

	};





    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
		private NamedPipeServer			m_pipedServer;

        public MainWindow()
        {
            InitializeComponent();
        }

		private void startButton_Click( object sender, RoutedEventArgs e )
		{
			string pipeName = "ProfilerPipeTest";
			m_pipedServer = new NamedPipeServer( pipeName, 0 );

			//m_pipedServer.onMessageSent += new EventHandler( GetMessageFromPipe );
			m_pipedServer.onMessageSent = GetMessageFromPipe;
			m_pipedServer.m_syncContext = SynchronizationContext.Current;
			m_pipedServer.StartServer();
			//m_pipedServer.m_syncObject = (ISynchronizeInvoke)this.ProfilerTree1;
		}

		private void getMessage_Click( object sender, RoutedEventArgs e )
		{
			ReadDataObject data = m_pipedServer.ReadBytes();
			if( data.m_bytesRead > 0 )
			{
				string message = System.Text.Encoding.Default.GetString( data.m_data );
				this.Input.Text = message;
			}
		}

		private void endServer_Click( object sender, RoutedEventArgs e )
		{
			m_pipedServer.EndServer();
		}

		//private void GetMessageFromPipe( object sender, EventArgs e )
		//{
		//	ReadDataObject data = m_pipedServer.ReadBytes();

			//if( data.m_bytesRead > 0 )
			//{
			//	string message = System.Text.Encoding.Default.GetString( data.m_data );
			//	this.Input.Text = message;
			//}
		//}

		//private void GetMessageFromPipe()
		//{
		//	ReadDataObject data = m_pipedServer.ReadBytes();

			//if( data.m_bytesRead > 0 )
			//{
			//	string message = System.Text.Encoding.Default.GetString( data.m_data );
			//	this.Input.Text = message;
			//}
		//}

		private void GetMessageFromPipe( object state )
		{
			ReadDataObject data = m_pipedServer.ReadBytes();

			if( data.m_bytesRead > 0 )
			{
				//string message = System.Text.Encoding.Default.GetString( data.m_data );
				//this.Input.Text = message;
				ProfilerSample[] samples;
				ProfilerSample sam = new ProfilerSample();		// This shit is only to obtain size of ProfilerSample;
				int sampleSize = Marshal.SizeOf( sam );
				long numStructs = data.m_bytesRead / sampleSize;

				samples = new ProfilerSample[numStructs];

				for( long i = 0; i < numStructs; ++i )
				{
					ProfilerSample sample = new ProfilerSample();
					samples[ i ] = (ProfilerSample)ByteArrayToStructure( data.m_data, sample, (int)i * sampleSize );
				}

			}
		}

		object ByteArrayToStructure( byte[] bytearray, object structureObj, int position )
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
