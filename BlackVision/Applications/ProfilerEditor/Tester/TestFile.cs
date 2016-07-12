using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;

namespace ProfilerEditor.Tester
{
    public class TestFile
    {
        public string FileName { get; set; }

        private ObservableCollection< Event >   m_testEvents;
        private ObservableCollection< Event >   m_referenceResponses;
        private ObservableCollection< Event >   m_realResponses;



        public TestFile()
        {
            m_testEvents = new ObservableCollection< Event >();
            m_referenceResponses = new ObservableCollection< Event >();
            m_realResponses = new ObservableCollection< Event >();
        }


        #region Properties
        public ObservableCollection<Event> TestEvents
        {
            get
            {
                return m_testEvents;
            }

            set
            {
                m_testEvents = value;
            }
        }

        public ObservableCollection<Event> ReferenceResponses
        {
            get
            {
                return m_referenceResponses;
            }

            set
            {
                m_referenceResponses = value;
            }
        }

        public ObservableCollection<Event> RealResponses
        {
            get
            {
                return m_realResponses;
            }

            set
            {
                m_realResponses = value;
            }
        }

    #endregion
    }
}
