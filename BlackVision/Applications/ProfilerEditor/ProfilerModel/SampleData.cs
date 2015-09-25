using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProfilerEditor.ProfilerModel
{
	public class SampleData
	{
		public Int64 name;
		public uint depth;
		public float durationSecs;
		public float averageDuration;
		public float maxDuration;
		public float minDuration;

		public SampleData()
		{
			name = 0;
			depth = 0;
			durationSecs = 0.0F;
			averageDuration = 0.0F;
			maxDuration = 0.0F;
			minDuration = float.MaxValue;
		}

		public SampleData( ProfilerSample sample )
		{
			name = sample.name;
			depth = sample.depth;
			durationSecs = sample.durationSecs;
			averageDuration = sample.durationSecs;
			maxDuration = 0.0F;
			minDuration = float.MaxValue;
			if( maxDuration < sample.durationSecs )
				maxDuration = sample.durationSecs;
			if( minDuration > sample.durationSecs )
				minDuration = sample.durationSecs;
		}

		public SampleData( SampleData data )
		{
			name = data.name;
			depth = data.depth;
			durationSecs = data.durationSecs;
			averageDuration = data.averageDuration;
			maxDuration = data.maxDuration;
			minDuration = data.minDuration;
		}
	}
}
