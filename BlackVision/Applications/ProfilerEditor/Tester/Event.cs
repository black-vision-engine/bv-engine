using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProfilerEditor.Tester
{
    public class Event
    {
        public UInt32       EventID { get; set; }
        public string       EventName { get; set; }
        public string       CommandName { get; set; }
        
        public string       EventContent { get; set; }

    }
}
