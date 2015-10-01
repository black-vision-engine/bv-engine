using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using System.ComponentModel;


namespace ProfilerEditor.ProfilerModel
{
	public class ProfilerTreeViewModel : INotifyPropertyChanged
	{
		private Collection<ProfilerSampleModel>		m_topLevelSamples;

#region Constructor
		public ProfilerTreeViewModel( DataProtocol.ProfilerSample[] samples, uint maxTreeExpansionLevel )
		{
			m_topLevelSamples = new Collection<ProfilerSampleModel>();

			uint curSampleIdx = 0;

			while( curSampleIdx < samples.Length && samples[ curSampleIdx ].depth == 0 )
				m_topLevelSamples.Add( new ProfilerSampleModel( null, samples, ref curSampleIdx, maxTreeExpansionLevel ) );
		}

#endregion

		public void Update( DataProtocol.ProfilerSample[] samples, uint maxTreeExpansionLevel )
		{
			uint curSampleIdx = 0;

			foreach( ProfilerSampleModel sampleModel in m_topLevelSamples )
				sampleModel.Update( samples, ref curSampleIdx, maxTreeExpansionLevel );
		}

		/**This funnction is used to update tree, on basis of data from another tree.*/
		public void Update( ProfilerTreeViewModel treeView, uint maxTreeExpansionLevel )
		{
			for( int i = 0; i < m_topLevelSamples.Count; ++i )
				m_topLevelSamples[ i ].Update( treeView.m_topLevelSamples[ i ], maxTreeExpansionLevel );
		}

		public void Average( uint numFrames )
		{
			foreach( ProfilerSampleModel sampleModel in m_topLevelSamples )
				sampleModel.Average( numFrames );
		}

		public void ClearMaxTime()
		{
			foreach( var child in m_topLevelSamples )
				child.ClearMaxTime();
		}

		public void ClearMinTime()
		{
			foreach( var child in m_topLevelSamples )
				child.ClearMinTime();
		}

		public void ClearTotalTime()
		{
			foreach( var child in m_topLevelSamples )
				child.ClearTotalTime();
		}

		public Collection<ProfilerSampleModel> TopLevelSamples
		{
			get { return m_topLevelSamples; }
		}

		public void RefreshTree()
		{
			OnPropertyChanged( "TopLevelSamples" );
			foreach( var child in m_topLevelSamples )
				child.Refresh();
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
