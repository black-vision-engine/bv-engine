using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
using System.Windows.Media;

namespace ProfilerEditor.ProfilerModel
{
	public class NameMapping
	{
		Dictionary<UInt16, string>			m_namesMap;
		Dictionary<UInt16, Color>			m_colorMap;

		Random								m_randomGenerator;

		public NameMapping()
		{
			m_namesMap = new Dictionary<UInt16, string>();
			m_colorMap = new Dictionary<UInt16, Color>();

			m_randomGenerator = new Random();
		}

		public void Update( ProfilerEditor.DataProtocol.LoadedData loadedData )
		{
			for( int i = 0; i < loadedData.m_nameIDs.Length; i++ )
				if( !m_colorMap.ContainsKey( loadedData.m_nameIDs[ i ] ) )
				{
					Color randomColor = Color.FromArgb( 255, (byte)m_randomGenerator.Next( 255 ), (byte)m_randomGenerator.Next( 255 ), (byte)m_randomGenerator.Next( 255 ) );
					m_colorMap.Add( loadedData.m_nameIDs[ i ], randomColor );
					m_namesMap.Add( loadedData.m_nameIDs[ i ], loadedData.m_nameStrings[ i ] );
				}
				
		}

		public Color GetColorOf( UInt16 nameID )
		{
			Color retColor = new Color();
			if( m_colorMap.TryGetValue( nameID, out retColor ) )
				return retColor;
			else
				return Color.FromArgb( 255, (byte)m_randomGenerator.Next( 255 ), (byte)m_randomGenerator.Next( 255 ), (byte)m_randomGenerator.Next( 255 ) );
		}

		public string GetStringOf( UInt16 nameID )
		{
			string retString;
			if( m_namesMap.TryGetValue( nameID, out retString ) )
				return retString;
			else
				return "Name not sent";
		}
	}
}
