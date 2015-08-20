using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Microsoft.Win32.SafeHandles;
using System.Runtime.InteropServices;
using System.IO;
using System.Collections;

namespace ProfilerEditor
{

	// Helper data object to queue
	class ReadDataObject
	{
		public uint			m_bytesRead;
		public byte[]		m_data;
	}

    class NamedPipeServer
    {
		public const uint PIPE_ACCESS_INBOUND = (0x00000001);
		public const uint FILE_FLAG_OVERLAPPED = (0x40000000);

        private uint				m_inBufferSize				= 5000;
        private uint				m_outBufferSize				= 0;


        private string				m_pipeName;
        private int                 m_openMode;

        private Thread				m_listenThread;
		private bool				m_endThread = false;

		private SafeFileHandle		m_pipeHandle;
		private FileStream			m_pipeStream;

		private Queue				m_queue;


	//Contructor
        public NamedPipeServer( string pipeName, int openMode )
        {
            m_pipeName = pipeName;
            m_openMode = openMode;//0 Reading Pipe, 1 Writing Pipe

			// Makes queue thread-safe.
			m_queue = new Queue();
			m_queue = Queue.Synchronized( m_queue );
        }

	// Members
        public void StartServer()
        {
            this.m_listenThread = new Thread( new ThreadStart( ListenerFunction ) );
            this.m_listenThread.Start();
        }

		public void EndServer()
		{
			m_endThread = true;

			DisconnectNamedPipe( m_pipeHandle );
			m_listenThread.Join();
		}


        private void ListenerFunction()
        {
			string pipeFullName = "\\.\\pipe\\" + m_pipeName;

			m_pipeHandle = CreateNamedPipe( pipeFullName, PIPE_ACCESS_INBOUND, FILE_FLAG_OVERLAPPED, 0, 1, m_outBufferSize, m_inBufferSize, IntPtr.Zero );

			if( m_pipeHandle.IsInvalid )
				return ;

			m_pipeStream = new FileStream( m_pipeHandle, FileAccess.Read, (int)m_inBufferSize, true );

			// Reading form stream
			while( !m_endThread )
			{
				uint bytesRead = 0;
				try
				{
					byte[] buffer = new byte[ m_inBufferSize ];
					bytesRead = (uint)m_pipeStream.Read( buffer, 0, (int)m_inBufferSize );

					ReadDataObject readBuffer = new ReadDataObject();
					readBuffer.m_data = buffer;
					readBuffer.m_bytesRead = bytesRead;

					m_queue.Enqueue( readBuffer );
				}
				catch
				{
					// Just for now. @todo Error handling here.
					break;
				}

				//client has disconnected
				if( bytesRead == 0 )
					break;				// @todo Better error handling here.
			}

			m_pipeStream.Close();
        }

		/**Reads last message from queue. [thread-safe]*/
		public ReadDataObject ReadBytes()
		{
			if( m_queue.Count == 0 )
			{
				ReadDataObject emptyData = new ReadDataObject();
				emptyData.m_bytesRead = 0;
				emptyData.m_data = null;
				return emptyData;
			}

			ReadDataObject message = (ReadDataObject)m_queue.Dequeue();
			return message;
		}

		/**Writes message to stream. [unused for now]*/
		public bool WriteBytes()
		{
			return false;
		}





    // Loaded fucntions
        [DllImport( "kernel32.dll", SetLastError = true )]
        public static extern SafeFileHandle CreateNamedPipe(
           String pipeName,
           uint dwOpenMode,
           uint dwPipeMode,
           uint nMaxInstances,
           uint nOutBufferSize,
           uint nInBufferSize,
           uint nDefaultTimeOut,
           IntPtr lpSecurityAttributes );

        [DllImport( "kernel32.dll", SetLastError = true )]
        public static extern int ConnectNamedPipe(
           SafeFileHandle hNamedPipe,
           IntPtr lpOverlapped );

        [DllImport( "kernel32.dll", SetLastError = true )]
        public static extern int DisconnectNamedPipe(
           SafeFileHandle hNamedPipe );
    }
}
