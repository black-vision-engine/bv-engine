using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AutomaticTester.Network
{
    public class EventDispatcher
    {
        RegressionLib.TesterMainLogic           m_testerLogic;
        BlockingCollection< DispatchEvent >     m_messages;


        public EventDispatcher( RegressionLib.TesterMainLogic tester )
        {
            m_testerLogic = tester;
            m_messages = new BlockingCollection<DispatchEvent>();

            m_testerLogic.m_network.Connected += EngineConnected;
            m_testerLogic.m_network.ExceptionRaised += EngineDisconnected;
            m_testerLogic.m_network.ConnectionFailed += EngineConnectionFailed;
            m_testerLogic.m_network.Received += MsgReceived;
        }




        public void MainLoop    ()
        {

            foreach( var item in m_messages.GetConsumingEnumerable() )
            {
                // Do the "work" on the item
                item.Execute( m_testerLogic );

                // If tester logic is in init state, all tests have ended.
                if( m_testerLogic.IsInitState( null ) )
                    return;
            }
        }



        void EngineConnected( int ClientId, EventArgs e )
        {
            m_messages.Add( new Network.EngineConnectedEvent( ClientId, e ) );
        }

        void EngineConnectionFailed( int ClientId, EventArgs e )
        {
            m_messages.Add( new Network.ConnectionFailedEvent( ClientId, e ) );
        }

        void EngineDisconnected( int ClientId, int ErrorCode, String Message )
        {
            m_messages.Add( new Network.EngineDisconnectedEvent( ClientId, ErrorCode, Message ) );
        }

        void MsgReceived( String data, EventArgs e )
        {
            m_messages.Add( new Network.MsgReceivedEvent( data, e ) );
        }
    }
}
