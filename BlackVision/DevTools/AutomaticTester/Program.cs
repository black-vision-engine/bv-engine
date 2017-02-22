using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Diagnostics;
using System.Globalization;


namespace AutomaticTester
{
    class Program
    {
        RegressionLib.TesterMainLogic       m_logic;
        Network.EventDispatcher             m_dispatcher;


        static int     Main( string[] args )
        {
            // Note: Thanks to this we don't have to see polish messages in exceptions.
            if( Debugger.IsAttached )
                CultureInfo.DefaultThreadCurrentUICulture = CultureInfo.GetCultureInfo( "en-US" );


            var Program = new Program();
            var options = new Options();
            if( CommandLine.Parser.Default.ParseArguments( args, options ) )
            {
                return Program.CallTest( options );
            }
            else
            {
                Console.WriteLine( "[Error] Invalid arguments. Parsing failed." );
            }

            return 0;
        }

        private int CallTest( Options options )
        {
            m_logic = new RegressionLib.TesterMainLogic();
            m_dispatcher = new Network.EventDispatcher( m_logic );


            bool verbose = options.Verbose;
            string inputTestsDir = options.InputTestsDir;
            string exec = options.BVExecPath;



            bool validState = true;

            if( verbose )
            {
                Console.Write( "Input tests directory:              " );
                Console.WriteLine( inputTestsDir );

                Console.Write( "BlackVision executable:             " );
                Console.WriteLine( exec );
            }

            bool pathExists = m_logic.UpdateTestPath( inputTestsDir );

            if( verbose )
            {
                Console.WriteLine( "[Error] Path: [" + inputTestsDir + "] isn't valid directory" );
            }

            m_logic.UpdateBVExecPath( exec );

            validState = pathExists;

            if( validState )
            {
                m_logic.RunAllTests.Execute( null );
                m_dispatcher.MainLoop();
            }
            else
            {
                return 1;
            }

            return 0;
        }

    }

}
