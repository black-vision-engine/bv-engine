﻿using System;
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
		Process												m_BlackVisionProcess;

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

			m_BlackVisionProcess = null;
			// Default names
			m_pipeName = "BlackVisionProfiler";
			m_BlackVisionPathName = "C:\\Users\\WitekD\\BV\\BlackVision\\_Builds\\x64-v110-Debug\\Applications\\BlackVision\\BlackVision.exe";

			m_pipedServer = null;
			m_namesMap = new ProfilerModel.NameMapping();

			m_firstTime = new bool[ m_numThreads ];
			for( int i = 0; i < m_firstTime.Length; ++i )
				m_firstTime[ i ] = true;

			m_dataProcessor = new DataAnalysis.AverageSamples[ m_numThreads ];
			for( int i = 0; i < m_numThreads; ++i )
				m_dataProcessor[ i ] = new DataAnalysis.AverageSamples();

			m_profilerTreeView = new ProfilerModel.ProfilerTreeViewModel[ m_numThreads ];
        }

		private void startButton_Click( object sender, RoutedEventArgs e )
		{
			m_pipedServer = new DataProtocol.NamedPipeServer( m_pipeName, 0, m_numThreads );

			m_pipedServer.onMessageSent = GetMessageFromPipe;
			m_pipedServer.m_syncContext = SynchronizationContext.Current;
			m_pipedServer.StartServer();

			m_BlackVisionProcess = new Process();
			m_BlackVisionProcess.StartInfo.FileName = m_BlackVisionPathName;
			m_BlackVisionProcess.StartInfo.Arguments = "";
			m_BlackVisionProcess.StartInfo.WorkingDirectory = System.IO.Path.GetDirectoryName( m_BlackVisionPathName );
			m_BlackVisionProcess.Start();
		}


		private void endServer_Click( object sender, RoutedEventArgs e )
		{
			if( m_pipedServer == null )
				return;
			m_pipedServer.EndServer();
			m_pipedServer = null;

			if( !m_BlackVisionProcess.HasExited )
			{
				m_BlackVisionProcess.CloseMainWindow();
				m_BlackVisionProcess.WaitForExit();
			}
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

		private int GetTreeExpansionLevel()
		{
			int maxDepthLevel;
			bool success = Int32.TryParse( depthLevels.Text, out maxDepthLevel );
			if( !success )
				maxDepthLevel = 0;
			return maxDepthLevel;
		}

		private void MakeTree( ProfilerModel.ProfilerTreeViewModel treeView, uint thread )
		{
			int maxDepthLevel = GetTreeExpansionLevel();

			if( !m_firstTime[ thread ] )
				m_profilerTreeView[ thread ].Update( treeView, (uint)maxDepthLevel );
			else
			{
				m_firstTime[ thread ] = false;
				m_profilerTreeView[ thread ] = treeView;
				SetTreeDataContext( thread, treeView );
			}
		}

		private void SetTreeDataContext( uint thread, ProfilerModel.ProfilerTreeViewModel treeView )
		{
			switch( thread )
			{
				case 0:
					ProfilerTree1.DataContext = treeView;
					break;
				case 1:
					ProfilerTree2.DataContext = treeView;
					break;
				case 2:
					ProfilerTree3.DataContext = treeView;
					break;
				case 3:
					ProfilerTree4.DataContext = treeView;
					break;
				case 4:
					ProfilerTree5.DataContext = treeView;
					break;
				case 5:
					ProfilerTree6.DataContext = treeView;
					break;
			}
		}

		private void ProfilingTool_Click( object sender, RoutedEventArgs e )
		{
			ProfilingToolDialog dialog = new ProfilingToolDialog();
			dialog.BlackVisionPathTextBox.Text = m_BlackVisionPathName;
			dialog.NamedPipeTextBox.Text = m_pipeName;

			dialog.ShowDialog();
			m_BlackVisionPathName = dialog.BlackVisionPathTextBox.Text;
			m_pipeName = dialog.NamedPipeTextBox.Text;
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
				m_profilerTreeView[ thread ].ClearTotalTime();
			}
		}

		private void ClearTreeButton_Click( object sender, RoutedEventArgs e )
		{
			if( AffectAllCheckBox.IsChecked == true )
			{
				for( int i = 0; i < m_profilerTreeView.Length; ++i )
				{
					m_profilerTreeView[ i ] = null;
					m_firstTime[ i ] = true;
					SetTreeDataContext( (uint)i, null );
				}
			}
			else
			{
				int thread = ThreadsTabControl.SelectedIndex;
				m_profilerTreeView[ thread ] = null;
				m_firstTime[ thread ] = true;
				SetTreeDataContext( (uint)thread, null );
			}
		}

    }
}
