using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;


namespace RegressionLib
{
    public class Event
    {
        public UInt32       EventID { get; set; }
        public string       EventName { get; set; }
        public string       CommandName { get; set; }

        private string      m_eventContent;
        public JObject      EventJSon;
        public bool         SyncEvent { get; set; }

        public TimeSpan     Time;
        public bool         Used;
        public bool         Breakpoint { get; set; }

        public string       EventContent
        {
            get
            {
                if( EventJSon != null )
                    return EventJSon.ToString();
                else
                    return m_eventContent;
            }
            set
            {
                m_eventContent = value;
            }
        }

        public string GetUnformattedContent()
        {
            return m_eventContent;
        }

        public Event()
        {
            SyncEvent = false;
            Used = false;
            Breakpoint = false;
        }
    }
}
