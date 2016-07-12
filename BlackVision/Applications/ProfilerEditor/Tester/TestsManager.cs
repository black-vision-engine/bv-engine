using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace ProfilerEditor.Tester
{
    public class TestsManager
    {
        private ObservableCollection< TestFile >    m_testFiles;
        private TestFile                            m_selectedFile;
        private string                              m_testsPath;

        const string    START_EVENT     = "#Evt#";
        const string    START_RESPONSE  = "#Res#";
        const string    END             = "#End#";

        public TestsManager()
        {
            SelectedFile = null;
            TestFiles = new ObservableCollection<TestFile>();
        }


        public void    UpdateTestPath( string newPath )
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

        public void     DebugCurrentFile()
        {
            if( SelectedFile != null )
                ParseFile( SelectedFile );
        }

        public void     ParseFile   ( TestFile testFile )
        {
            string content = ReadFile( Path.Combine( m_testsPath, testFile.FileName ) );

            int startIdx = 0;
            int endIdx = 0;

            while( startIdx < content.Length )
            {
                startIdx = content.IndexOf( '#', startIdx );
                if( startIdx < 0 )
                    break;

                string header = content.Substring( startIdx, 5 );

                bool isEvent = header.Contains( START_EVENT );
                bool isResponse = header.Contains( START_RESPONSE );

                if( isEvent || isResponse )
                {
                    startIdx += 6;  // START_EVENT or START_RESPONSE string + \n character
                    endIdx = startIdx;

                    while( endIdx < content.Length )
                    {
                        endIdx = content.IndexOf( '#', endIdx );
                        header = content.Substring( endIdx, 5 );

                        if( header.Contains( END ) )
                        {
                            string singleMsg = content.Substring( startIdx, endIdx - startIdx - 2 );

                            if( isEvent )
                                ParseSingleMessage( singleMsg, testFile.TestEvents );
                            else
                                ParseSingleResponse( singleMsg, testFile.ReferenceResponses );

                            endIdx += 6;  //END string + \n character
                            break;
                        }
                        else
                            endIdx++;
                    }
                }
                else
                    startIdx++;

                startIdx = endIdx;
            }

            
        }

        public void    ParseSingleMessage  ( string msgString, ObservableCollection< Event > eventsCollection )
        {
            JObject json = JObject.Parse( msgString );
            var eventsArray = json[ "Events" ];

            foreach( var evt in eventsArray )
            {
                Event newEvent = new Event();
                newEvent.EventContent = msgString;

                newEvent.EventName = evt[ "Event" ].ToString();
                newEvent.CommandName = evt[ "Command" ].ToString();

                JToken eventID = evt[ "EventID" ];
                if( eventID != null )
                    newEvent.EventID = eventID.Value<UInt32>();
                else
                    newEvent.EventID = UInt32.MaxValue;

                eventsCollection.Add( newEvent );
            }
        }

        public void ParseSingleResponse( string msgString, ObservableCollection<Event> eventsCollection )
        {
            JObject json = JObject.Parse( msgString );

            Event newEvent = new Event();
            newEvent.EventContent = msgString;

            newEvent.EventName = "Response";
            newEvent.CommandName = json[ "cmd" ].ToString();

            JToken eventID = json[ "EventID" ];
            if( eventID != null )
                newEvent.EventID = eventID.Value<UInt32>();
            else
                newEvent.EventID = UInt32.MaxValue;

            eventsCollection.Add( newEvent );
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
                //foreach( var file in TestFiles )
                //{
                //    if( m_selectedFile == file )
                //    {

                //    }
                //}
            }
        }
        #endregion
    }
}
