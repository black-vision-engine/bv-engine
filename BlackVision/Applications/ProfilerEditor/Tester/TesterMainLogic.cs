using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Windows.Media;
using System.Windows.Input;
using BlackBurst.Backend.TCP;


namespace ProfilerEditor.Tester
{
    enum TestsState
    {
        Init,
        TestingAll,
        DebugTest

    }


    public class TesterMainLogic : INotifyPropertyChanged
    {
        public AsyncTcpClient           m_network;       // Hack public

        TestsState                      m_state;

        bool                            m_isConnected;
        SolidColorBrush                 m_connectedBrush;

        string      m_ipAddress;
        string      m_port;

        TestsManager                    m_testsManager;


        #region Commands

        public ICommand ConnectCommand { get; internal set; }
        public ICommand ChooseTestDirectory { get; internal set; }

        #endregion



        public TesterMainLogic()
        {
            m_network = new AsyncTcpClient();
            TestsManager = new TestsManager();

            m_ipAddress = "127.0.0.1";
            m_port = "11101";

            m_state = TestsState.Init;
            IsConnected = false;
            ConnectedBrush = Brushes.Red;

            ConnectCommand = new RelayCommand( ConnectClick );
            ChooseTestDirectory = new RelayCommand( ChooseTestDir, IsInitState );
        }


        private void ConnectClick           ( object parameter )
        {
            if( !IsConnected )
                m_network.Connect( m_ipAddress, Int32.Parse( m_port ) );
        }


        public void EngineConnected         ( int ClientId, EventArgs e )
        {
            IsConnected = true;

            // Disable logger
            byte[] initMessage = new byte[ 8 ];
            for( int i = 0; i < 8; ++i )
            {
                initMessage[ i ] = 0;
            }

            m_network.WritePlain( initMessage.ToString() );

        }

        public void EngineConnectionFailed  ( int ClientId, EventArgs e )
        {
            IsConnected = false;
        }


        public void EngineDisconnected      ( int ClientId, int ErrorCode, String Message )
        {
            IsConnected = false;
        }

        public void MsgReceived             ( String data, EventArgs e )
        {
        }

        private bool IsInitState( object parameter )
        {
            if( m_state == TestsState.Init )
                return true;
            else
                return false;
        }

        private void ChooseTestDir( object parameter )
        {
            var dialog = new System.Windows.Forms.FolderBrowserDialog();
            System.Windows.Forms.DialogResult result = dialog.ShowDialog();

            if( result == System.Windows.Forms.DialogResult.OK )
            {
                m_testsManager.UpdateTestPath( dialog.SelectedPath );
            }
        }


        #region Properties

        public bool IsConnected
        {
            get
            {
                return m_isConnected;
            }

            set
            {
                m_isConnected = value;
                NotifyPropertyChanged( "IsConnected" );

                if (m_isConnected)
                    ConnectedBrush = Brushes.Green;
                else
                    ConnectedBrush = Brushes.Red;
            }
        }

        public SolidColorBrush ConnectedBrush
        {
            get
            {
                return m_connectedBrush;
            }

            internal set
            {
                m_connectedBrush = value;
                NotifyPropertyChanged( "ConnectedBrush" );
            }
        }

        public string IpAddress
        {
            get
            {
                return m_ipAddress;
            }

            set
            {
                m_ipAddress = value;
            }
        }

        public string Port
        {
            get
            {
                return m_port;
            }

            set
            {
                m_port = value;
            }
        }


        public TestsManager TestsManager
        {
            get
            {
                return m_testsManager;
            }

            set
            {
                m_testsManager = value;
            }
        }

        #endregion

        #region PropertyChangedImpl

        public event PropertyChangedEventHandler PropertyChanged;


        private void NotifyPropertyChanged( String info )
        {
            if( PropertyChanged != null )
            {
                PropertyChanged( this, new PropertyChangedEventArgs( info ) );
            }
        }

        #endregion
    }
}
