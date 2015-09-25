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


namespace ProfilerEditor
{






    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
		private NamedPipeServer								m_pipedServer;
		private ProfilerModel.ProfilerTreeViewModel			m_profilerTreeView;
		private ProfilerModel.NameMapping					m_namesMap;
		private DataAnalysis.AverageSamples					m_dataProcessor;

		private bool										m_firstTime;

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

			m_firstTime = true;
			m_namesMap = new ProfilerModel.NameMapping();
			m_dataProcessor = new DataAnalysis.AverageSamples();
        }

		private void startButton_Click( object sender, RoutedEventArgs e )
		{
			string pipeName = "ProfilerPipeTest";
			m_pipedServer = new NamedPipeServer( pipeName, 0 );

			m_pipedServer.onMessageSent = GetMessageFromPipe;
			m_pipedServer.m_syncContext = SynchronizationContext.Current;
			m_pipedServer.StartServer();
		}


		private void endServer_Click( object sender, RoutedEventArgs e )
		{
			m_pipedServer.EndServer();
		}


		private void Window_Closing( object sender, CancelEventArgs e )
		{
			if( m_pipedServer != null )
				m_pipedServer.EndServer();
		}


		private void GetMessageFromPipe( object state )
		{
			ReadDataObject data = m_pipedServer.ReadBytes();

			if( data.m_bytesRead > 0 )
			{
				ProfilerSample[] samples;
				DataProtocol.LoadedData loadedData;
				loadedData = DataProtocol.SamplesLoader.NewLoad( data );
				samples = loadedData.m_samples;

				m_namesMap.Update( samples );
				ProfilerModel.ProfilerTreeViewModel newTreeView = m_dataProcessor.AddNewData( loadedData );
				if( newTreeView != null )
					MakeTree( newTreeView );
			}
		}

		private int GetTreeExpansionLevel()
		{
			int maxDepthLevel;
			bool success = Int32.TryParse( depthLevels.Text, out maxDepthLevel );
			if( !success )
				maxDepthLevel = 0;
			return maxDepthLevel;
		}

		private void MakeTree( ProfilerModel.ProfilerTreeViewModel treeView )
		{
			int maxDepthLevel = GetTreeExpansionLevel();

			if( !m_firstTime )
			{
				m_profilerTreeView.Update( treeView, (uint)maxDepthLevel );
				m_firstTime = false;
			}
			else
			{
				m_profilerTreeView = treeView;
				ProfilerTree1.DataContext = m_profilerTreeView;
			}
		}

    }
}
