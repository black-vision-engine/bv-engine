#include "PdrPBOMemTransfer.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"


namespace bv {

// ****************************
//
PdrPBOMemTransfer::PdrPBOMemTransfer    ( DataBuffer::Semantic semantic )
{
    assert( (int) DataBuffer::Semantic::S_TOTAL == 8 );
    assert( IsStreaming( semantic ) );

    m_pboTarget = PBOTarget( semantic );
    m_pboUsage  = PBOUsage( semantic );
    m_pboAccess = PBOAccess( semantic );

}

// ****************************
//
PdrPBOMemTransfer::~PdrPBOMemTransfer   ()
{
}

// ****************************
//
bool PdrPBOMemTransfer::IsStreaming     ( DataBuffer::Semantic semantic )
{
//    return semantic == DataBuffer::Semantic::S_RENDERTARGET_STREAMING_READ || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_READ || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE;
    return false;
}

// ****************************
//
GLenum  PdrPBOMemTransfer::PBOTarget    ( DataBuffer::Semantic semantic ) const
{
    assert( IsStreaming( semantic ) );
    /*
    if( semantic == DataBuffer::Semantic::S_RENDERTARGET_STREAMING_READ || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_READ )
    {
        return GL_PIXEL_PACK_BUFFER;
    }
    else if( semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE )
    {
        return GL_PIXEL_UNPACK_BUFFER;
    }
    */
    return 0;
}

// ****************************
//
GLenum  PdrPBOMemTransfer::PBOUsage     ( DataBuffer::Semantic semantic ) const
{
    assert( IsStreaming( semantic ) );
    /*
    if( semantic == DataBuffer::Semantic::S_RENDERTARGET_STREAMING_READ || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_READ )
    {
        return GL_STREAM_READ;
    }
    else if( semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE )
    {
        return GL_STREAM_DRAW;
    }
    */
    return 0;
}

// ****************************
//
GLenum  PdrPBOMemTransfer::PBOAccess    ( DataBuffer::Semantic semantic ) const
{
    /*
    if( semantic == DataBuffer::Semantic::S_RENDERTARGET_STREAMING_READ || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_READ )
    {
        return GL_READ_ONLY;
    }
    else if( semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE )
    {
        return GL_WRITE_ONLY;
    }
    */
    return 0;
}

} //bv
