#include "stdafx.h"

#include "RenderTarget.h"

#include <cassert>

#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "Memory/MemoryChunk.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
RenderTarget::RenderTarget ( const std::vector< TextureFormat > & formats, UInt32 w, UInt32 h, bool hasDepthBuffer, const std::vector< UInt32 > & levels, RenderTarget::RTSemantic semantic )
    : m_numTargets( (UInt32) formats.size() )
    , m_hasDepthBuffer( hasDepthBuffer )
    , m_semantic( semantic )
{
    assert( m_numTargets > 0 );

    //int numLevels = hasMipmaps ? 0 : 1;

    for( SizeType i = 0; i < m_numTargets; ++i )
    {
        auto tx = std::make_shared< Texture2D >( formats[ i ], w, h, DataBuffer::Semantic::S_TEXTURE_STATIC, levels.size() > i ? levels[ i ] : 1 );
        //tx->SetData( MemoryChunk::EMPTY(), formats[ i ], w, h ); //FIXME: empty pointer (this memory was never used as it is supposed only to serve as a key for Renderer).
        m_ColorTextures.push_back( tx );    
    }
}

// *********************************
//
RenderTarget::~RenderTarget ()
{
    //FIXME: unbind all resources associated with this one
    //Renderer::UnbindAll(this);
}

// *********************************
//
RenderTarget::RTSemantic  RenderTarget::Semantic  () const
{
    return m_semantic;
}

// *********************************
//
UInt32  RenderTarget::NumTargets () const
{
    return (unsigned int) m_numTargets;
}

// *********************************
//
TextureFormat RenderTarget::Format () const
{
    return m_ColorTextures[ 0 ]->GetFormat();
}

// *********************************
//
UInt32  RenderTarget::Width () const
{
    return m_ColorTextures[ 0 ]->GetWidth();
}

// *********************************
//
UInt32  RenderTarget::Height () const
{
    return m_ColorTextures[ 0 ]->GetHeight();
}

// *********************************
//
Texture2DPtr RenderTarget::ColorTexture ( UInt32 i ) const
{
    return m_ColorTextures[ i ];
}

// *********************************
//
bool RenderTarget::HasMipmaps ( UInt32 i ) const
{
	return ColorTexture( i )->HasMipmaps();
}

// *********************************
//
bool RenderTarget::HasDepthBuffer () const
{
    return m_hasDepthBuffer;
}

} //bv
