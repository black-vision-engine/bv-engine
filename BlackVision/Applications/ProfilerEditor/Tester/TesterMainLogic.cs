using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows.Media;
using System.Windows.Input;
using System.IO;
using BlackBurst.Backend.TCP;
using System.Windows.Threading;


namespace ProfilerEditor.Tester
{
    enum TestsState
    {
        Init,
        Testing,
        WaitForInput,       // Debug
        WaitForResponse,
    }


    public class TesterMainLogic : INotifyPropertyChanged
    {
        public AsyncTcpClient           m_network;       // Hack public

        TestsState                      m_state;

        bool                            m_isConnected;
        SolidColorBrush                 m_connectedBrush;

        DispatcherTimer                 m_timer;
        int                             m_secondsTimeout = 5;

        string                          m_ipAddress;
        string                          m_port;

        TestsManager                    m_testsManager;


        // Hack: We need to make message from parts.
        string  m_message;


        #region Commands

        public ICommand ConnectCommand { get; internal set; }
        public ICommand ChooseTestDirectory { get; internal set; }
        public ICommand DebugCurrentTest { get; internal set; }
        public ICommand RunSingleTest { get; internal set; }
        public ICommand RunAllTests { get; internal set; }

        public ICommand ContinueDebuggingCommand { get; internal set; }
        public ICommand DebugStepCommand { get; internal set; }
        public ICommand StopDebuggingCommand { get; internal set; }

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
            DebugCurrentTest = new RelayCommand( DebugTest, IsInitState );
            RunSingleTest = new RelayCommand( TestSingleFile, IsInitState );
            RunAllTests = new RelayCommand( TestAllFiles, IsInitState );

            DebugStepCommand = new RelayCommand( DebugStep, CanStep );
            ContinueDebuggingCommand = new RelayCommand( ContinueDebug, CanStep );


            m_timer = new DispatcherTimer();
            m_timer.Tick += ReceivingTimeout;
            m_timer.Interval = new TimeSpan( 0, 0, 0, m_secondsTimeout, 0 );
        }


        #region Connection

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
            m_testsManager.EngineConnectionFailure();
        }


        public void EngineDisconnected      ( int ClientId, int ErrorCode, String Message )
        {
            IsConnected = false;
            m_testsManager.EngineDisconnected( Message );
        }

        #endregion


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
            dialog.SelectedPath = System.AppDomain.CurrentDomain.BaseDirectory;

            System.Windows.Forms.DialogResult result = dialog.ShowDialog();
            if( result == System.Windows.Forms.DialogResult.OK )
            {
                m_testsManager.UpdateTestPath( dialog.SelectedPath );
            }
        }

        // ================================================= //

        private void DebugTest( object parameter )
        {
            m_state = TestsState.WaitForInput;
            m_testsManager.DebugCurrentFile();
        }

        private void TestSingleFile( object parameter )
        {
            m_state = TestsState.Testing;
            m_testsManager.TestSingleFile();

            QueryAndSendNextMessage();
        }

        private void TestAllFiles( object parameter )
        {
            m_state = TestsState.Testing;
            m_testsManager.TestAllFiles();

            QueryAndSendNextMessage();
        }

        // ================================================= //

        private void ContinueDebug( object parameter )
        {
            m_state = TestsState.Testing;
            m_testsManager.ContinueToBreakPoint();

            QueryAndSendNextMessage();
        }

        private void DebugStep( object parameter )
        {
            m_state = TestsState.Testing;

            QueryAndSendNextMessage();
        }

        private bool CanStep( object parameter )
        {
            if( m_state == TestsState.WaitForInput )
                return true;
            return false;
        }


        // ================================================= //

        private void ReceivingTimeout       ( object sender, EventArgs e )
        {
            if( m_testsManager.Timeout( m_secondsTimeout ) )
            {
                SetAfterReceiveState();
                QueryAndSendNextMessage();
            }
        }

        public void MsgReceived             ( string data, EventArgs e )
        {
            int startIdx = 0;
            int length = data.Length;
            bool msgEnd = false;

            // Message contains 0x2 and 0x3 signs at beginning and end. We have to remove them.
            if( data.First() == 0x2 )
            {
                startIdx = 1;
                length -= 1;
            }

            int endOfStream = data.IndexOf( (char)0x3 );
            if( endOfStream > 0 )
            {
                length = endOfStream - 1;
                msgEnd = true;
            }

            m_message += data.Substring( startIdx, length );

            if( msgEnd )
            {
                m_testsManager.ReceivedReponse( m_message );
                m_timer.Stop();

                SetAfterReceiveState();
                QueryAndSendNextMessage();

                m_message = "";

                if( endOfStream < data.Length - 3 )
                {
                    startIdx = data.IndexOf( (char)0x2, endOfStream );
                    MsgReceived( data.Substring( startIdx ), e );
                }
            }


        }



        private void QueryAndSendNextMessage()
        {
            // Invalidate commands canExecute function value.
            CommandManager.InvalidateRequerySuggested();

            if( m_state != TestsState.Testing )
                return;

            string eventToSend = m_testsManager.MakeTestStep();

            if( eventToSend != null )
            {
                m_state = TestsState.WaitForResponse;

                if( eventToSend != "{}" )
                {
                    m_network.Write( eventToSend );
                    m_timer.Start();
                }
            }
            else
            {
                if( m_testsManager.TestMode == TestingMode.AllFiles )
                {
                    // Here BV should be restarted or cleaned.


                    if( m_testsManager.StepToNextFile() )
                    {
                        eventToSend = m_testsManager.MakeTestStep();
                        m_network.Write( eventToSend );
                        m_timer.Start();
                    }
                    else
                        m_state = TestsState.Init;
                }
                else
                {
                    m_state = TestsState.Init;
                }
            }
        }

        private void    SetAfterReceiveState()
        {
            if( m_testsManager.TestMode == TestingMode.DebugFile && m_testsManager.DebugBreak() )
                m_state = TestsState.WaitForInput;
            else
                m_state = TestsState.Testing;
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
