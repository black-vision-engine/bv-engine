using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;


namespace ProfilerEditor.PresentationLayer
{
    public class EnableFileListConverter : IValueConverter
    {

        public object Convert( object value, Type targetType, object parameter, System.Globalization.CultureInfo culture )
        {
            RegressionLib.TestsState state = (RegressionLib.TestsState)value;

            if( state == RegressionLib.TestsState.Init )
                return true;
            return false;
        }

        public object ConvertBack( object value, Type targetType, object parameter,
                                System.Globalization.CultureInfo culture )
        {
            return null;
        }
    }
}
