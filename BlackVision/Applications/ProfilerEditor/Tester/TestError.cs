using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProfilerEditor.Tester
{
    public class TestError
    {
        public bool             IsError { get; set; }

        public string           EventName { get; set; }
        public string           CommandName { get; set; }
        public UInt32           EventID { get; set; }

        public string           Message { get; set; }

        //public Event            EventSent { get; set; }
        public Event            ReferenceReponse { get; set; }
        public Event            ReceivedResponse{ get; set; }
        public TestFile         FileRef { get; set; }

        public          TestError()
        {
            //EventSent = null;
            ReceivedResponse = null;
            ReceivedResponse = null;
            FileRef = null;
        }

        public          TestError( Event newEvent, bool isError = true )
        {
            EventID = newEvent.EventID;
            EventName = newEvent.EventName;
            CommandName = newEvent.CommandName;
            IsError = isError;

            ReceivedResponse = newEvent;
        }
    }
}
