#include "stdafxCore.h"

#include "SharedMemoryBuffer.h"

#include "UseLoggerLibCoreModule.h"


namespace bv {

// *********************************
//
SharedMemoryBuffer::SharedMemoryBuffer                          ( HANDLE hMapFile, LPTSTR pBuf, UInt32 bufferSize )
    : m_hMapFile( hMapFile )
    , m_pBuf( ( LPTSTR ) pBuf )
    , m_bufferSize( bufferSize )
{
}

// *********************************
//
SharedMemoryBuffer::~SharedMemoryBuffer                         ()
{
    UnmapViewOfFile( m_pBuf );
    CloseHandle( m_hMapFile );    
}

// *********************************
//
UInt32                  SharedMemoryBuffer::GetBufferSize       () const
{
    return m_bufferSize;
}

// *********************************
//
void                    SharedMemoryBuffer::Write               ( const char * data, UInt32 size )
{
    assert( size <= m_bufferSize );

    CopyMemory( (PVOID) m_pBuf, data, size );
}

// *********************************
//
SharedMemoryBufferPtr   SharedMemoryBuffer::Create              ( const std::wstring & name, UInt32 bufferSize )
{
    SharedMemoryBufferPtr res   = nullptr;

    HANDLE          hMapFile    = nullptr;
    LPTSTR          pBuf        = nullptr;

    hMapFile = CreateFileMapping( INVALID_HANDLE_VALUE,           // use paging file
                                  NULL,                           // default security
                                  PAGE_READWRITE,                 // read/write access
                                  0,                              // maximum object size (high-order DWORD)
                                  bufferSize,                     // maximum object size (low-order DWORD)
                                  name.c_str() );                 // name of mapping object

    if( hMapFile )
    {
        pBuf = ( LPTSTR ) MapViewOfFile( hMapFile,            // handle to map object
                                         FILE_MAP_ALL_ACCESS, // read/write permission
                                         0,
                                         0,
                                         bufferSize );

        if ( pBuf == NULL )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "Could not map view of file [" << name << "], error [" << GetLastError() << "].";

            CloseHandle( hMapFile );
        }
        else
        {
            res = SharedMemoryBufferPtr( new SharedMemoryBuffer( hMapFile, pBuf, bufferSize ) );
        }
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Could not create file mapping object [" << name << "], error [" << GetLastError() << "].";
    }

    return res;
}

} //bv
