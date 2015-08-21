#include "NamedPipe.h"

#include "assert.h"
#include "windows.h"

//#include "../Tools/Logger/UseLogger.h"

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
	m_pipeName = pipeName;

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
	if( m_pipeHandle == INVALID_HANDLE_VALUE )
	{
		//LOG_MESSAGE( bv::SeverityLevel::error ) << "Named Pipe: " << m_pipeName << " Message could not be sent. Invalid pipe handle.";
		return;
	}

	unsigned long int bytesWritten;

	BOOL success = WriteFile( m_pipeHandle, buffer, bytesNum, &bytesWritten, nullptr );
	assert( success == TRUE );

	//if( success != TRUE )
	//{
	//	LOG_MESSAGE( bv::SeverityLevel::error ) << "Named Pipe: " << m_pipeName << " Message could not be sent. Unalbe to write to stream.";
	//}
}

bool NamedPipe::IsValid					()
{
	return ( m_pipeHandle != INVALID_HANDLE_VALUE );
}

} //bv
