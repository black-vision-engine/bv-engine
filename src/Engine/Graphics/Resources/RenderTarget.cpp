#include "RenderTarget.h"

#include <cassert>

#include "Engine/Graphics/Resources/Texture2DImpl.h"


namespace bv {

// *********************************
//
RenderTarget::RenderTarget ( const std::vector< TextureFormat > & formats, int w, int h, bool hasDepthBuffer, bool hasMipmaps )
    : m_numTargets( formats.size() )
    , m_hasMipmaps( hasMipmaps )
    , m_hasDepthBuffer( hasDepthBuffer )
{
    assert( m_numTargets > 0 );
    assert( hasMipmaps == false ); //FIXME: to be implemented

    //int numLevels = hasMipmaps ? 0 : 1;

    for( int i = 0; i < m_numTargets; ++i )
    {
        auto tx = new Texture2DImpl( formats[ i ], w, h, DataBuffer::Semantic::S_RENDERTARGET );
        tx->AllocateMemory();
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
int RenderTarget::NumTargets () const
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
int RenderTarget::Width () const
{
    return m_ColorTextures[ 0 ]->GetWidth();
}

// *********************************
//
int RenderTarget::Height () const
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
