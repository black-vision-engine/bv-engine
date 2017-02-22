using RegressionLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AutomaticTester.Network
{
    public class MsgReceivedEvent : DispatchEvent
    {
        String              Data;
        EventArgs           Args;


        public MsgReceivedEvent( String data, EventArgs e )
        {
            Data = data;
            Args = e;
        }

        public void Execute( TesterMainLogic logic )
        {
            logic.MsgReceived( Data, Args );
        }
    }
}
