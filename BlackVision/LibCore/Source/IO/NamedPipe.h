#pragma once

#include <string>

namespace bv{


typedef void* HANDLE;

enum NamedPipeAccess
{
	PipeRead,
	PipeWrite,
	PipeReadWrite
};


class NamedPipe
{
private:
	HANDLE			m_pipeHandle;
protected:

public:
	NamedPipe();
	~NamedPipe();

	bool			ConnectToNamedPipe		( const std::wstring& pipeName, NamedPipeAccess access );
	void			WriteToPipe				( char* buffer, unsigned int bytesNum );
};


} //bv
