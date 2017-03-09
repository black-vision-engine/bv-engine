using RegressionLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AutomaticTester.Network
{
    public class EngineDisconnectedEvent : DispatchEvent
    {
        int         ClientId;
        int         ErrorCode;
        String      Message;


        public EngineDisconnectedEvent( int clientId, int errorCode, String message )
        {
            ClientId = clientId;
            ErrorCode = errorCode;
            Message = message;
        }

        public void Execute( TesterMainLogic logic )
        {
            logic.EngineDisconnected( ClientId, ErrorCode, Message );
        }


    }
}
