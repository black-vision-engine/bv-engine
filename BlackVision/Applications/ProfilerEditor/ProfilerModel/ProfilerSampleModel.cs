using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Collections.ObjectModel;

namespace ProfilerEditor.ProfilerModel
{
	class ProfilerSampleModel : INotifyPropertyChanged
	{
		private Collection<ProfilerSampleModel>				m_childSamples;
		private ProfilerSampleModel							m_parent;
		private ProfilerSample								m_sampleData;

		bool												m_isExpanded;




#region Constructor

		// Root sample
		public ProfilerSampleModel()
		{
			m_childSamples = new Collection<ProfilerSampleModel>();
			m_sampleData = new ProfilerSample();
			m_parent = null;

			m_sampleData.depth = 0;
			m_sampleData.duration = 0;
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

		public void Update( ProfilerSample[] samples, ref uint curSampleIndex, uint maxTreeExpansionLevel )
		{
			m_childSamples.Clear();

			AddSamples( m_parent, samples, ref curSampleIndex, maxTreeExpansionLevel );
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
