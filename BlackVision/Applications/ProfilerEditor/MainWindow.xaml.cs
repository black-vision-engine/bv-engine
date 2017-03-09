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
using ProfilerEditor.PresentationLayer;
using System.Diagnostics;
using System.IO;
using System.Net.Sockets;

using Tools;

namespace ProfilerEditor
{

    public partial class MainWindow : Window
    {
		private DataProtocol.NamedPipeServer				m_pipedServer;
		private ProfilerModel.ProfilerTreeViewModel[]		m_profilerTreeView;
		private ProfilerModel.NameMapping					m_namesMap;
		private DataAnalysis.AverageSamples[]				m_dataProcessor;

		private bool[]										m_firstTime;
		private uint										m_numThreads = 6;

		string												m_pipeName;
		string												m_BlackVisionPathName;
		BlackVisionProcess									m_BlackVisionProcess;
		string												m_commandLineArg;

		public int											m_timeFormatUnits;
		public string										m_timeFormatString;

        // TCP client
        private NetworkStream                               m_networkStream;
        private TcpClient                                   m_tcpClient;
        bool                                                m_connected;
        const int                                           tcpReadBufferSize = 1000;


#region Properties
		public ProfilerModel.NameMapping ColorMapping
		{
			get { return m_namesMap; }
		}

		public ProfilerModel.NameMapping NameMapping
		{
			get { return m_namesMap; }
		}

#endregion



		public MainWindow()
        {
            InitializeComponent();

			// BlackVision process handle and default names
			m_BlackVisionProcess = new BlackVisionProcess();
			m_pipeName = "BlackVisionProfiler";
			m_BlackVisionPathName = "..\\BlackVision\\BlackVision.exe";
			m_BlackVisionPathName = System.IO.Path.GetFullPath( m_BlackVisionPathName );
			m_commandLineArg = "-EnableProfiler";

			// Default formatting
			m_timeFormatUnits = 0;
			TimeUnitsComboBox.SelectedIndex = m_timeFormatUnits;
			m_timeFormatString = "0.000000 s";
			TimeDigitsTextBox.Text = "6";

			// Pipe server
			m_pipedServer = null;
			m_namesMap = new ProfilerModel.NameMapping();

			// treeViews and data processor
			m_firstTime = new bool[ m_numThreads ];
			for( int i = 0; i < m_firstTime.Length; ++i )
				m_firstTime[ i ] = true;

			m_dataProcessor = new DataAnalysis.AverageSamples[ m_numThreads ];
			for( int i = 0; i < m_numThreads; ++i )
				m_dataProcessor[ i ] = new DataAnalysis.AverageSamples();

			m_profilerTreeView = new ProfilerModel.ProfilerTreeViewModel[ m_numThreads ];

            // TCP
            m_connected = false;
            m_tcpClient = null;
            m_networkStream = null;
        }

		private void startButton_Click( object sender, RoutedEventArgs e )
		{
			if( m_pipedServer != null )
				endServer_Click( null, null );

			m_pipedServer = new DataProtocol.NamedPipeServer( m_pipeName, 0, m_numThreads );

			m_pipedServer.onMessageSent = GetMessageFromPipe;
			m_pipedServer.m_syncContext = SynchronizationContext.Current;
			m_pipedServer.StartServer();
			Thread.Sleep( 20 );		//Let's give named pipe server time, to start work.

			StartBlackVisionProcess();
		}


		private void endServer_Click( object sender, RoutedEventArgs e )
		{
            m_BlackVisionProcess.Kill();

			if( m_pipedServer == null )
				return;
			m_pipedServer.EndServer();
			m_pipedServer = null;
		}


		private void Window_Closing( object sender, CancelEventArgs e )
		{
			endServer_Click( null, null );
		}


		private void GetMessageFromPipe( object state )
		{
			if( m_pipedServer == null )
				return;	// It happens, when connection ends, but there are still messages form piped server in windows events queue.
			DataProtocol.ReadDataObject data = m_pipedServer.ReadBytes();

			if( data.m_bytesRead > 0 )
			{
				DataProtocol.LoadedData loadedData;
				loadedData = DataProtocol.SamplesLoader.NewLoad( data );

				m_namesMap.Update( loadedData );
				ProfilerModel.ProfilerTreeViewModel newTreeView = m_dataProcessor[ loadedData.m_threadID ].AddNewData( loadedData );
				if( newTreeView != null )
					MakeTree( newTreeView, loadedData.m_threadID );
			}
		}

		private void MakeTree( ProfilerModel.ProfilerTreeViewModel treeView, uint thread )
		{
			if( !m_firstTime[ thread ] )
			{
				m_profilerTreeView[ thread ].Update( treeView );
			}
			else
			{
				m_firstTime[ thread ] = false;
				m_profilerTreeView[ thread ] = treeView;
				SetTreeDataContext( thread, treeView );

                var tree = GetThreadTreeView(thread);
                tree.ExpandTree();
			}
		}

		private void SetTreeDataContext( uint thread, ProfilerModel.ProfilerTreeViewModel treeView )
		{
			TreeListView treeListView = GetThreadTreeView( thread );
			treeListView.DataContext = treeView;
		}

		private TreeListView GetThreadTreeView( uint thread )
		{
			switch( thread )
			{
				case 0:
					return ProfilerTree1;
				case 1:
					return ProfilerTree2;
				case 2:
					return ProfilerTree3;
				case 3:
					return ProfilerTree4;
				case 4:
					return ProfilerTree5;
				case 5:
					return ProfilerTree6;
			}
			return null;
		}

		private void ProfilingTool_Click( object sender, RoutedEventArgs e )
		{
			ProfilingToolDialog dialog = new ProfilingToolDialog();
			dialog.BlackVisionPathTextBox.Text = m_BlackVisionPathName;
			dialog.NamedPipeTextBox.Text = m_pipeName;

			dialog.ShowDialog();
			m_BlackVisionPathName = dialog.BlackVisionPathTextBox.Text;
			m_pipeName = dialog.NamedPipeTextBox.Text;

            TesterControl.UpdateBVExecPath( m_BlackVisionPathName );
            m_BlackVisionProcess.BlackVisionPathName = m_BlackVisionPathName;

        }

		private void ClearMaxTimeButton_Click( object sender, RoutedEventArgs e )
		{
			if( AffectAllCheckBox.IsChecked == true )
			{
				foreach( var treeViewModel in m_profilerTreeView )
					if( treeViewModel != null )
						treeViewModel.ClearMaxTime();
			}
			else
			{
				int thread = ThreadsTabControl.SelectedIndex;
				if( m_profilerTreeView[ thread ] != null )
					m_profilerTreeView[ thread ].ClearMaxTime();
			}
		}

		private void ClearMinTimeButton_Click( object sender, RoutedEventArgs e )
		{
			if( AffectAllCheckBox.IsChecked == true )
			{
				foreach( var treeViewModel in m_profilerTreeView )
					if( treeViewModel != null )
						treeViewModel.ClearMinTime();
			}
			else
			{
				int thread = ThreadsTabControl.SelectedIndex;
				if( m_profilerTreeView[ thread ] != null )
					m_profilerTreeView[ thread ].ClearMinTime();
			}
		}

		private void ClearTotalTimeButton_Click( object sender, RoutedEventArgs e )
		{
			if( AffectAllCheckBox.IsChecked == true )
			{
				foreach( var treeViewModel in m_profilerTreeView )
					if( treeViewModel != null )
						treeViewModel.ClearTotalTime();
			}
			else
			{
				int thread = ThreadsTabControl.SelectedIndex;
				if( m_profilerTreeView[ thread ] != null )
					m_profilerTreeView[ thread ].ClearTotalTime();
			}
		}


        private void ClearTree( bool affectAll )
        {
            if( affectAll )
            {
                for( int i = 0; i < m_profilerTreeView.Length; ++i )
                {
                    m_dataProcessor[i] = new DataAnalysis.AverageSamples();
                    m_profilerTreeView[i] = null;
                    m_firstTime[i] = true;
                    SetTreeDataContext( (uint)i, null );
                }
            }
            else
            {
                int thread = ThreadsTabControl.SelectedIndex;
                m_profilerTreeView[thread] = null;
                m_dataProcessor[thread] = new DataAnalysis.AverageSamples();
                m_firstTime[thread] = true;
                SetTreeDataContext( (uint)thread, null );
            }
        }

		private void ClearTreeButton_Click( object sender, RoutedEventArgs e )
		{
            bool affectAll = AffectAllCheckBox.IsChecked.Value;
            ClearTree( affectAll );
		}


		private void UpdateTimeFormatButton_Click( object sender, RoutedEventArgs e )
		{
			int decimalDigits;
			bool success = Int32.TryParse( TimeDigitsTextBox.Text, out decimalDigits );
			if( !success )
				return;

			string units;
			string format = "0.";

			m_timeFormatUnits = TimeUnitsComboBox.SelectedIndex;
			if( m_timeFormatUnits == 0 )
				units = " s";
			else
				units = " ms";

			for( int i = 0; i < decimalDigits; ++i )
				format += "0";
			format += units;
			m_timeFormatString = format;

			foreach( var tree in m_profilerTreeView )
				if( tree != null )
					tree.RefreshTree();
		}

		private void ExpandButton_Click( object sender, RoutedEventArgs e )
		{
			if( AffectAllCheckBox.IsChecked == true )
			{
				for( int i = 0; i < m_profilerTreeView.Length; ++i )
				{
					TreeListView treeListView = GetThreadTreeView( (uint)i );
					treeListView.ExpandTree();
				}
			}
			else
			{
				int thread = ThreadsTabControl.SelectedIndex;
				TreeListView treeListView = GetThreadTreeView( (uint)thread );
				treeListView.ExpandTree();
			}
		}

		private void StartBlackVisionProcess()
		{
			if( !File.Exists( m_BlackVisionPathName ) )
				return;

            m_BlackVisionProcess.Start( m_commandLineArg );

            ClearTree( true );
		}


        private uint GetSeverityLevel()
        {
            return (uint)SeverityLevelsCombobox.SelectedIndex;
        }

        private uint GetModuleFilter()
        {
            uint filter = 0;
            foreach( var item in ModulesListView.SelectedItems )
            {
                var listView = item as ListViewItem;
                var tag = Convert.ToUInt32( listView.Tag.ToString() );
                filter = filter | tag;
            }

            return filter;
        }

        private void OnConnect()
        {
            SeverityLevelsCombobox.IsEnabled = false;
            ModulesListView.IsEnabled = false;

            NetStatusLabel.Content = "Connected";
            m_connected = true;
        }

        private void OnDisconnect()
        {
            SeverityLevelsCombobox.IsEnabled = true;
            ModulesListView.IsEnabled = true;

            NetStatusLabel.Content = "Disconnected";
            m_connected = false;
        }

        private void ConnectButton_Click( object sender, RoutedEventArgs e )
        {
            try
            {
                Int32 port = 11011;
                string addressIP = "127.0.0.1";
                m_tcpClient = new TcpClient(addressIP, port);

                m_networkStream = m_tcpClient.GetStream();

                byte[] initMessage = new byte[ 8 ];

                uint severityLevel = GetSeverityLevel();
                uint modulesFilter = GetModuleFilter();
                byte[] severity = BitConverter.GetBytes( severityLevel );
                byte[] modules = BitConverter.GetBytes( modulesFilter );

                if( BitConverter.IsLittleEndian )
                {
                    Array.Reverse( severity );
                    Array.Reverse( modules );
                }

                severity.CopyTo( initMessage, 0 );
                modules.CopyTo( initMessage, 4 );


                m_networkStream.Write( initMessage, 0, initMessage.Length );
                m_networkStream.Flush();

                OnConnect();
            }
            catch ( SocketException except )
            {
                NetStatusLabel.Content = "Socket exception " + except.ToString();
                OnDisconnect();
            }
        }

        private void DisconnetcButton_Click( object sender, RoutedEventArgs e )
        {
            if( m_connected )
            {
                m_networkStream.Close();
                m_tcpClient.Close();

                OnDisconnect();
            }
        }

        private void SendButton_Click( object sender, RoutedEventArgs e )
        {
            string prefix = "{\n\"Events\" : \n[";
            string postfix = "]\n}";

            if( m_connected )
            {
                byte[] command = Encoding.UTF8.GetBytes( prefix + CommandTextBox.Text + postfix );

                byte[] message = new byte[command.Length + 2];

                message[0] = 0x002;                         // Start transmission sign
                System.Buffer.BlockCopy( command, 0, message, 1, command.Length );
                message[command.Length + 1] = 0x003;        // End transmission sign

                try
                {
                    m_networkStream.Write(message, 0, command.Length + 2);
                }
                catch (Exception exc) 
                {
                    // Cannot send message.
                    var error = exc.ToString();
                    MessageBox.Show("Cannot send message. Error: \n" + error);
                }
            }
        }

        private void ReceiveButton_Click( object sender, RoutedEventArgs e )
        {
            if( m_connected && m_networkStream.DataAvailable )
            {
                byte[] message = new byte[ tcpReadBufferSize ];
                int numBytesRead = 0;

                do
                {
                    numBytesRead = m_networkStream.Read( message, 0, tcpReadBufferSize );
                    string stringMessage = System.Text.Encoding.UTF8.GetString( message, 0, numBytesRead );

                    ResponseTextBox.Text += stringMessage;
                } while( m_networkStream.DataAvailable );
                

            }
        }

        private void ClearResponsesButton_Click( object sender, RoutedEventArgs e )
        {
            ResponseTextBox.Text = "";
        }

        private void ClearEventsButton_Click( object sender, RoutedEventArgs e )
        {
            CommandTextBox.Text = "";
        }
    }
}
