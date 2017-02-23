using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Collections.ObjectModel;
using Newtonsoft.Json.Linq;


namespace RegressionLib
{
    public class TestFile : INotifyPropertyChanged
    {
        public string FileName { get; set; }

        private ObservableCollection< Event >   m_testEvents;
        private ObservableCollection< Event >   m_referenceResponses;
        private ObservableCollection< Event >   m_realResponses;

        private List< TestError >               m_errorList;

        private UInt32                          m_testEventPtr;
        private UInt32                          m_responsePtr;

        private int                             m_responseTimeout;
        private static float                    m_timeoutScale = 2.5F;
        private static int                      m_additionalWaitTime = 2;

        private int                             m_numErrors;
        private int                             m_numWarnings;      // For future use



        private Event        m_currentTestEvent;
        private Event        m_currentResponse;
        private Event        m_currentRealResponse;


        override public string ToString()
        {
            return "ProfilerEditor.Tester.TestFile";
        }

        public              TestFile()
        {
            m_testEvents = new ObservableCollection< Event >();
            m_referenceResponses = new ObservableCollection< Event >();
            m_realResponses = new ObservableCollection< Event >();

            m_errorList = new List< TestError >();

            NumErrors = 0;
            NumWarnings = 0;
        }

        public void         InitTest()
        {
            RealResponses.Clear();

            m_testEventPtr = 0;
            m_responsePtr = 0;

            NumErrors = 0;
            NumWarnings = 0;
        }

        public bool         IsBreakPoint()
        {
            if( m_testEventPtr + 1 >= TestEvents.Count )
                return false;

            if( TestEvents[ (int)m_testEventPtr - 1 ].Breakpoint )
                return true;

            return false;
        }

        // ================================================= //

        public TestError    Timeout     ( int seconds )
        {
            if( ReferenceResponses.Count <= m_responsePtr )
            {
                TestError error = new TestError( TestEvents[ (int)m_testEventPtr - 1 ] );
                error.Message = "No more responses in reference file";
                error.FileRef = this;
                error.IsError = ErrorRank.Warning;
                error.EventSent = TestEvents[(int)m_testEventPtr - 1];
                error.ReceivedResponse = null;

                m_responsePtr++;
                m_errorList.Add( error );       // Local error list.
                return error;
            }

            if( m_responseTimeout < seconds )
            {
                var expResp = ReferenceResponses[ (int)m_responsePtr ];

                TestError error = new TestError();
                error.Message = "Reponse timeout: [ " + seconds.ToString() + " ] s";
                error.FileRef = this;
                error.IsError = ErrorRank.Warning;
                error.ReferenceReponse = expResp;

                if( !expResp.SyncEvent )
                    error.EventSent = TestEvents[ (int)m_testEventPtr - 1 ];

                // Ignore expected mesage and take next in queue.
                expResp.Used = false;
                m_responsePtr++;

                m_errorList.Add( error );       // Local error list.
                return error;
            }
            return null;
        }

        public string       SendingStep ()
        {
            if( m_testEventPtr >= TestEvents.Count )
                return null;

            bool responseExist = true;
            if( ReferenceResponses.Count <= m_responsePtr )
                responseExist = false;

            if( responseExist && ReferenceResponses[(int)m_responsePtr].SyncEvent )
            {
                var timeDiff = ReferenceResponses[ (int)m_responsePtr ].Time - ReferenceResponses[ (int)m_responsePtr - 1 ].Time;
                m_responseTimeout = (int)( timeDiff.TotalSeconds * m_timeoutScale ) + m_additionalWaitTime;

                // Don't send next event before TriggerEvent comes.
                return "{}";
            }


            string sendEvent = TestEvents[ (int)m_testEventPtr ].GetUnformattedContent();

            if( responseExist )
            {
                var timeDiff2 = ReferenceResponses[(int)m_responsePtr].Time - TestEvents[(int)m_testEventPtr].Time;
                m_responseTimeout = (int)( timeDiff2.TotalSeconds * m_timeoutScale ) + m_additionalWaitTime;
            }
            else
                m_responseTimeout = 0;

            CurrentTestEvent = TestEvents[ (int)m_testEventPtr ];
            m_testEventPtr++;

            return sendEvent;
        }

        public List< TestError >    ResponseStep( string response, ComparisionRules rules )
        {
            Event newEvent = ParseResponse( response );
            Event expectedResponse = FindCorespondingEvent( newEvent );
            CurrentResponse = expectedResponse;

            if( expectedResponse == null )
            {
                TestError error = new TestError( newEvent );
                error.Message = "Reference data doesn't contain this response.";
                error.FileRef = this;
                error.EventSent = TestEvents[ (int)m_testEventPtr - 1 ];
                error.IsError = ErrorRank.Warning;

                List < TestError > errorsList = new List< TestError >();
                errorsList.Add( error );
                m_errorList.Add( error );       // Local error list.
                CountErrorsWarnings( errorsList );

                return errorsList;
            }

            if( expectedResponse.CommandName != newEvent.CommandName )
            {
                TestError error = new TestError( newEvent );
                error.Message = "Command response from engine [ " + newEvent.CommandName + " ] and reference [ " + expectedResponse.CommandName + " ] are different";
                error.FileRef = this;
                error.EventSent = TestEvents[ (int)m_testEventPtr - 1 ];
                error.IsError = ErrorRank.Error;

                List < TestError > errorsList = new List< TestError >();
                errorsList.Add( error );
                m_errorList.Add( error );       // Local error list.
                CountErrorsWarnings( errorsList );

                return errorsList;
            }

            // Reference and reponse comparision.
            List < TestError > errors = rules.Compare( expectedResponse, newEvent );
            expectedResponse.Used = true;

            if( errors == null )
            {    
                // There's no error in this response.
                return null;
            }
            else
            {
                CountErrorsWarnings( errors );

                // Complete error info.
                foreach( var error in errors )
                {
                    error.FileRef = this;
                    //if( !expectedResponse.SyncEvent )
                        error.EventSent = TestEvents[ (int)m_testEventPtr - 1 ];

                    m_errorList.Add( error );       // Local error list.
                }

                return errors;
            }
        }

        private Event       FindCorespondingEvent   ( Event newEvent )
        {
            int expectedRespPtr = (int)m_responsePtr;
            m_responsePtr++;

            if( expectedRespPtr >= ReferenceResponses.Count )
                return null;


            if( newEvent.SyncEvent )
            {
                if( ReferenceResponses[ (int)expectedRespPtr ].SyncEvent )
                {
                    if( newEvent.CommandName == ReferenceResponses[ (int)expectedRespPtr ].CommandName )
                        return ReferenceResponses[ (int)expectedRespPtr ];
                    else
                        return null;
                }
                else
                {
                    m_responsePtr--;
                }

                return null;
            }
            else
            {
                while( ReferenceResponses[ (int)expectedRespPtr ].SyncEvent )
                    expectedRespPtr++;

                m_responsePtr = (uint)expectedRespPtr + 1;

                var expectedResp = ReferenceResponses[ (int)expectedRespPtr ];
                if( expectedResp.Used )
                    return null;

                return ReferenceResponses[ (int)expectedRespPtr ];
            }
        }

        private void        CountErrorsWarnings ( List< TestError > errorsList )
        {
            // Count warnings in future.
            foreach( var error in errorsList )
            {
                if( error.IsError == ErrorRank.Error )
                    NumErrors++;
                else if( error.IsError == ErrorRank.Warning )
                    NumWarnings++;
            }
            
        }


        // ================================================= //

        #region Parsing

        const string    START_EVENT     = "#Evt#";
        const string    START_RESPONSE  = "#Res#";
        const string    END             = "#End#";

        public Event        ParseResponse( string content )
        {
            ParseSingleResponse( content, RealResponses, new TimeSpan() );
            return RealResponses.Last();
        }

        public void         ParseFile( string content )
        {
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
                    startIdx += 5;  // START_EVENT or START_RESPONSE string

                    // Now find timestamp
                    endIdx = content.IndexOf( '#', startIdx );
                    TimeSpan timestamp = ParseTimestamp( content.Substring( startIdx, endIdx - startIdx ) );

                    startIdx = endIdx + 1;
                    endIdx = startIdx;


                    while( endIdx < content.Length )
                    {
                        endIdx = content.IndexOf( '#', endIdx );
                        header = content.Substring( endIdx, 5 );

                        if( header.Contains( END ) )
                        {
                            string singleMsg = content.Substring( startIdx, endIdx - startIdx - 2 );

                            if( isEvent )
                                ParseSingleMessage( singleMsg, TestEvents, timestamp );
                            else
                                ParseSingleResponse( singleMsg, ReferenceResponses, timestamp );

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

            PreprocessEvents();
        }

        public void         ParseSingleMessage( string msgString, ObservableCollection<Event> eventsCollection, TimeSpan time )
        {
            JObject json = JObject.Parse( msgString );
            var eventsArray = json[ "Events" ];

            foreach( var evt in eventsArray )
            {
                Event newEvent = new Event();
                newEvent.EventContent = msgString;
                newEvent.EventJSon = (JObject)evt;
                newEvent.Time = time;

                var eventField = evt[ "Event" ];
                var commandField = evt[ "Command" ];

                if( eventField != null )
                    newEvent.EventName = eventField.ToString();
                if( commandField != null )
                    newEvent.CommandName = commandField.ToString();

                JToken eventID = evt[ "EventID" ];
                if( eventID != null )
                    newEvent.EventID = eventID.Value<UInt32>();
                else
                    newEvent.EventID = UInt32.MaxValue;

                eventsCollection.Add( newEvent );
            }
        }

        public void         ParseSingleResponse( string msgString, ObservableCollection<Event> eventsCollection, TimeSpan time )
        {
            JObject json = JObject.Parse( msgString );

            Event newEvent = new Event();
            newEvent.EventContent = msgString;
            newEvent.EventJSon = json;
            newEvent.Time = time;

            newEvent.CommandName = json[ "cmd" ].ToString();
            var triggerEvent = json[ "TriggerEvent" ];

            if( triggerEvent != null )
            {
                newEvent.EventName = triggerEvent.ToString();
                newEvent.SyncEvent = true;
            }
            else
            {
                newEvent.EventName = "Response";
            }
            

            JToken eventID = json[ "EventID" ];
            if( eventID != null )
                newEvent.EventID = eventID.Value<UInt32>();
            else
                newEvent.EventID = UInt32.MaxValue;

            eventsCollection.Add( newEvent );
        }

        public TimeSpan     ParseTimestamp  ( string timeStr )
        {
            UInt16 hours = UInt16.Parse( timeStr.Substring( 0, 4 ) );
            UInt16 minutes = UInt16.Parse( timeStr.Substring( 5, 2 ) );
            UInt16 seconds = UInt16.Parse( timeStr.Substring( 8, 2 ) );
            UInt16 millis = UInt16.Parse( timeStr.Substring( 11, 3 ) );

            TimeSpan span = new TimeSpan( hours / 24, hours % 24, minutes, seconds, millis );
            return span;
        }

        private void        PreprocessEvents    ()
        {
            // Replace groups of AddKey events with one event setting parameter to last value.
            PreprocessFilters.AddKeyFilter.Preprocess( TestEvents, ReferenceResponses );
            PreprocessFilters.CheckTimelineTimeFilter.Preprocess( TestEvents, ReferenceResponses );

        }

        #endregion


        #region Properties
        public ObservableCollection<Event> TestEvents
        {
            get
            {
                return m_testEvents;
            }

            set
            {
                m_testEvents = value;
            }
        }

        public ObservableCollection<Event> ReferenceResponses
        {
            get
            {
                return m_referenceResponses;
            }

            set
            {
                m_referenceResponses = value;
            }
        }

        public ObservableCollection<Event> RealResponses
        {
            get
            {
                return m_realResponses;
            }

            set
            {
                m_realResponses = value;
            }
        }

        public int NumErrors
        {
            get
            {
                return m_numErrors;
            }

            set
            {
                m_numErrors = value;
            }
        }

        public int NumWarnings
        {
            get
            {
                return m_numWarnings;
            }

            set
            {
                m_numWarnings = value;
            }
        }

        public Event CurrentTestEvent
        {
            get
            {
                return m_currentTestEvent;
            }

            set
            {
                m_currentTestEvent = value;
                NotifyPropertyChanged( "CurrentTestEvent" );
            }
        }

        public Event CurrentResponse
        {
            get
            {
                return m_currentResponse;
            }

            set
            {
                m_currentResponse = value;
                NotifyPropertyChanged( "CurrentResponse" );
            }
        }

        public Event CurrentRealResponse
        {
            get
            {
                return m_currentRealResponse;
            }

            set
            {
                m_currentRealResponse = value;
                NotifyPropertyChanged( "CurrentRealResponse" );
            }
        }

        public List<TestError> ErrorList { get => m_errorList; set => m_errorList = value; }

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
