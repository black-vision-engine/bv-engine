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

	std::wstring	m_pipeName;
protected:

public:
	NamedPipe();
	~NamedPipe();

	bool			ConnectToNamedPipe		( const std::wstring& pipeName, NamedPipeAccess access, unsigned int waitMillis );
	void			WriteToPipe				( const char* buffer, unsigned int bytesNum );

	bool			IsValid					();
};


} //bv
