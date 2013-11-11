#include "RenderTarget.h"

#include <cassert>

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

// *********************************
//
RenderTarget::RenderTarget ( int numTargets, TextureFormat format, int w, int h, bool hasMipmaps )
    : m_numTargets( numTargets )
    , m_hasMipmaps( hasMipmaps )
{
    assert( numTargets > 0 );

    int numLevels = hasMipmaps ? 0 : 1;
    int i = 0;


    for( i = 0; i < numTargets; ++i )
    {
        m_ColorTextures.push_back( new Texture2D( format, w, h, DataBuffer::Semantic::S_RENDERTARGET ) );
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
Texture2D * RenderTarget::ColorTexture ( int i )
{
    return m_ColorTextures[ i ];
}

// *********************************
//
bool RenderTarget::HasMipmaps () const
{
    return m_hasMipmaps;
}

}
