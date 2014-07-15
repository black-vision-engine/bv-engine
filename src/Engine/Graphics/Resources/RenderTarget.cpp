#include "RenderTarget.h"

#include <cassert>

#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Core/MemoryChunk.h"


namespace bv {

// *********************************
//
RenderTarget::RenderTarget ( const std::vector< TextureFormat > & formats, unsigned int w, unsigned int h, bool hasDepthBuffer, bool hasMipmaps, RenderTarget::RTSemantic semantic )
    : m_numTargets( formats.size() )
    , m_hasMipmaps( hasMipmaps )
    , m_hasDepthBuffer( hasDepthBuffer )
    , m_semantic( semantic )
{
    assert( m_numTargets > 0 );
    assert( hasMipmaps == false ); //FIXME: to be implemented

    //int numLevels = hasMipmaps ? 0 : 1;

    for( int i = 0; i < m_numTargets; ++i )
    {
        MemoryChunkPtr emptyChunk = MemoryChunkPtr( new MemoryChunk( nullptr, 0 ) );

        auto tx = new Texture2DImpl( formats[ i ], w, h, DataBuffer::Semantic::S_TEXTURE_STATIC );
        tx->SetRawData( emptyChunk, formats[ i ], w, h ); //FIXME: empty pointer (this memory was never used as it is supposed only to serve as a key for Renderer).
        tx->SetChanged( false );
        m_ColorTextures.push_back( tx );    
    }
}

// *********************************
//
RenderTarget::~RenderTarget ()
{
    //FIXME: unbind all resources associated with this one
    //Renderer::UnbindAll(this);

    for( auto ptrTx : m_ColorTextures )
    {
        delete ptrTx;
    }
}

// *********************************
//
RenderTarget::RTSemantic  RenderTarget::Semantic  () const
{
    return m_semantic;
}

// *********************************
//
unsigned int RenderTarget::NumTargets () const
{
    return m_numTargets;
}

// *********************************
//
TextureFormat RenderTarget::Format () const
{
    return m_ColorTextures[ 0 ]->GetFormat();
}

// *********************************
//
unsigned int RenderTarget::Width () const
{
    return m_ColorTextures[ 0 ]->GetWidth();
}

// *********************************
//
unsigned int RenderTarget::Height () const
{
    return m_ColorTextures[ 0 ]->GetHeight();
}

// *********************************
//
Texture2D * RenderTarget::ColorTexture ( int i ) const
{
    return m_ColorTextures[ i ];
}

// *********************************
//
bool RenderTarget::HasMipmaps () const
{
    return m_hasMipmaps;
}

// *********************************
//
bool RenderTarget::HasDepthBuffer () const
{
    return m_hasDepthBuffer;
}

} //bv
