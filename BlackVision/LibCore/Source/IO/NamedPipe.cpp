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
	CloseHandle( m_pipeHandle );
}

/**Connects to existing named pipe.

@param pipeName Name of pipe. Should not contain \\.\pipe\ in begining. This string will be added.*/
bool NamedPipe::ConnectToNamedPipe( const std::wstring& pipeName, NamedPipeAccess access )
{
	std::wstring pipeFullName = L"\\\\.\\pipe\\" + pipeName;

	int pipeAccess;
	if( access == NamedPipeAccess::PipeRead )
		pipeAccess = GENERIC_READ;
	else if( access == NamedPipeAccess::PipeWrite )
		pipeAccess = GENERIC_WRITE;
	else
		pipeAccess = GENERIC_READ | GENERIC_WRITE;

	m_pipeHandle = CreateFile( pipeFullName.c_str(), pipeAccess, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( m_pipeHandle == INVALID_HANDLE_VALUE )
		return false;
	return true;
}

void NamedPipe::WriteToPipe( const char* buffer, unsigned int bytesNum )
{
	unsigned long int bytesWritten;

	BOOL success = WriteFile( m_pipeHandle, buffer, bytesNum, &bytesWritten, nullptr );
	assert( success == TRUE ); { success; }
}

} //bv
