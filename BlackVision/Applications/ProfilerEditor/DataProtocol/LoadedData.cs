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
		public UInt16[]					m_nameIDs;
		public string[]					m_nameStrings;

		public ProfilerSample[]			m_samples;
	}
}
