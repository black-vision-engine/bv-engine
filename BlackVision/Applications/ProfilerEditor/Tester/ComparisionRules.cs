using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;



namespace ProfilerEditor.Tester
{
    public class ComparisionRules
    {



        public List< TestError >    Compare ( Event expectedResponse, Event response )
        {
            if( JToken.DeepEquals( response.EventJSon, expectedResponse.EventJSon ) )
                return null;

            List < TestError > errorsList = new List< TestError >();

            ComparatorSelector( expectedResponse.EventJSon, response.EventJSon, errorsList, expectedResponse, response );

            return errorsList;
        }

        private void                CompareObjects      ( JObject expected, JObject response, List< TestError > errorsList, Event expectedEvent, Event responseEvent )
        {
            foreach( var child in expected )
            {
                var respChild = response[ child.Key ];

                if( respChild == null )
                {
                    string message = "[ " +  child.Value.Path + " ] Doesn't exists in response.";

                    TestError error = CreateTestError( expectedEvent, responseEvent, message );
                    errorsList.Add( error );
                }
                else
                {
                    ComparatorSelector( child.Value, respChild, errorsList, expectedEvent, responseEvent );
                }
            }
                
        }

        private void                CompareArrays       ( JArray expected, JArray response, List< TestError > errorsList, Event expectedEvent, Event responseEvent )
        {
            int maxIdx = expected.Count;
            if( maxIdx > response.Count )
            {
                string message = "[ " +  expected.Path + " ] Response array have more elements.";

                TestError error = CreateTestError( expectedEvent, responseEvent, message );
                errorsList.Add( error );

                maxIdx = response.Count;
            }


            for( int i = 0; i < maxIdx; ++i  )
            {
                JToken expChild = expected[ i ];
                JToken resChild = response[ i ];

                if( resChild == null )
                {
                    string message = "[ " + expected.Path + " ] Reference array have more elements.";

                    TestError error = CreateTestError( expectedEvent, responseEvent, message );
                    errorsList.Add( error );

                    return;
                }

                ComparatorSelector( expChild, resChild, errorsList, expectedEvent, responseEvent );
            }
        }

        private void                CompareAttributes   ( JToken expected, JToken response, List< TestError > errorsList, Event expectedEvent, Event responseEvent )
        {
            if( expected.Value< string >() != response.Value< string >() )
            {
                string message = "[ " + expected.Path + " ] Attribute values are different [ expected = " + expected.ToString() + " ] [ response = " + response.ToString() + " ]";

                TestError error = CreateTestError( expectedEvent, responseEvent, message );
                errorsList.Add( error );

                return;
            }
        }

        private void                ComparatorSelector  ( JToken expected, JToken response, List< TestError > errorsList, Event expectedEvent, Event responseEvent )
        {
            if( expected.Type != response.Type )
            {
                string message = "[ " +  expected.Path + " ] Reference and reponse tokens types are different.";

                TestError error = CreateTestError( expectedEvent, responseEvent, message );
                errorsList.Add( error );

                return;
            }

            if( expected.Type == JTokenType.Array )
                CompareArrays( expected.Value< JArray >(), response.Value< JArray >(), errorsList, expectedEvent, responseEvent );
            else if( expected.Type == JTokenType.Object )
                CompareObjects( expected.Value< JObject >(), response.Value< JObject >(), errorsList, expectedEvent, responseEvent );
            else if( expected.Type == JTokenType.String )
                CompareAttributes( expected, response, errorsList, expectedEvent, responseEvent );
            else
            {
                string message = "[ " + expected.Path + "] Token is not array object or string. Other types are not supported.";

                TestError error = CreateTestError( expectedEvent, responseEvent, message );
                errorsList.Add( error );

                return;
            }

        }

        private TestError           CreateTestError     ( Event expectedEvent, Event responseEvent, string message )
        {
            TestError error = new TestError( responseEvent );
            error.ReferenceReponse = expectedEvent;
            error.Message = message;

            return error;
        }

    }
}
