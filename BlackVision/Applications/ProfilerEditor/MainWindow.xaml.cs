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

namespace ProfilerEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
		NamedPipeServer			m_pipedServer;

        public MainWindow()
        {
            InitializeComponent();
        }

		private void startButton_Click( object sender, RoutedEventArgs e )
		{
			string pipeName = "ProfilerPipeTest";
			m_pipedServer = new NamedPipeServer( pipeName, 0 );

			m_pipedServer.StartServer();
		}

		private void getMessage_Click( object sender, RoutedEventArgs e )
		{
			ReadDataObject data = m_pipedServer.ReadBytes();

			string message = System.Text.Encoding.Default.GetString( data.m_data );
			this.Input.Text = message;
		}

		private void endServer_Click( object sender, RoutedEventArgs e )
		{
			m_pipedServer.EndServer();
		}
    }
}
