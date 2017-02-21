using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;


namespace RegressionLib.PreprocessFilters
{
    public class FilterBase
    {



        protected static int RemoveResponses( ObservableCollection<Event> testEvents,
                                                        ObservableCollection<Event> referenceEvents,
                                                        int startRmRange,
                                                        int endRmRange,
                                                        int referenceCounter )
        {
            // Note last event won't be removed.
            for( int i = startRmRange; i < endRmRange - 1; ++i )
            {
                var curEvent = testEvents[ i ];
                referenceCounter = FindReferenceEvent( curEvent, referenceEvents, referenceCounter );

                if( referenceEvents.Count != referenceCounter )
                    referenceEvents.RemoveAt( referenceCounter );
            }

            return referenceCounter;
        }

        protected static void RemoveTestEvents( ObservableCollection<Event> testEvents,
                                                        ObservableCollection<Event> referenceEvents,
                                                        int startRmRange,
                                                        int endRmRange )
        {
            var numRemove = endRmRange - startRmRange - 1;

            while( numRemove > 0 )
            {
                testEvents.RemoveAt( startRmRange );
                numRemove--;
            }
        }

        protected static int FindReferenceEvent( Event curEvent, ObservableCollection<Event> referenceEvents, int referenceCounter )
        {
            // Note: we can't just delete successive events responses, because some other event could 
            for( ; referenceCounter < referenceEvents.Count; referenceCounter++ )
            {
                if( curEvent.EventID == referenceEvents[ referenceCounter ].EventID )
                    return referenceCounter;
            }
            return referenceCounter;
        }
    }
}
