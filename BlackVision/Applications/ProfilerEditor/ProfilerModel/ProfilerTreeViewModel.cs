using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;

namespace ProfilerEditor.ProfilerModel
{
	public class ProfilerTreeViewModel
	{
		private Collection<ProfilerSampleModel>				m_topLevelSamples;

#region Constructor
		public ProfilerTreeViewModel( ProfilerSample[] samples, uint maxTreeExpansionLevel )
		{
			m_topLevelSamples = new Collection<ProfilerSampleModel>();

			uint curSampleIdx = 0;

			while( curSampleIdx < samples.Length && samples[ curSampleIdx ].depth == 0 )
				m_topLevelSamples.Add( new ProfilerSampleModel( null, samples, ref curSampleIdx, maxTreeExpansionLevel ) );
		}

#endregion

		public void Update( ProfilerSample[] samples, uint maxTreeExpansionLevel )
		{
			uint curSampleIdx = 0;

			foreach( ProfilerSampleModel sampleModel in m_topLevelSamples )
				sampleModel.Update( samples, ref curSampleIdx, maxTreeExpansionLevel );
		}

		public void Average( uint numFrames )
		{
			foreach( ProfilerSampleModel sampleModel in m_topLevelSamples )
				sampleModel.Average( numFrames );
		}

		public Collection<ProfilerSampleModel> TopLevelSamples
		{
			get { return m_topLevelSamples; }
		}


	}
}
