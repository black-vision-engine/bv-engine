using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;


namespace ProfilerEditor.PresentationLayer
{
    public class TesterStateToStringCoverter : IValueConverter
    {

        public object Convert( object value, Type targetType, object parameter, System.Globalization.CultureInfo culture )
        {
            Tester.TestsState state = (Tester.TestsState)value;

            if( state == Tester.TestsState.WaitingForResponse )
                return "Waiting for response";
            else if( state == Tester.TestsState.WaitingForInput )
                return "Sending message";
            else if( state == Tester.TestsState.Init )
                return "Test not started";
            else
                return "Testing";
        }

        public object ConvertBack( object value, Type targetType, object parameter,
                                System.Globalization.CultureInfo culture )
        {
            return null;
        }
    }
}
