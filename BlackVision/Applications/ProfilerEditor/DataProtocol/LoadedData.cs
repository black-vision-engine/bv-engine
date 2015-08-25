using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProfilerEditor.DataProtocol
{
	public class LoadedData
	{
		public UInt16					m_threadID;
		public string[]					m_nameStrings;
		public UInt16[]					m_nameIDs;

		public ProfilerSample[]			m_samples;
	}
}
