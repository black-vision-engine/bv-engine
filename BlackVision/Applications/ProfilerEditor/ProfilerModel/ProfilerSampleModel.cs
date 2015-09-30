using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Collections.ObjectModel;


namespace ProfilerEditor.ProfilerModel
{
	public class ProfilerSampleModel : INotifyPropertyChanged
	{
		private Collection<ProfilerSampleModel>				m_childSamples;
		private ProfilerSampleModel							m_parent;
		private SampleData									m_sampleData;

		bool												m_isExpanded;


		public SampleData GetData() { return m_sampleData; }

#region Constructor

		// Root sample
		public ProfilerSampleModel()
		{
			m_childSamples = new Collection<ProfilerSampleModel>();
			m_sampleData = new SampleData();
			m_parent = null;

			m_sampleData.depth = 0;
			m_sampleData.durationSecs = 0;
			m_sampleData.name = 0;
		}

		public ProfilerSampleModel( ProfilerSampleModel parent, DataProtocol.ProfilerSample[] samples, ref uint curSampleIndex, uint maxTreeExpansionLevel )
		{
			m_childSamples = new Collection<ProfilerSampleModel>();
			AddSamples( parent, samples, ref curSampleIndex, maxTreeExpansionLevel );
		}

		public ProfilerSampleModel( ProfilerSampleModel parent, ProfilerSampleModel referenceSample, uint maxTreeExpansionLevel )
		{
			m_childSamples = new Collection<ProfilerSampleModel>();
			AddSamples( parent, referenceSample, maxTreeExpansionLevel );
		}

#endregion

		private void AddSamples( ProfilerSampleModel parent, DataProtocol.ProfilerSample[] samples, ref uint curSampleIndex, uint maxTreeExpansionLevel )
		{
			m_sampleData = new SampleData( samples[ curSampleIndex++ ] );
			m_parent = parent;
			if( m_sampleData.depth < maxTreeExpansionLevel )
				m_isExpanded = true;
			else
				m_isExpanded = false;

			while( curSampleIndex < samples.Length && samples[ curSampleIndex ].depth > m_sampleData.depth )
				m_childSamples.Add( new ProfilerSampleModel( this, samples, ref curSampleIndex, maxTreeExpansionLevel ) );
		}

		private void AddSamples( ProfilerSampleModel parent, ProfilerSampleModel referenceSample, uint maxTreeExpansionLevel )
		{
			m_sampleData = new SampleData( referenceSample .m_sampleData );
			m_parent = parent;
			if( m_sampleData.depth < maxTreeExpansionLevel )
				m_isExpanded = true;
			else
				m_isExpanded = false;

			foreach( var child in referenceSample.m_childSamples )
				m_childSamples.Add( new ProfilerSampleModel( this, child, maxTreeExpansionLevel ) );
		}

		public void Update( DataProtocol.ProfilerSample[] samples, ref uint curSampleIndex, uint maxTreeExpansionLevel )
		{
			m_sampleData.averageDuration += samples[ curSampleIndex++ ].durationSecs;
			if( m_sampleData.depth < maxTreeExpansionLevel )
				m_isExpanded = true;
			else
				m_isExpanded = false;

			while( curSampleIndex < samples.Length && samples[ curSampleIndex ].depth > m_sampleData.depth )
			{
				bool found = false;
				bool[] updated = new bool[ m_childSamples.Count ];
				updated = Enumerable.Repeat( false, m_childSamples.Count ).ToArray();

				for( int i = 0; i < m_childSamples.Count; ++i )
				{
					if( m_childSamples[ i ].m_sampleData.name == samples[ curSampleIndex ].name && !updated[ i ] )
					{
						m_childSamples[ i ].Update( samples, ref curSampleIndex, maxTreeExpansionLevel );

						found = true;
						updated[ i ] = true;
					}
				}

				if( !found )
					m_childSamples.Add( new ProfilerSampleModel( this, samples, ref curSampleIndex, maxTreeExpansionLevel ) );
			}
		}

		/**This funnction is used to update tree, on basis of data from another tree.*/
		public void Update( ProfilerSampleModel sampleModel, uint maxTreeExpansionLevel )
		{
			TotalDuration = sampleModel.m_sampleData.durationSecs + TotalDuration;
			AverageDuration = sampleModel.m_sampleData.averageDuration;
			if( MaxDuration < sampleModel.m_sampleData.maxDuration )
				MaxDuration = sampleModel.m_sampleData.maxDuration;
			if( MinDuration > sampleModel.m_sampleData.minDuration )
				MinDuration = sampleModel.m_sampleData.minDuration;


			if( m_sampleData.depth < maxTreeExpansionLevel )
				m_isExpanded = true;
			else
				m_isExpanded = false;

			foreach( var childSample in sampleModel.m_childSamples )
			{
				bool found = false;
				bool[] updated = new bool[ m_childSamples.Count ];
				updated = Enumerable.Repeat( false, m_childSamples.Count ).ToArray();

				for( int i = 0; i < m_childSamples.Count; ++i )
				{
					if( m_childSamples[ i ].m_sampleData.name == childSample.m_sampleData.name && !updated[ i ] )
					{
						m_childSamples[ i ].Update( childSample, maxTreeExpansionLevel );

						found = true;
						updated[ i ] = true;
					}
				}

				if( !found )
					m_childSamples.Add( new ProfilerSampleModel( this, childSample, maxTreeExpansionLevel ) );
			}
		}

		/**Use only with function Update( DataProtocol.ProfilerSample[] samples ...).
		Devides field m_sampleData.averageDuration.*/
		public void Average( uint numFrames )
		{
			m_sampleData.averageDuration = m_sampleData.averageDuration / numFrames;
			foreach( var child in m_childSamples )
				child.Average( numFrames );
		}

		public void ClearMaxTime()
		{
			MaxDuration = 0.0F;
			foreach( var child in m_childSamples )
				child.ClearMaxTime();
		}

		public void ClearMinTime()
		{
			MinDuration = float.MaxValue;
			foreach( var child in m_childSamples )
				child.ClearMinTime();
		}

		public void ClearTotalTime()
		{
			TotalDuration = 0.0F;
			foreach( var child in m_childSamples )
				child.ClearTotalTime();
		}


#region Sample Properties
		public Collection<ProfilerSampleModel> Children
		{
			get { return m_childSamples; }
		}

		public bool HasItems
		{
			get { if( m_childSamples.Count == 0 ) return false; else return true; }
		}
#endregion

		public bool IsExpanded
		{
			get { return m_isExpanded; }
			set
			{
				if( value != m_isExpanded )
				{
					m_isExpanded = value;
					this.OnPropertyChanged( "IsExpanded" );
				}

				// Expand all the way up to the root.
				if( m_isExpanded && m_parent != null )
					m_parent.IsExpanded = true;
			}
		}

		public ProfilerSampleModel Parent
		{
			get { return m_parent; }
		}

		public Int64 SampleName
		{
			get { return m_sampleData.name; }
		}
		public SampleData Sample
		{
			get { return m_sampleData; }
		}
		public float AverageDuration
		{
			get { return m_sampleData.averageDuration; }
			set
			{
				if( m_sampleData.averageDuration != value )
				{
					m_sampleData.averageDuration = value;
					OnPropertyChanged( "AverageDuration" );
				}
			}
		}
		public float TotalDuration
		{
			get { return m_sampleData.durationSecs; }
			set
			{
				if( m_sampleData.durationSecs != value )
				{
					m_sampleData.durationSecs = value;
					OnPropertyChanged( "TotalDuration" );
				}
			}
		}
		public float MaxDuration
		{
			get { return m_sampleData.maxDuration; }
			set
			{
				if( m_sampleData.maxDuration != value )
				{
					m_sampleData.maxDuration = value;
					OnPropertyChanged( "MaxDuration" );
				}
			}
		}
		public float MinDuration
		{
			get { return m_sampleData.minDuration; }
			set
			{
				if( m_sampleData.minDuration != value )
				{
					m_sampleData.minDuration = value;
					OnPropertyChanged( "MinDuration" );
				}
			}
		}

#region INotifyPropertyChanged Members

		public event PropertyChangedEventHandler PropertyChanged;

		protected virtual void OnPropertyChanged( string propertyName )
		{
			if( this.PropertyChanged != null )
				this.PropertyChanged( this, new PropertyChangedEventArgs( propertyName ) );
		}

#endregion // INotifyPropertyChanged Members

	}
}
