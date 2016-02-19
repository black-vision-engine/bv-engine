#include "NamedPipe.h"

#include "assert.h"
#include "windows.h"

namespace bv {


NamedPipe::NamedPipe()
{
	m_pipeHandle = nullptr;
}


NamedPipe::~NamedPipe()
{
	if( m_pipeHandle != nullptr )
		CloseHandle( m_pipeHandle );
}

/**Connects to existing named pipe.

@param pipeName Name of pipe. Should not contain \\.\pipe\ in begining. This string will be added.*/
bool NamedPipe::ConnectToNamedPipe( const std::wstring& pipeName, NamedPipeAccess access, unsigned int waitMillis )
{
	std::wstring pipeFullName = L"\\\\.\\pipe\\" + pipeName;
	m_pipeName = pipeName;

	int pipeAccess;
	if( access == NamedPipeAccess::PipeRead )
		pipeAccess = GENERIC_READ;
	else if( access == NamedPipeAccess::PipeWrite )
		pipeAccess = GENERIC_WRITE;
	else
		pipeAccess = GENERIC_READ | GENERIC_WRITE;

	WaitNamedPipe( pipeFullName.c_str(), waitMillis );
	m_pipeHandle = CreateFile( pipeFullName.c_str(), pipeAccess, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( m_pipeHandle == INVALID_HANDLE_VALUE )
	{
		DWORD error = GetLastError();
		{ error; }
		return false;
	}
	
	DWORD mode = PIPE_READMODE_MESSAGE;
	SetNamedPipeHandleState( m_pipeHandle, &mode, nullptr, nullptr );

	return true;
}

/**Funciotn writes message from buffer to pipe, if connection is valid.
If connection has broken, this function sets pipe handle to invalid state and returns.*/
void NamedPipe::WriteToPipe( const char* buffer, unsigned int bytesNum )
{
	if( !IsValid() )
		return;

	unsigned long int bytesWritten;

	BOOL success = WriteFile( m_pipeHandle, buffer, bytesNum, &bytesWritten, nullptr );
	if( success != TRUE )
	{
		m_pipeHandle = INVALID_HANDLE_VALUE;
		// Uncomment if you want to know, that sth is wrong with pipe.
		// In normal work, application shouldn't break, even if writing to pipe failed.
		//assert( success == TRUE );
	}
}

bool NamedPipe::IsValid					()
{
	return ( m_pipeHandle != INVALID_HANDLE_VALUE );
}

} //bv
