#include "stdafx.h"

#include "Tools/Profiler/HerarchicalProfiler.h"
#include "SharedMemoryVideoBuffer.h"


namespace bv {

// *********************************
// FIXME: this suxx - should be read from a global file with static names for all such entities
const std::wstring    SharedMemoryVideoBuffer::MAPPING_OBJECT_NAME = L"BV";

// *********************************
//
SharedMemoryVideoBuffer::SharedMemoryVideoBuffer                ( SharedMemoryBufferPtr buf )
    : m_buf( buf )
{    
}

// *********************************
//
void                        SharedMemoryVideoBuffer::PushFrame  ( Texture2DConstPtr frame )
{
	auto data = frame->GetData()->Get();

    unsigned int dataSize = frame->GetHeight() * frame->GetWidth() * ( unsigned int ) frame->GetPixelSize();

    m_buf->Write( data, dataSize );
}

// *********************************
//
SharedMemoryVideoBufferPtr  SharedMemoryVideoBuffer::Create     ( UInt32 width, UInt32 height, TextureFormat format )
{
    return Create( MAPPING_OBJECT_NAME, width, height, format );
}

// ***********************
//
SharedMemoryVideoBufferPtr  SharedMemoryVideoBuffer::Create     ( const std::wstring & name, UInt32 width, UInt32 height, TextureFormat format )
{
    SharedMemoryVideoBufferPtr result = nullptr;

    auto buffSize = width * height * ( UInt32 )Texture::GetPixelSize( format );

    auto buf = SharedMemoryBuffer::Create( name, buffSize );

    if( buf )
    {
        result = SharedMemoryVideoBufferPtr( new SharedMemoryVideoBuffer( buf ) );
    }

    return result;
}

} //bv
