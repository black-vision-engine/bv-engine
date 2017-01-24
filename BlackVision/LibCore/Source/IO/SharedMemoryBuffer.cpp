#include "stdafxCore.h"

#include "SharedMemoryBuffer.h"


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
            printf( "Could not map view of file (%d).\n", GetLastError() );

            CloseHandle( hMapFile );
        }
        else
        {
            res = SharedMemoryBufferPtr( new SharedMemoryBuffer( hMapFile, pBuf, bufferSize ) );
        }
    }
    else
    {
        printf( "Could not create file mapping object (%d).\n", GetLastError() );
    }

    return res;
}

} //bv
