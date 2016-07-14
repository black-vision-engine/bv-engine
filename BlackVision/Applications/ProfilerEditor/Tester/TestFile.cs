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
        }

        public void         InitTest()
        {
            m_realResponses.Clear();

            m_testEventPtr = 0;
            m_responsePtr = 0;
        }

        // ================================================= //

        public string       SendingStep ()
        {
            if( m_testEventPtr >= TestEvents.Count )
                return null;

            string sendEvent = TestEvents[ (int)m_testEventPtr ].GetUnformattedContent();
            m_testEventPtr++;

            return sendEvent;
        }

        public TestError    ResponseStep( string response )
        {
            Event newEvent = ParseResponse( response );
            int expectedRespPtr = (int)m_responsePtr;
            m_responsePtr++;

            if( expectedRespPtr >= ReferenceResponses.Count )
            {
                // This should be warning only.
                TestError error = new TestError( newEvent );
                error.Message = "Reference data doesn't contain response to this message.";
                error.FileRef = this;
                error.EventSent = TestEvents[ (int)m_testEventPtr - 1 ];

                return error;
            }

            Event expectedResponse = ReferenceResponses[ (int)expectedRespPtr ];

            // First simple version.
            if( JToken.DeepEquals( newEvent.EventJSon, expectedResponse.EventJSon ) )
            {
                // There's no error in this response.
                return null;
            }
            else
            {
                TestError error = new TestError( newEvent );
                error.Message = "Response doesn't equals reference response.";
                error.FileRef = this;
                error.ReferenceReponse = expectedResponse;
                error.EventSent = TestEvents[ (int)m_testEventPtr - 1 ];

                return error;
            }
        }


        // ================================================= //

        #region Parsing

        const string    START_EVENT     = "#Evt#";
        const string    START_RESPONSE  = "#Res#";
        const string    END             = "#End#";

        public Event        ParseResponse( string content )
        {
            ParseSingleResponse( content, RealResponses );
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
                                ParseSingleMessage( singleMsg, TestEvents );
                            else
                                ParseSingleResponse( singleMsg, ReferenceResponses );

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

        public void         ParseSingleMessage( string msgString, ObservableCollection<Event> eventsCollection )
        {
            JObject json = JObject.Parse( msgString );
            var eventsArray = json[ "Events" ];

            foreach( var evt in eventsArray )
            {
                Event newEvent = new Event();
                newEvent.EventContent = msgString;
                newEvent.EventJSon = (JObject)evt;

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

        public void         ParseSingleResponse( string msgString, ObservableCollection<Event> eventsCollection )
        {
            JObject json = JObject.Parse( msgString );

            Event newEvent = new Event();
            newEvent.EventContent = msgString;
            newEvent.EventJSon = json;

            newEvent.EventName = "Response";
            newEvent.CommandName = json[ "cmd" ].ToString();

            JToken eventID = json[ "EventID" ];
            if( eventID != null )
                newEvent.EventID = eventID.Value<UInt32>();
            else
                newEvent.EventID = UInt32.MaxValue;

            eventsCollection.Add( newEvent );
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

    #endregion
    }
}
