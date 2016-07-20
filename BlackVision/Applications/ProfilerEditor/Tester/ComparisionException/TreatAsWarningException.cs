using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace ProfilerEditor.Tester.ComparisionException
{
    class TreatAsWarningException : CmpException
    {
        string          m_fieldName;
        string          m_commandName;


        TreatAsWarningException( string fieldName, string command )
        {
            m_fieldName = fieldName;
            m_commandName = command;
        }


        public bool CompareObjects( JToken expected, JToken response, List<TestError> errorsList, Event expectedEvent, Event responseEvent )
        {
            if( expectedEvent.CommandName == m_commandName )
            {



                return true;
            }

            return false;
        }
    }
}
