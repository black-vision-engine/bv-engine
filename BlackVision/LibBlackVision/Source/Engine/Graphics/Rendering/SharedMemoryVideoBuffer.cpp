#include "stdafx.h"

#include "SharedMemoryVideoBuffer.h"


namespace bv {


// *********************************
//
const std::wstring    SharedMemoryVideoBuffer::MAPPING_OBJECT_NAME = L"BV";

// *********************************
//
SharedMemoryVideoBuffer::SharedMemoryVideoBuffer( UInt32 width, UInt32 height, TextureFormat format, UInt32 scaleFactor )
    : m_isAllocated( false )
    , m_pBuf( nullptr )
    , m_scaleFactor( scaleFactor )
    , m_format( format )
{
    // FIXME: rescale
    m_width = width / scaleFactor;
    m_height = height / scaleFactor;
    m_buffSize = m_width * m_height * ( UInt32 )Texture::GetPixelSize( m_format );

    m_hMapFile = CreateFileMapping( INVALID_HANDLE_VALUE,           // use paging file
                                    NULL,                           // default security
                                    PAGE_READWRITE,                 // read/write access
                                    0,                              // maximum object size (high-order DWORD)
                                    m_buffSize,                     // maximum object size (low-order DWORD)
                                    MAPPING_OBJECT_NAME.c_str() );  // name of mapping object

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
            m_data = new char[ m_buffSize ];
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
    auto data = frame->GetData()->Get();

    auto inPixelSize = ( UInt32 )frame->GetPixelSize();
    auto outPixelSize = ( UInt32 )Texture::GetPixelSize( m_format );

    assert( inPixelSize >= outPixelSize );

    //FIXME: interpolate
    for( UInt32 j = 0; j < m_height; ++j )
    {
        int y_scaled_offset = j * m_width * outPixelSize;
        int y_offset = j * m_width * ( UInt32 )pow( m_scaleFactor, 2 ) * inPixelSize;

        for( UInt32 i = 0; i < m_width; ++i )
        {
            int x_scaled_offset = i * outPixelSize;
            int x_offset = i * inPixelSize * m_scaleFactor;

            memcpy( &m_data[ y_scaled_offset + x_scaled_offset ], &data[ y_offset + x_offset ], outPixelSize );
        }
    }

    CopyMemory( ( PVOID )m_pBuf, m_data, m_buffSize * sizeof( char ) );
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