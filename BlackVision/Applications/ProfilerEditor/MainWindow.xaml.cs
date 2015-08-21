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

		[MarshalAs( UnmanagedType.I4)]
		[FieldOffset( 28 )]
		public UInt32	depth;

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
				//this.Input.Text = message;
			}
		}

		private void endServer_Click( object sender, RoutedEventArgs e )
		{
			m_pipedServer.EndServer();
		}


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

				MakeTree( samples );
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

		// Tree maker
		private int GetTreeExpansionLevel()
		{
			int maxDepthLevel;
			bool success = Int32.TryParse( depthLevels.Text, out maxDepthLevel );
			if( !success )
				maxDepthLevel = 0;
			return maxDepthLevel;
		}

		private void MakeTree( ProfilerSample[] samples )
		{
			Stack<TreeViewItem> treeStack = new Stack<TreeViewItem>();

			int maxDepthLevel = GetTreeExpansionLevel();
			
			TreeViewItem currentParent = new TreeViewItem();
			currentParent.Header = "Thread1 Profiler data";
			currentParent.IsExpanded = true;

			int currentDepth = 0;
			TreeViewItem newItem = currentParent;

			ProfilerTree1.Items.Add( currentParent );

			foreach( var sample in samples )
			{
				if( sample.depth > currentDepth )
				{
					currentDepth = (int)sample.depth;
					treeStack.Push( currentParent );
					currentParent = newItem;
				}
				else if( sample.depth < currentDepth )
				{
					currentDepth = (int)sample.depth;
					currentParent = treeStack.Pop();
				}

				// Item already added, but reference needed outside this function.
				newItem = AddSampleToTree( sample, currentParent, maxDepthLevel );
			}
		}


		public TreeViewItem AddSampleToTree( ProfilerSample sample, TreeViewItem currentParent, int maxDepthLevel )
		{
			TreeViewItem newItem = new TreeViewItem();

			StackPanel stack = new StackPanel();
			stack.Orientation = Orientation.Horizontal;

			Label nameLabel = new Label();
			nameLabel.Content = sample.name;
			stack.Children.Add( nameLabel );

			Label durationLabel = new Label();
			durationLabel.Content = sample.durationSecs;
			stack.Children.Add( durationLabel );

			newItem.Header = stack;

			if( sample.depth <= maxDepthLevel )
				newItem.IsExpanded = true;

			currentParent.Items.Add( newItem );
			return newItem;
		}
    }
}
