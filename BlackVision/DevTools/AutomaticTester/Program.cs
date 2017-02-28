using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Diagnostics;
using System.Globalization;
using System.IO;

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
            m_logic.TestsManager.UpdateOutputPath( options.Output );
            string exec = options.BVExecPath;
            string bvLogFile = Path.Combine( Directory.GetCurrentDirectory(), options.Output, Path.GetFileName( inputTestsDir.TrimEnd( Path.DirectorySeparatorChar ) ) ) + ".bvlog";

            m_logic.Port = options.Port;

            // Add log file
            m_logic.AddCmdLineArg( " -FileLog " + bvLogFile + " error" );



            bool validState = true;

            if( verbose )
            {
                Console.Write( "Input tests directory:              " );
                Console.WriteLine( inputTestsDir );

                Console.Write( "Output directory:                   " );
                Console.WriteLine( m_logic.TestsManager.OutputPath );

                Console.Write( "BlackVision executable:             " );
                Console.WriteLine( exec );

                Console.Write( "BlackVision IP:                     " );
                Console.WriteLine( m_logic.IpAddress );

                Console.Write( "BlackVision port:                   " );
                Console.WriteLine( m_logic.Port );
            }

            bool pathExists = m_logic.UpdateTestPath( inputTestsDir );

            if( verbose && !pathExists )
            {
                Console.WriteLine( "[Error] Path: [" + inputTestsDir + "] isn't valid directory" );
            }

            m_logic.UpdateBVExecPath( exec );

            validState = pathExists;

            if( validState )
            {
                if( verbose )
                    Console.WriteLine( "Start Testing..." );

                try
                {
                    m_logic.RunAllTests.Execute( null );
                    m_dispatcher.MainLoop();
                }
                catch( Exception e )
                {
                    if( verbose )
                        Console.WriteLine( "Exception: " + e.Message );
                }
                finally
                {
                    m_logic.KillBV();
                }

                if( verbose )
                    Console.WriteLine( "Test Ended." );
            }
            else
            {
                return 1;
            }

            return 0;
        }

    }

}
