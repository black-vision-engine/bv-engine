using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;
using System.Xml;



namespace RegressionLib
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
        private TestError                           m_selectedError;

        private string                              m_testsPath;
        private string                              m_outputPath;

        private ObservableCollection< TestError >   m_errorList;

        private EventsViewModel                     m_eventsView;
        int                                         m_tabItemIdx;

        TestingMode                                 m_testMode;
        ComparisionRules                            m_comparisionRules;

        int             m_progressStepsMade;
        int             m_numProgressSteps;

        bool            m_break;


        public TestsManager()
        {
            TestMode = TestingMode.Uninitialized;

            m_eventsView = new EventsViewModel();
            SelectedFile = null;
            SelectedError = null;

            TestFiles = new ObservableCollection< TestFile >();
            ErrorList = new ObservableCollection< TestError >();

            m_comparisionRules = new ComparisionRules();
            m_break = false;
            m_outputPath = "";
        }

        // ================================================= //

        public void     DebugCurrentFile()
        {
            TestMode = TestingMode.DebugFile;
            m_break = true;

            if( SelectedFile != null )
            {
                ParseFile( SelectedFile );
                InitProgress();
            }
        }

        public void     TestAllFiles()
        {
            TestMode = TestingMode.AllFiles;

            if( TestFiles.Count != 0 )
            {
                SelectedFile = TestFiles[ 0 ];
                ParseFile( SelectedFile );
                InitProgress();
            }
        }

        public void     TestSingleFile()
        {
            TestMode = TestingMode.SingleFile;

            if( SelectedFile != null )
            {
                ParseFile( SelectedFile );
                InitProgress();
            }
        }

        public void     ContinueToBreakPoint()
        {
            m_break = false;
        }

        public bool     DebugBreak()
        {
            return m_break;
        }

        public bool     IsEnded ()
        {
            if( SelectedFile == null &&
                ChooseNextFile() == null )
                return true;
            return false;
        }

        // ================================================= //

        public bool     StepToNextFile  ()
        {
            // Try to select next file.
            var nextFile = ChooseNextFile();
            if( nextFile == null )
            {
                // Write result of previous test to file.
                WriteResultToFile( m_outputPath );
                return false;
            }

            // Choose next file as selected file.
            SelectedFile = nextFile;
            ParseFile( SelectedFile );

            InitProgress();

            return true;
        }

        private TestFile        ChooseNextFile()
        {
            // Function doesn't change SelectedFile.
            for( int i = 0; i < TestFiles.Count; ++i )
            {
                if( TestFiles[ i ] == SelectedFile )
                {
                    // No more files on list.
                    if( i + 1 >= TestFiles.Count )
                        return null;
                    else
                        return TestFiles[ i + 1 ]; ;
                }
            }

            return null;
        }


        public string   MakeTestStep    ()
        {
            if( SelectedFile != null )
            {
                string nextMsg = NormalStep();

                if( nextMsg == null )
                {
                    ErrorRank fileTestResult = SelectedFile.NumErrors == 0 ? ErrorRank.ResultOk : ErrorRank.Error;
                    AddError( "Test ended", "", "File [ " + SelectedFile.FileName + " ] Errors: [ " + SelectedFile.NumErrors + " ] Warnings: [ " + SelectedFile.NumWarnings + " ]", 0, fileTestResult, SelectedFile );
                }

                Refresh();
                return nextMsg;
            }
            else
                return null;
        }


        private string  NormalStep      ()
        {
            string msg = SelectedFile.SendingStep();

            if( SelectedFile.IsBreakPoint() )
                m_break = true;

            return msg;
        }


        public bool     Timeout         ( int seconds )
        {
            TestError error = SelectedFile.Timeout( seconds );
            if( error == null )
                return false;

            ErrorList.Add( error );
            return true;
        }

        public void     ReceivedReponse( string response )
        {
            Progress = 1;   // In reality it's += 1. We add one step.

            var errors = SelectedFile.ResponseStep( response, m_comparisionRules );
            if( errors != null )
            {
                foreach( var error in errors )
                {
                    ErrorList.Add( error );
                }
            }

            Refresh();
        }

        public void     EngineDisconnected( string message )
        {
            AddError( "Engine Connection failed", "", message, 0, ErrorRank.Warning );
        }

        public void     EngineConnectionFailure()
        {
            AddError( "Engine Disconnected", "", "", 0, ErrorRank.Warning );
        }

        public void     EngineCrash()
        {
            SelectedFile.NumErrors++;

            AddError( "Engine crashed", "", "Engine crashed or became unresponsive", 0, ErrorRank.Error, SelectedFile );
            AddError( "Test ended", "", "File [ " + SelectedFile.FileName + " ] Errors: [ " + SelectedFile.NumErrors + " ] Warnings: [ " + SelectedFile.NumWarnings + " ]", 0, ErrorRank.Error, SelectedFile );
        }


        // ================================================= //

        public void     AddError( string cmdName, string eventName, string message, UInt32 eventID, ErrorRank isError )
        {
            AddError( cmdName, eventName, message, eventID, isError, null );
        }

        public void     AddError( string cmdName, string eventName, string message, UInt32 eventID, ErrorRank isError, TestFile testFile )
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

        public void     ParseFile( TestFile testFile )
        {
            string content = ReadFile( Path.Combine( m_testsPath, testFile.FileName ) );
            testFile.ParseFile( content );
        }

        public void     UpdateTestPath( string newPath )
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

        public void     UpdateOutputPath    ( string output )
        {
            m_outputPath = output;
        }


        private string      ReadFile( string filePath )
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

        private void    InitProgress    ()
        {
            m_numProgressSteps = SelectedFile.ReferenceResponses.Count;
            m_progressStepsMade = 0;
        }

        private void    Refresh         ()
        {
            if( m_tabItemIdx == 0 )
            {
                SelectedError = SelectedError;
            }
            else
            {
                if( SelectedFile != null )
                {
                    CurrentTestEvent = SelectedFile.CurrentTestEvent;
                    CurrentReferenceResponse = SelectedFile.CurrentResponse;
                    CurrentRealResponse = SelectedFile.CurrentRealResponse;
                }
            }
        }

        private void        WriteResultToFile( string outputPath )
        {
            string testSuiteName = Path.GetFileName( m_testsPath.TrimEnd( Path.DirectorySeparatorChar ) );
            string reportFile = Path.Combine( outputPath, testSuiteName + ".xml" );

            if( !Directory.Exists( outputPath ) )
                Directory.CreateDirectory( outputPath );


            using( FileStream outputStream = File.Open( reportFile, FileMode.Create ) )
            {
                XmlWriterSettings settings = new XmlWriterSettings();
                settings.Indent = true;
                settings.CloseOutput = true;


                using( XmlWriter writer = XmlWriter.Create( outputStream, settings ) )
                {
                    writer.WriteStartElement( "testsuites" );

                    writer.WriteAttributeString( "tests", m_testFiles.Count.ToString() );
                    writer.WriteAttributeString( "name", testSuiteName );

                    int failedTest = 0;
                    foreach( var file in m_testFiles )
                    {
                        if( file.NumErrors > 0 )
                            failedTest++;
                    }

                    writer.WriteAttributeString( "failures", failedTest.ToString() );


                    foreach( var file in m_testFiles )
                    {
                        WriteFileReport( writer, file );
                    }

                    writer.WriteEndElement();
                }
            }
        }

        private void        WriteFileReport( XmlWriter writer, TestFile file )
        {
            writer.WriteStartElement( "testsuite" );

            writer.WriteAttributeString( "name", file.FileName );
            writer.WriteAttributeString( "tests", file.TestEvents.Count.ToString() );
            writer.WriteAttributeString( "failures", file.NumErrors.ToString() );
            writer.WriteAttributeString( "warnings", file.NumWarnings.ToString() );
            writer.WriteAttributeString( "disabled", "0" );

            writer.WriteStartElement( "testcase" );
            writer.WriteAttributeString( "name", file.FileName );
            writer.WriteAttributeString( "status", "run" );
            writer.WriteAttributeString( "classname", file.FileName );

            foreach( var error in file.ErrorList )
            {
                if( error.IsError == ErrorRank.Error )
                {
                    writer.WriteStartElement( "failure" );
                    writer.WriteAttributeString( "message", error.Message );
                    writer.WriteEndElement();
                }
                else if( error.IsError == ErrorRank.Warning )
                {
                    writer.WriteStartElement( "warning" );
                    writer.WriteAttributeString( "message", error.Message );
                    writer.WriteEndElement();
                }
            }


            writer.WriteEndElement();
            writer.WriteEndElement();
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
                EventsView.ErrorSelected( m_selectedError );
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

        public float Progress
        {
            get
            {
                return (float)m_progressStepsMade / (float)m_numProgressSteps;
            }

            internal set
            {
                m_progressStepsMade += (int)value;
                NotifyPropertyChanged( "Progress" );
            }
        }

        public EventsViewModel EventsView
        {
            get
            {
                return m_eventsView;
            }

            set
            {
                m_eventsView = value;
            }
        }

        public Event CurrentTestEvent
        {
            get
            {
                if( SelectedFile != null )
                    return SelectedFile.CurrentTestEvent;
                return null;
            }

            set
            {
                SelectedFile.CurrentTestEvent = value;
                EventsView.SetTestEvent( SelectedFile.CurrentTestEvent );
                NotifyPropertyChanged( "CurrentTestEvent" );
            }
        }

        public Event CurrentReferenceResponse
        {
            get
            {
                if( SelectedFile != null )
                    return SelectedFile.CurrentResponse;
                return null;
            }

            set
            {
                SelectedFile.CurrentResponse = value;
                EventsView.SetReferenceResponse( SelectedFile.CurrentResponse );
                NotifyPropertyChanged( "CurrentReferenceResponse" );
            }
        }

        public Event CurrentRealResponse
        {
            get
            {
                if( SelectedFile != null )
                    return SelectedFile.CurrentRealResponse;
                return null;
            }

            set
            {
                SelectedFile.CurrentRealResponse = value;
                EventsView.SetRealResponse( SelectedFile.CurrentRealResponse );
                NotifyPropertyChanged( "CurrentRealResponse" );
            }
        }

        public int TabItemIdx
        {
            get
            {
                return m_tabItemIdx;
            }

            set
            {
                m_tabItemIdx = value;
            }
        }

        public string TestsPath
        {
            get { return m_testsPath; }
            set { m_testsPath = value; }
        }
        public string OutputPath
        {
            get { return m_outputPath; }
            set { m_outputPath = value; }
        }

        //public object TabItemIdx
        //{
        //    get
        //    {
        //        return m_tabItemIdx;
        //    }

        //    set
        //    {
        //        var tab = value as System.Windows.Controls.TabItem;
        //        if( tab != null )
        //        {
        //            if( tab.DataContext == this )
        //                m_tabItemIdx = 0;
        //            else
        //                m_tabItemIdx = 1;
        //        }
        //    }
        //}
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
