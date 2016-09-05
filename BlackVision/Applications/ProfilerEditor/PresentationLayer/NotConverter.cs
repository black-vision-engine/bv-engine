using System;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Data;
using System.Globalization;


namespace ProfilerEditor.PresentationLayer
{
    /// <summary>
    /// Convert Level to left margin
    /// Pass a prarameter if you want a unit length other than 19.0.
    /// </summary>
    public class NotConverter : IValueConverter
    {
        public object Convert(object value, Type type, object parameter, CultureInfo culture)
        {
            Nullable< bool > val = value as Nullable<bool>;
            return !val.Value;
        }

        public object ConvertBack(object value, Type type, object parameter, CultureInfo culture)
        {
            Nullable<bool> val = value as Nullable<bool>;
            return !val.Value;
        }


    }
}
