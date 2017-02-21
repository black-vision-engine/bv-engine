using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;



namespace RegressionLib.ComparisionException
{
    class IgnoreException : CmpException
    {
        string          m_commandName;


        public IgnoreException( string command )
        {
            m_commandName = command;
        }


        public bool CompareObjects( KeyValuePair<string, JToken> expected, JToken response, List<TestError> errorsList, Event expectedEvent, Event responseEvent )
        {
            if( expectedEvent.CommandName == m_commandName )
            {
                return true;
            }

            return false;
        }

    }
}
