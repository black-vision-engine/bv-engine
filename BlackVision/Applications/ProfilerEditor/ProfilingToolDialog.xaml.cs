using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Microsoft.Win32;



namespace ProfilerEditor
{
	/// <summary>
	/// Interaction logic for ProfilingToolDialog.xaml
	/// </summary>
	public partial class ProfilingToolDialog : Window
	{
		public ProfilingToolDialog()
		{
			InitializeComponent();
		}

		private void OpenBlackVision_Click( object sender, RoutedEventArgs e )
		{
			OpenFileDialog openFileDialog = new OpenFileDialog();
			openFileDialog.InitialDirectory = BlackVisionPathTextBox.Text;
			if( openFileDialog.ShowDialog() == true )
			{
				if( openFileDialog.CheckFileExists )
					BlackVisionPathTextBox.Text = openFileDialog.FileName;
			}
		}

		private void Button_Click( object sender, RoutedEventArgs e )
		{
			this.DialogResult = true;
		}
	}
}
