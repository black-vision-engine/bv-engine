using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProfilerEditor.Tester
{
    public enum ErrorRank
    {
        ResultOk,
        Error,
        Warning
    }


    public class TestError
    {
        public ErrorRank        IsError { get; set; }

        public string           EventName { get; set; }
        public string           CommandName { get; set; }
        public UInt32           EventID { get; set; }

        public string           Message { get; set; }

        public Event            EventSent { get; set; }
        public Event            ReferenceReponse { get; set; }
        public Event            ReceivedResponse{ get; set; }

        public TestFile         FileRef { get; set; }

        public          TestError()
        {
            EventSent = null;
            ReceivedResponse = null;
            ReferenceReponse = null;
            FileRef = null;
        }

        public          TestError( Event newEvent, ErrorRank isError = ErrorRank.Error )
        {
            EventID = newEvent.EventID;
            EventName = newEvent.EventName;
            CommandName = newEvent.CommandName;
            IsError = isError;

            EventSent = null;
            ReferenceReponse = null;
            ReceivedResponse = newEvent;
        }
    }
}
