using RegressionLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AutomaticTester.Network
{
    public interface DispatchEvent
    {
        void        Execute     ( TesterMainLogic logic );
    }

}
