using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;




namespace ProfilerEditor.Tester.PreprocessFilters
{
    public class CheckTimelineTimeFilter : FilterBase
    {


        public static void Preprocess( ObservableCollection<Event> testEvents, ObservableCollection<Event> referenceEvents )
        {
            int referenceCounter = 0;

            for( int i = 0; i < testEvents.Count; ++i )
            {
                var curEvent = testEvents[ i ];
                if( curEvent.CommandName == "CheckTimelineTime" )
                {
                    referenceCounter = RemoveResponses( testEvents, referenceEvents, i, i + 2, referenceCounter );
                    RemoveTestEvents( testEvents, referenceEvents, i, i + 2 );

                    i--;    // Now thre's other event under this index.
                }
            }
        }

    }
}
