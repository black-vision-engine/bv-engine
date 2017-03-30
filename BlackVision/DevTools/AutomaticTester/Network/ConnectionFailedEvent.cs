using RegressionLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AutomaticTester.Network
{
    public class ConnectionFailedEvent : DispatchEvent
    {
        int             ClientId;
        EventArgs       Args;

        public ConnectionFailedEvent( int clientId, EventArgs e )
        {
            ClientId = clientId;
            Args = e;
        }

        public void Execute( TesterMainLogic logic )
        {
            logic.EngineConnectionFailed( ClientId, Args );
        }
    }
}
