using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;

namespace ProfilerEditor.PresentationLayer
{
	public class TimeFormatter : IValueConverter
	{
		public TimeFormatter() { }

		public object Convert( object value, Type targetType, object parameter,
						System.Globalization.CultureInfo culture )
		{
			MainWindow mainWindow = (MainWindow)parameter;
			int selectedUnit = mainWindow.m_timeFormatUnits;


			float time = (float)value;
			if( selectedUnit == 1 )		// milliseconds
				time = time * 1000;

			return time.ToString( mainWindow.m_timeFormatString, culture );
		}

		public object ConvertBack( object value, Type targetType, object parameter,
										System.Globalization.CultureInfo culture )
		{
			return null;
		}


	}
}
