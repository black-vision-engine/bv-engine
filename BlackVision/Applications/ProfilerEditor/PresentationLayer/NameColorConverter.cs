using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;

namespace ProfilerEditor.PresentationLayer
{
	public class NameColorConverter : IValueConverter
	{
		public object Convert( object value, Type targetType, object parameter,
								System.Globalization.CultureInfo culture )
		{
			//ProfilerModel.ProfilerSampleModel sample = (ProfilerModel.ProfilerSampleModel)value;
			Brush color = new SolidColorBrush( Color.FromArgb( 255, 200, 100, 40 ) );
			return color;
		}

		public object ConvertBack( object value, Type targetType, object parameter,
										System.Globalization.CultureInfo culture )
		{
			Brush color = new SolidColorBrush( Color.FromArgb( 255, 200, 100, 40 ) );
			return color;
		}
	}
}
