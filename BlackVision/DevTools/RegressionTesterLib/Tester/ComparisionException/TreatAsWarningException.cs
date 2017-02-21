using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace RegressionLib.ComparisionException
{
    class TreatAsWarningException : CmpException
    {
        string          m_fieldName;
        string          m_commandName;


        public TreatAsWarningException( string fieldName, string command )
        {
            m_fieldName = fieldName;
            m_commandName = command;
        }


        public bool CompareObjects( KeyValuePair< string, JToken > expected, JToken response, List<TestError> errorsList, Event expectedEvent, Event responseEvent )
        {
            if( expectedEvent.CommandName == m_commandName && expected.Key == m_fieldName )
            {
                if( expected.Value == response )
                    return true;

                string message = "[ " +  expected.Value.Path + " ] Timelines current times are different. (Local time is never the same in test)";

                TestError error = ComparisionRules.CreateTestError( expectedEvent, responseEvent, message );
                error.IsError = ErrorRank.Warning;
                errorsList.Add( error );

                return true;
            }

            return false;
        }
    }
}
