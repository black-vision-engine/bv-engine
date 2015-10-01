using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Microsoft.Win32.SafeHandles;
using System.Runtime.InteropServices;
using System.IO;
using System.Collections;
using System.ComponentModel;
using System.Collections.ObjectModel;


namespace ProfilerEditor.DataProtocol
{
	// Helper data object to queue
	class ReadDataObject
	{
		public uint			m_bytesRead;
		public byte[]		m_data;
	}

    class NamedPipeServer
    {
		public delegate void MessageSentDelegate();

		public const uint			PIPE_ACCESS_INBOUND		= (0x00000001);
		public const uint			FILE_FLAG_OVERLAPPED	= (0x40000000);
		public const uint			PIPE_TYPE_MESSAGE		= (0x00000004);
		public const uint			PIPE_READMODE_MESSAGE	= (0x00000002);

		private uint						m_inBufferSize				= 2048;//376;
        private uint						m_outBufferSize				= 0;


        private string						m_pipeName;
        private int							m_openMode;
		private uint						m_maxConnectedClients;

        private Thread						m_listenThread;
		private Collection<Thread>			m_readThreads;
		private Semaphore					m_allHandlesSemaphore;
		private Collection<SafeFileHandle>	m_allHandles;
		private bool						m_endThreads = false;

		private SafeFileHandle				m_pipeHandle;

		private Queue						m_queue;

		public SynchronizationContext		m_syncContext;
		public SendOrPostCallback			onMessageSent;

	//Contructor
        public NamedPipeServer( string pipeName, int openMode, uint maxClients )
        {
			m_maxConnectedClients = maxClients;
            m_pipeName = pipeName;
            m_openMode = openMode;//0 Reading Pipe, 1 Writing Pipe

			// Makes queue thread-safe.
			m_queue = new Queue();
			m_queue = Queue.Synchronized( m_queue );

			m_readThreads = new Collection<Thread>();
			m_allHandlesSemaphore = new Semaphore( 1, 1 );
			m_allHandles = new Collection<SafeFileHandle>();
        }

	// Members
        public void StartServer()
        {
            this.m_listenThread = new Thread( new ThreadStart( ListenerFunction ) );
            this.m_listenThread.Start();
        }

		public void EndServer()
		{
			m_endThreads = true;

			string pipeFullName = "\\\\.\\pipe\\" + m_pipeName;
			DeleteFile( pipeFullName );
			m_listenThread.Join();


			foreach( var pipeHandle in m_allHandles )
			{
				m_allHandlesSemaphore.WaitOne();
				if( !pipeHandle.IsClosed )
					DisconnectNamedPipe( pipeHandle );
				m_allHandlesSemaphore.Release();
			}

			foreach( var thread in m_readThreads )
				thread.Join();
		}


        private void ListenerFunction()
        {
			string pipeFullName = "\\\\.\\pipe\\" + m_pipeName;

			while( !m_endThreads )
			{
				m_pipeHandle = CreateNamedPipe( pipeFullName, PIPE_ACCESS_INBOUND, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, m_maxConnectedClients, m_outBufferSize, m_inBufferSize, 0, IntPtr.Zero );

				if( m_pipeHandle.IsInvalid )
				{
					uint error = GetLastError();
					return;
				}

				int success = ConnectNamedPipe( m_pipeHandle, IntPtr.Zero );

				// Could not connect clients
				if( success == 0 || m_endThreads )
				{
					m_pipeHandle.Close();
					return;
				}

				Thread newReadThread = new Thread( new ParameterizedThreadStart( ReadThreadFunction ) );

				m_allHandlesSemaphore.WaitOne();
				m_allHandles.Add( m_pipeHandle );
				m_allHandlesSemaphore.Release();
				
				newReadThread.Start( m_pipeHandle );
				m_readThreads.Add( newReadThread );
			}
        }

		public void ReadThreadFunction( object handle )
		{
			SafeFileHandle pipeHandle = (SafeFileHandle)handle;
			FileStream pipeStream = new FileStream( pipeHandle, FileAccess.Read, (int)m_inBufferSize, false );

			// Reading form stream
			while( !m_endThreads )
			{
				uint bytesRead = 0;
				try
				{
					byte[] buffer = new byte[ m_inBufferSize ];
					bytesRead = (uint)pipeStream.Read( buffer, 0, (int)m_inBufferSize );

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
				
				// Asynchronously invokes delegate added by user of this class.
				NotifyDelegate();
			}

			pipeStream.Close();
			m_allHandlesSemaphore.WaitOne();
			if( !pipeHandle.IsClosed )
				pipeHandle.Close();
			m_allHandlesSemaphore.Release();
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


		private void NotifyDelegate()
		{
			m_syncContext.Post( onMessageSent, this );

			//EventHandler eventHandler = onMessageSent;
			//if( eventHandler != null )
			//	eventHandler.Invoke( this, null );

			//ISynchronizeInvoke sync = onMessageSent.Target as ISynchronizeInvoke;
			//if( sync == null )
			//{
			//	onMessageSent.DynamicInvoke();
			//}
			//else
			//{
			//	sync.BeginInvoke( onMessageSent, null );
			//}

			//if( m_syncObject != null && m_syncObject.InvokeRequired )
			//{
			//	ThreadStart ts = () => { onMessageSent(); };
			//	m_syncObject.BeginInvoke( ts, null );
			//}
			//else
			//{
			//	if( onMessageSent != null )
			//		onMessageSent();
			//}
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

		[DllImport( "kernel32.dll", SetLastError = true )]
		public static extern uint GetLastError();

		[DllImport( "kernel32.dll", SetLastError = true )]
		public static extern bool DeleteFile( string name );
    }
}
