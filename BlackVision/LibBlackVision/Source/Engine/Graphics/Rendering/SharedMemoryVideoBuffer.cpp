#include "stdafx.h"

#include "SharedMemoryVideoBuffer.h"


namespace bv {

// *********************************
//
SharedMemoryVideoBuffer::SharedMemoryVideoBuffer        ( UInt32 width, UInt32 height, UInt32 scaleFactor )
    : m_isAllocated( false )
    , m_pBuf( nullptr )
{
    TCHAR szName[] = TEXT( "BV" );

    m_scaleFactor = scaleFactor;
    m_width = width;// / m_scaleFactor;
    m_height = height;// / m_scaleFactor;
    m_buffSize = m_width * m_height * 3;

    m_hMapFile = CreateFileMapping( INVALID_HANDLE_VALUE,    // use paging file
                                    NULL,                    // default security
                                    PAGE_READWRITE,          // read/write access
                                    0,                       // maximum object size (high-order DWORD)
                                    m_buffSize,              // maximum object size (low-order DWORD)
                                    szName );                // name of mapping object

    if( m_hMapFile )
    {
        m_pBuf = ( LPTSTR ) MapViewOfFile( m_hMapFile,   // handle to map object
                                           FILE_MAP_ALL_ACCESS, // read/write permission
                                           0,
                                           0,
                                           m_buffSize );

        if ( m_pBuf == NULL )
        {
            printf( "Could not map view of file (%d).\n", GetLastError() );

            CloseHandle(m_hMapFile);
        }
        else
        {
            m_data = new unsigned char[ m_buffSize ];
            CopyMemory( ( PVOID )m_pBuf, m_data, m_buffSize * sizeof( unsigned char ) );
            m_isAllocated = true;
        }
    }
    else
    {
        printf( "Could not create file mapping object (%d).\n", GetLastError() );
    }
}

// *********************************
//
void        SharedMemoryVideoBuffer::DistributeFrame                            ( Texture2DConstPtr frame )
{
    auto data = ( unsigned char * )frame->GetData()->Get();  

    for( UInt32 j = 0; j < m_height; ++j )
    {
        int y_scaled_offset = j * m_width;
        //int y_offset = j * m_width * m_scaleFactor;
            
        for( UInt32 i = 0; i < m_width ; ++i )
        {
            //int x_scaled_offset = i * 3;
            int x_offset = i * 3;

            auto w = (int)data[ y_scaled_offset + x_offset + 2 ];
            w;
            if( w > 0 )
            {
                auto a = 0; a;
            }
            if( w > 1 )
            {
                auto b = 0; b;
            }
            m_data[ y_scaled_offset + x_offset + 0 ] = data[ y_scaled_offset + x_offset + 0 ];
            m_data[ y_scaled_offset + x_offset + 1 ] = data[ y_scaled_offset + x_offset + 1 ];
            m_data[ y_scaled_offset + x_offset + 2 ] = data[ y_scaled_offset + x_offset + 2 ];
        }
    }
    CopyMemory( ( PVOID )m_pBuf, m_data, m_buffSize * sizeof( unsigned char ) );
}

// *********************************
//
SharedMemoryVideoBuffer::~SharedMemoryVideoBuffer()
{
    if( m_isAllocated )
    {
        UnmapViewOfFile( m_pBuf );
        CloseHandle( m_hMapFile );
    }
}

} //bv