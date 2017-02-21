using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;



namespace RegressionLib.PreprocessFilters
{
    public class AddKeyFilter : FilterBase
    {


        public static void         Preprocess  ( ObservableCollection< Event > testEvents, ObservableCollection< Event > referenceEvents )
        {
            int testCounter = 0;
            int referenceCounter = 0;

            for( int i = 0; i < testEvents.Count; ++i )
            {
                if( testEvents[ i ].CommandName == "AddKey" )
                {
                    testCounter = i;
                    referenceCounter = Group( testEvents, referenceEvents, testCounter, referenceCounter );
                }
            }
        }


        private static int          Group       (   ObservableCollection< Event > testEvents,
                                                    ObservableCollection< Event > referenceEvents,
                                                    int testCounter,
                                                    int referenceCounter )
        {
            int startRmRange = testCounter;
            int endRmRange = startRmRange;
            Event testEvent = testEvents[ testCounter ];

            for( ; endRmRange < testEvents.Count; ++endRmRange )
            {
                if( !SimilarAddKey( testEvent, testEvents[ endRmRange ] ) )
                    break;
            }

            if( startRmRange < endRmRange - 1 )
            {
                referenceCounter = RemoveResponses( testEvents, referenceEvents, startRmRange, endRmRange, referenceCounter );
                RemoveTestEvents( testEvents, referenceEvents, startRmRange, endRmRange );
            }

            return referenceCounter;
        }

        private static bool         SimilarAddKey   ( Event firstEvent, Event checkedEvent )
        {
            if( firstEvent.EventName != checkedEvent.EventName )
                return false;

            var firstJSON = firstEvent.EventJSon;
            var secondJSON = checkedEvent.EventJSon;

            try
            {
                if( !firstJSON[ "SceneName" ].Equals( secondJSON[ "SceneName" ] ) )
                    return false;

                if( !firstJSON[ "NodeName" ].Equals( secondJSON[ "NodeName" ] ) )
                    return false;

                if( !firstJSON[ "PluginName" ].Equals( secondJSON[ "PluginName" ] ) )
                    return false;

                if( !firstJSON[ "ParamName" ].Equals( secondJSON[ "ParamName" ] ) )
                    return false;

                if( !firstJSON[ "Time" ].Equals( secondJSON[ "Time" ] ) )
                    return false;

                if( !firstJSON[ "Target" ].Equals( secondJSON[ "Target" ] ) )
                    return false;
            }
            catch( Exception )
            {
                return false;
            }

            var subName1 = firstJSON.GetValue( "ParamSubName" );
            var subName2 = secondJSON.GetValue( "ParamSubName" );

            if( subName1 != null && subName2 != null && !subName1.Equals( subName2 ) )
                return false;

            var index1 = firstJSON.GetValue( "Index" );
            var index2 = secondJSON.GetValue( "Index" );
            if( index1 != null && index2 != null && !index1.Equals( index2 ) )
                return false;

            return true;
        }


    }
}
