using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;




namespace ProfilerEditor.Tester
{

    enum TestingMode
    {
        Uninitialized,
        AllFiles,
        SingleFile,
        DebugFile
    }

    public class TestsManager : INotifyPropertyChanged
    {
        private ObservableCollection< TestFile >    m_testFiles;
        private TestFile                            m_selectedFile;
        private string                              m_testsPath;
        private TestError                           m_selectedError;

        private ObservableCollection< TestError >   m_errorList;

        TestingMode                                 m_testMode;
        ComparisionRules                            m_comparisionRules;


        public TestsManager()
        {
            TestMode = TestingMode.Uninitialized;

            SelectedFile = null;
            SelectedError = null;
            TestFiles = new ObservableCollection< TestFile >();
            ErrorList = new ObservableCollection< TestError >();

            m_comparisionRules = new ComparisionRules();
        }

        // ================================================= //

        public void     DebugCurrentFile()
        {
            TestMode = TestingMode.DebugFile;

            if( SelectedFile != null )
                ParseFile( SelectedFile );
        }

        public void     TestAllFiles()
        {
            TestMode = TestingMode.AllFiles;

            if( TestFiles.Count != 0 )
            {
                SelectedFile = TestFiles[ 0 ];
                ParseFile( SelectedFile );
            }
        }

        public void     TestSingleFile()
        {
            TestMode = TestingMode.SingleFile;

            if( SelectedFile != null )
                ParseFile( SelectedFile );
        }


        // ================================================= //

        public bool     StepToNextFile  ()
        {
            for( int i = 0; i < TestFiles.Count; ++i )
            {
                if( TestFiles[ i ] == SelectedFile )
                {
                    // No more files on list.
                    if( i + 1 >= TestFiles.Count )
                        return false;

                    // Choose next file as selected file.
                    SelectedFile = TestFiles[ i + 1 ];
                    ParseFile( SelectedFile );

                    return true;
                }
            }

            // Something very wrong ;)
            return false;
        }


        public string   MakeTestStep    ()
        {
            if( SelectedFile != null )
            {
                string msg = SelectedFile.SendingStep();
                if( msg == null )
                {
                    // No more messages in file. Choose new file or send null.
                    if( TestMode == TestingMode.SingleFile )
                    {
                        TestMode = TestingMode.Uninitialized;
                    }

                    bool fileTestResult = SelectedFile.NumErrors == 0 ? false : true;

                    AddError( "Test ended", "", "File [ " + SelectedFile.FileName + " ] Errors: [ " + SelectedFile.NumErrors + " ] Warnings: [ " + SelectedFile.NumWarnings + " ]", 0, fileTestResult, SelectedFile );
                    return null;
                }

                return msg;
            }
            else
                return null;
        }

        public bool     Timeout         ( int seconds )
        {

        }

        public void     ReceivedReponse( string response )
        {
            var errors = SelectedFile.ResponseStep( response, m_comparisionRules );
            if( errors != null )
            {
                foreach( var error in errors )
                {
                    ErrorList.Add( error );
                }
            }
        }

        public void     EngineDisconnected( string message )
        {
            AddError( "Engine Connection failed", "", message, 0, true );
        }

        public void     EngineConnectionFailure()
        {
            AddError( "Engine Disconnected", "", "", 0, true );
        }


        // ================================================= //

        public void AddError( string cmdName, string eventName, string message, UInt32 eventID, bool isError )
        {
            AddError( cmdName, eventName, message, eventID, isError, null );
        }

        public void AddError( string cmdName, string eventName, string message, UInt32 eventID, bool isError, TestFile testFile )
        {
            TestError error = new TestError();
            error.CommandName = cmdName;
            error.IsError = isError;
            error.EventID = eventID;
            error.EventName = eventName;
            error.Message = message;
            error.FileRef = testFile;

            ErrorList.Add( error );
        }

        public void ParseFile( TestFile testFile )
        {
            string content = ReadFile( Path.Combine( m_testsPath, testFile.FileName ) );
            testFile.ParseFile( content );
        }

        public void UpdateTestPath( string newPath )
        {
            TestFiles.Clear();
            m_testsPath = newPath;

            var files = Directory.EnumerateFiles( m_testsPath, "*.bvtest" );
            foreach( var file in files )
            {
                TestFile newFile = new TestFile();
                newFile.FileName = Path.GetFileName( file );

                TestFiles.Add( newFile );
            }

        }


        private string ReadFile( string filePath )
        {
            try
            {   // Open the text file using a stream reader.
                using( StreamReader sr = new StreamReader( filePath ) )
                {
                    // Read the stream to a string, and write the string to the console.
                    string content = sr.ReadToEnd();
                    return content;
                }
            }
            catch( Exception e )
            {
                Console.WriteLine( "The file could not be read:" );
                Console.WriteLine( e.Message );
            }
            return null;
        }


        #region Properties

        public ObservableCollection<TestFile> TestFiles
        {
            get
            {
                return m_testFiles;
            }

            set
            {
                m_testFiles = value;
            }
        }

        public TestFile SelectedFile
        {
            get
            {
                return m_selectedFile;
            }

            set
            {
                m_selectedFile = value;
                NotifyPropertyChanged( "SelectedFile" );
            }
        }

        public ObservableCollection<TestError> ErrorList
        {
            get
            {
                return m_errorList;
            }

            set
            {
                m_errorList = value;
            }
        }

        public TestError SelectedError
        {
            get
            {
                return m_selectedError;
            }

            set
            {
                m_selectedError = value;
                NotifyPropertyChanged( "SelectedError" );
            }
        }

        internal TestingMode TestMode
        {
            get
            {
                return m_testMode;
            }

            set
            {
                m_testMode = value;
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
