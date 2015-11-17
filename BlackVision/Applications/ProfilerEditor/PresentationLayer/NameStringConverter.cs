using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;


namespace ProfilerEditor.PresentationLayer
{
	public class NameStringConverter : IValueConverter
	{
		public NameStringConverter() { }

		public object Convert( object value, Type targetType, object parameter, System.Globalization.CultureInfo culture )
		{
			ProfilerModel.ProfilerSampleModel sample = (ProfilerModel.ProfilerSampleModel)value;
			MainWindow mainWindow = (MainWindow)parameter;

			UInt16 name = sample.GetData().name;
			ProfilerModel.NameMapping nameMap = mainWindow.ColorMapping;

			return nameMap.GetStringOf( name );
		}

		public object ConvertBack( object value, Type targetType, object parameter,
								System.Globalization.CultureInfo culture )
		{
			return null;
		}
	}
}
