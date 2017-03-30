using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;


namespace Tools
{
    public class BlackVisionProcess
    {
        string          m_blackVisionPathName;
        Process         m_blackVisionProcess;
        int             m_killTimeOut;


        public delegate void ProcessExitedDelegate();

        public ProcessExitedDelegate       BVCrashed;


        public BlackVisionProcess()
        {
            m_blackVisionProcess = null;
            BlackVisionPathName = "..\\BlackVision\\BlackVision.exe";
            BlackVisionPathName = Path.GetFullPath( BlackVisionPathName );

            KillTimeOut = 5000;
        }


        public void Start( string commandLineArg )
        {
            if( !File.Exists( BlackVisionPathName ) )
                return;

            m_blackVisionProcess = new Process();
            m_blackVisionProcess.StartInfo.FileName = "BlackVision.exe";
            m_blackVisionProcess.StartInfo.WorkingDirectory = Path.GetDirectoryName( BlackVisionPathName );
            m_blackVisionProcess.StartInfo.Arguments = commandLineArg;
            m_blackVisionProcess.EnableRaisingEvents = true;
            m_blackVisionProcess.StartInfo.ErrorDialog = false;
            m_blackVisionProcess.Exited += ProcessExited;

            m_blackVisionProcess.Start();
        }

        public bool IsSpawned()
        {
            if( m_blackVisionProcess != null )
                return true;
            return false;
        }


        public bool IsResponsive()
        {
            m_blackVisionProcess.Refresh();
            return m_blackVisionProcess.Responding;
        }

        private void ProcessExited( object sender, EventArgs e )
        {
            if( m_blackVisionProcess.ExitCode != 0 )
            {
                BVCrashed();
            }
        }

        public void Kill()
        {
            if( m_blackVisionProcess != null )
            {
                m_blackVisionProcess.Exited -= ProcessExited;

                Kill( m_blackVisionProcess );

                m_blackVisionProcess = null;
            }
        }

        private void Kill( Process process )
        {
            if( process != null )
            {
                if( !process.HasExited )
                {
                    process.CloseMainWindow();
                    if( !process.WaitForExit( KillTimeOut ) )
                    {
                        process.Kill();
                    }
                }
            }
        }

        /// <summary>
        /// Finds all running BlackVision processes and kills them. 
        /// </summary>
        public void KillEmAll()
        {
            // Kill process spawned by this class.
            Kill();

            // Kill rest BV processes.
            string processName = Path.GetFileNameWithoutExtension( BlackVisionPathName );
            Process[] processes = Process.GetProcessesByName( processName );
            foreach( var process in processes )
            {
                Kill( process );
            }
        }

        #region Properties


        public string BlackVisionPathName
        {
            get
            {
                return m_blackVisionPathName;
            }

            set
            {
                m_blackVisionPathName = value;
            }
        }


        public int KillTimeOut
        {
            get
            {
                return m_killTimeOut;
            }

            set
            {
                m_killTimeOut = value;
            }
        }

        #endregion
    }
}
