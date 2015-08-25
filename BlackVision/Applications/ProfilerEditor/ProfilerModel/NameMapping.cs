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
		Dictionary<Int64, string>			m_namesMap;
		Dictionary<Int64, Color>			m_colorMap;

		Random								m_randomGenerator;

		public NameMapping()
		{
			m_namesMap = new Dictionary<Int64, string>();
			m_colorMap = new Dictionary<Int64, Color>();

			m_randomGenerator = new Random();
		}

		public void update( ProfilerSample[] samples )
		{
			foreach( var sample in samples )
				if( !m_colorMap.ContainsKey( sample.name ) )
				{
					Color randomColor = Color.FromArgb( 255, (byte)m_randomGenerator.Next( 255 ), (byte)m_randomGenerator.Next( 255 ), (byte)m_randomGenerator.Next( 255 ) );
					m_colorMap.Add( sample.name, randomColor );
				}
				
		}
	}
}
