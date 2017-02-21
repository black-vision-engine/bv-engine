using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;



namespace RegressionLib.ComparisionException
{
    interface CmpException
    {

        /// <summary>
        /// 
        /// </summary>
        /// <param name="expected"></param>
        /// <param name="response"></param>
        /// <param name="errorsList"></param>
        /// <param name="expectedEvent"></param>
        /// <param name="responseEvent"></param>
        /// <returns>Returns true if objects were compared as exception. Otherwise you have to use other
        /// compare function.</returns>
        bool CompareObjects( KeyValuePair< string, JToken > expected, JToken response, List<TestError> errorsList, Event expectedEvent, Event responseEvent );
    }
}
