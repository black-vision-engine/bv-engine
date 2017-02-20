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
using System.Windows.Threading;
using System.Diagnostics;
using System.Globalization;


using BlackBurst.Backend.TCP;


namespace ProfilerEditor.GUI
{
    /// <summary>
    /// Interaction logic for TesterControl.xaml
    /// </summary>
    public partial class TesterControl : UserControl
    {
        ProfilerEditor.Tester.TesterMainLogic               m_testerLogic;

        public TesterControl()
        {
            InitializeComponent();

            if( Debugger.IsAttached )
                CultureInfo.DefaultThreadCurrentUICulture = CultureInfo.GetCultureInfo( "en-US" );

            m_testerLogic = new Tester.TesterMainLogic();
            DataContext = m_testerLogic;



            m_testerLogic.m_network.Connected += EngineConnected;
            m_testerLogic.m_network.ExceptionRaised += EngineDisconnected;
            m_testerLogic.m_network.ConnectionFailed += EngineConnectionFailed;
            m_testerLogic.m_network.Received += MsgReceived;
        }



        void EngineConnected        ( int ClientId, EventArgs e )
        {
            Application.Current.Dispatcher.BeginInvoke( DispatcherPriority.Background, new Action( () =>
            {
                m_testerLogic.EngineConnected( ClientId, e );
            } ) );
        }

        void EngineConnectionFailed ( int ClientId, EventArgs e )
        {
            Application.Current.Dispatcher.BeginInvoke( DispatcherPriority.Background, new Action( () =>
            {
                m_testerLogic.EngineConnectionFailed( ClientId, e );
            } ) );
        }


        void EngineDisconnected     ( int ClientId, int ErrorCode, String Message )
        {
            Application.Current.Dispatcher.BeginInvoke( DispatcherPriority.Background, new Action( () =>
            {
                m_testerLogic.EngineDisconnected( ClientId, ErrorCode, Message );
            } ) );
        }

        void MsgReceived            ( String data, EventArgs e )
        {
            Application.Current.Dispatcher.BeginInvoke( DispatcherPriority.Background, new Action( () =>
            {
                m_testerLogic.MsgReceived( data, e );
            } ) );
        }

        public void UpdateBVExecPath( string newPath )
        {
            m_testerLogic.UpdateBVExecPath( newPath );
        }
    }
}
