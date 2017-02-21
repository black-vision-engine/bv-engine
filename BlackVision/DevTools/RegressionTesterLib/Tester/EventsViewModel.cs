using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;


namespace RegressionLib
{
    public class EventsViewModel : INotifyPropertyChanged
    {
        Event       m_currentTestEvent;
        Event       m_currentReferenceResponse;
        Event       m_currentRealResponse;


        public EventsViewModel()
        {
            CurrentTestEvent = null;
            CurrentReferenceResponse = null;
            CurrentRealResponse = null;
        }


        public void     ErrorSelected( TestError error )
        {
            if( error != null )
            {
                CurrentTestEvent = error.EventSent;
                CurrentReferenceResponse = error.ReferenceReponse;
                CurrentRealResponse = error.ReceivedResponse;
            }
        }

        public void     SetTestEvent( Event evt )
        {
            if( evt != null )
                CurrentTestEvent = evt;
        }

        public void     SetReferenceResponse( Event evt )
        {
            if( evt != null )
                CurrentReferenceResponse = evt;
        }

        public void     SetRealResponse( Event evt )
        {
            if( evt != null )
                CurrentRealResponse = evt;
        }


        public Event    CurrentTestEvent
        {
            get
            {
                return m_currentTestEvent;
            }

            set
            {
                m_currentTestEvent = value;
                NotifyPropertyChanged( "CurrentTestEvent" );
            }
        }

        public Event    CurrentReferenceResponse
        {
            get
            {
                return m_currentReferenceResponse;
            }

            set
            {
                m_currentReferenceResponse = value;
                NotifyPropertyChanged( "CurrentReferenceResponse" );
            }
        }

        public Event    CurrentRealResponse
        {
            get
            {
                return m_currentRealResponse;
            }

            set
            {
                m_currentRealResponse = value;
                NotifyPropertyChanged( "CurrentRealResponse" );
            }
        }


        #region PropertyChangedImpl

        public event PropertyChangedEventHandler PropertyChanged;


        private void NotifyPropertyChanged( String info )
        {
            if( PropertyChanged != null )
            {
                PropertyChanged( this, new PropertyChangedEventArgs( info ) );
            }
        }

        #endregion
    }
}
