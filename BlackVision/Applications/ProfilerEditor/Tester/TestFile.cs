using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using Newtonsoft.Json.Linq;


namespace ProfilerEditor.Tester
{
    public class TestFile
    {
        public string FileName { get; set; }

        private ObservableCollection< Event >   m_testEvents;
        private ObservableCollection< Event >   m_referenceResponses;
        private ObservableCollection< Event >   m_realResponses;

        private UInt32                          m_testEventPtr;
        private UInt32                          m_responsePtr;

        private int                             m_responseTimeout;
        private static float                    m_timeoutScale = 1.5F;
        private static int                      m_additionalWaitTime = 2;

        private int                             m_numErrors;
        private int                             m_numWarnings;      // For future use

        public Event        CurrentTestEvent { get; set; }
        public Event        CurrentResponse { get; set; }
        public Event        CurrentRealResponse { get; set; }


        override public string ToString()
        {
            return "ProfilerEditor.Tester.TestFile";
        }

        public              TestFile()
        {
            m_testEvents = new ObservableCollection< Event >();
            m_referenceResponses = new ObservableCollection< Event >();
            m_realResponses = new ObservableCollection< Event >();

            NumErrors = 0;
            NumWarnings = 0;
        }

        public void         InitTest()
        {
            m_realResponses.Clear();

            m_testEventPtr = 0;
            m_responsePtr = 0;

            NumErrors = 0;
            NumWarnings = 0;
        }

        // ================================================= //

        public TestError    Timeout     ( int seconds )
        {
            if( m_responseTimeout < seconds )
            {
                var expResp = ReferenceResponses[ (int)m_responsePtr ];

                TestError error = new TestError( expResp );
                error.Message = "Message timeout: [ " + seconds.ToString() + " ] s";
                error.FileRef = this;

                if( !expResp.SyncEvent )
                    error.EventSent = TestEvents[ (int)m_testEventPtr - 1 ];

                // Ignore expected mesage and take next in queue.
                expResp.Used = false;
                m_responsePtr++;

                return error;
            }
            return null;
        }

        public string       SendingStep ()
        {
            if( m_testEventPtr >= TestEvents.Count )
                return null;

            if( ReferenceResponses.Count <= m_responsePtr )
                return null;

            if( ReferenceResponses[ (int)m_responsePtr ].SyncEvent )
            {
                var timeDiff = ReferenceResponses[ (int)m_responsePtr ].Time - ReferenceResponses[ (int)m_responsePtr - 1 ].Time;
                m_responseTimeout = (int)( timeDiff.TotalSeconds * m_timeoutScale ) + m_additionalWaitTime;

                // Don't send next event before TriggerEvent comes.
                return "{}";
            }


            string sendEvent = TestEvents[ (int)m_testEventPtr ].GetUnformattedContent();

            var timeDiff2 = ReferenceResponses[ (int)m_responsePtr ].Time - TestEvents[ (int)m_testEventPtr ].Time;
            m_responseTimeout = (int)( timeDiff2.TotalSeconds * m_timeoutScale ) + m_additionalWaitTime;

            m_testEventPtr++;

            return sendEvent;
        }

        public List< TestError >    ResponseStep( string response, ComparisionRules rules )
        {
            Event newEvent = ParseResponse( response );
            Event expectedResponse = FindCorespondingEvent( newEvent );

            if( expectedResponse == null )
            {
                // This should be warning only.
                TestError error = new TestError( newEvent );
                error.Message = "Reference data doesn't contain response to this message.";
                error.FileRef = this;
                error.EventSent = TestEvents[ (int)m_testEventPtr - 1 ];

                List < TestError > errorsList = new List< TestError >();
                errorsList.Add( error );
                CountErrorsWarnings( errorsList );

                return errorsList;
            }

            // Reference and reponse comparision.
            List < TestError > errors = rules.Compare( expectedResponse, newEvent );
            if( errors == null )
            {
                expectedResponse.Used = true;
                // There's no error in this response.
                return null;
            }
            else
            {
                expectedResponse.Used = true;
                CountErrorsWarnings( errors );

                // Complete error info.
                foreach( var error in errors )
                {
                    error.FileRef = this;
                    if( !expectedResponse.SyncEvent )
                        error.EventSent = TestEvents[ (int)m_testEventPtr - 1 ];
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

            return ReferenceResponses[ (int)expectedRespPtr ];
        }

        private void        CountErrorsWarnings ( List< TestError > errorsList )
        {
            // Count warnings in future.
            NumErrors += errorsList.Count;
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

        #endregion
    }
}
