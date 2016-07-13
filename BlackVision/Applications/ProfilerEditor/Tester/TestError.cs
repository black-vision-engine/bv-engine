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

        public Event            EventRef;
        public TestFile         FileRef { get; set; }

        public          TestError()
        {
            EventRef = null;
            FileRef = null;
        }

        public          TestError( Event newEvent, bool isError = true )
        {
            EventID = newEvent.EventID;
            EventName = newEvent.EventName;
            CommandName = newEvent.CommandName;
            IsError = isError;

            EventRef = newEvent;
        }
    }
}
