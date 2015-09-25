using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;

namespace ProfilerEditor.DataAnalysis
{
	public class AverageSamples
	{
		private DataProtocol.LoadedData[]					m_waitingFrames;
		private uint										m_numWaitingFrames;
		private const uint									m_maxWaitingFrames = 200;

		public AverageSamples()
		{
			m_waitingFrames = new DataProtocol.LoadedData[ m_maxWaitingFrames ];
			m_numWaitingFrames = 0;
		}

		public ProfilerModel.ProfilerTreeViewModel AddNewData( DataProtocol.LoadedData loadedData )
		{
			m_waitingFrames[ m_numWaitingFrames++ ] = loadedData;
			if( m_numWaitingFrames < m_maxWaitingFrames )
				return null;
			
			m_numWaitingFrames = 0;
			return ProcessData();
		}

		private ProfilerModel.ProfilerTreeViewModel ProcessData()
		{
			ProfilerModel.ProfilerTreeViewModel profilerTreeView = new ProfilerModel.ProfilerTreeViewModel( m_waitingFrames[ 0 ].m_samples, (uint)10 );

			for( uint i = 1; i < m_maxWaitingFrames; ++i )
				profilerTreeView.Update( m_waitingFrames[ i ].m_samples, (uint)10 );
			profilerTreeView.Average( m_maxWaitingFrames );

			return profilerTreeView;
		}

	}
}
