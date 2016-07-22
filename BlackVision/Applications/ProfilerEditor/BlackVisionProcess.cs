using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;


namespace ProfilerEditor
{
    public class BlackVisionProcess
    {
        string          m_blackVisionPathName;
        Process         m_blackVisionProcess;
        int             m_killTimeOut;



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
            m_blackVisionProcess.StartInfo.FileName = BlackVisionPathName;
            m_blackVisionProcess.StartInfo.WorkingDirectory = Path.GetDirectoryName( BlackVisionPathName );
            m_blackVisionProcess.StartInfo.Arguments = commandLineArg;
            //m_blackVisionProcess.Exited

            m_blackVisionProcess.Start();
        }


        public void Kill()
        {
            Kill( m_blackVisionProcess );
            m_blackVisionProcess = null;
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
        /// Finds all running BlackVision processes and kill them. 
        /// </summary>
        public void KillEmAll()
        {
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
