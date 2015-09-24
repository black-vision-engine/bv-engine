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
		private ProfilerSample								m_sampleData;

		bool												m_isExpanded;


		public ProfilerSample GetData() { return m_sampleData; }

#region Constructor

		// Root sample
		public ProfilerSampleModel()
		{
			m_childSamples = new Collection<ProfilerSampleModel>();
			m_sampleData = new ProfilerSample();
			m_parent = null;

			m_sampleData.depth = 0;
			//m_sampleData.duration = 0;
			m_sampleData.durationSecs = 0;
			m_sampleData.name = 0;
		}

		public ProfilerSampleModel( ProfilerSampleModel parent, ProfilerSample[] samples, ref uint curSampleIndex, uint maxTreeExpansionLevel )
		{
			m_childSamples = new Collection<ProfilerSampleModel>();
			AddSamples( parent, samples, ref curSampleIndex, maxTreeExpansionLevel );
		}

#endregion

		private void AddSamples( ProfilerSampleModel parent, ProfilerSample[] samples, ref uint curSampleIndex, uint maxTreeExpansionLevel )
		{
			m_sampleData = samples[ curSampleIndex++ ];
			m_parent = parent;
			if( m_sampleData.depth < maxTreeExpansionLevel )
				m_isExpanded = true;
			else
				m_isExpanded = false;

			while( curSampleIndex < samples.Length && samples[ curSampleIndex ].depth > m_sampleData.depth )
				m_childSamples.Add( new ProfilerSampleModel( this, samples, ref curSampleIndex, maxTreeExpansionLevel ) );
		}

		private void UpdateSamples( ProfilerSampleModel parent, ProfilerSample[] samples, ref uint curSampleIndex, uint maxTreeExpansionLevel )
		{
			//m_sampleData = samples[ curSampleIndex++ ];

			m_sampleData.durationSecs += samples[ curSampleIndex++ ].durationSecs;
			m_parent = parent;
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
						curSampleIndex++;
						m_childSamples[ i ].UpdateSamples( this, samples, ref curSampleIndex, maxTreeExpansionLevel );

						found = true;
						updated[ i ] = true;
					}
				}

				if( !found )
					m_childSamples.Add( new ProfilerSampleModel( this, samples, ref curSampleIndex, maxTreeExpansionLevel ) );
			}
		}

		public void Update( ProfilerSample[] samples, ref uint curSampleIndex, uint maxTreeExpansionLevel )
		{
			UpdateSamples( m_parent, samples, ref curSampleIndex, maxTreeExpansionLevel );
		}

		public void Average( uint numFrames )
		{
			m_sampleData.durationSecs = m_sampleData.durationSecs / numFrames;
			foreach( var child in m_childSamples )
				child.Average( numFrames );
		}


#region Sample Properties
		public Collection<ProfilerSampleModel> Children
		{
			get { return m_childSamples; }
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

		public double DurationSecs
		{
			get { return m_sampleData.durationSecs; }
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
