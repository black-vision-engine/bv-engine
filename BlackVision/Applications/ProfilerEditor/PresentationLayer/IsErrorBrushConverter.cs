﻿using System;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Data;
using System.Globalization;
using System.Windows.Media;


namespace ProfilerEditor.PresentationLayer
{
    /// <summary>
    /// Convert Level to left margin
    /// Pass a prarameter if you want a unit length other than 19.0.
    /// </summary>
    public class IsErrorBrushConverter : IValueConverter
    {
        public object Convert( object value, Type type, object parameter, CultureInfo culture )
        {
            Nullable< ProfilerEditor.Tester.ErrorRank > val = value as Nullable< ProfilerEditor.Tester.ErrorRank >;
            if( val.Value == ProfilerEditor.Tester.ErrorRank.Error )
                return Brushes.Red;
            else if( val.Value == ProfilerEditor.Tester.ErrorRank.Warning )
                return Brushes.Orange;
            else
                return Brushes.Green;
        }

        public object ConvertBack( object value, Type type, object parameter, CultureInfo culture )
        {
            return false;
        }


    }
}
