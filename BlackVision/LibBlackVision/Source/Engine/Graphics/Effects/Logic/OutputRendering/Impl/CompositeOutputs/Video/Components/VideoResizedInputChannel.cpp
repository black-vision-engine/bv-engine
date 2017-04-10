#include "stdafx.h"

#include "VideoResizedInputChannel.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"


namespace bv { namespace nrl {

// **************************
//
VideoResizedInputChannel::VideoResizedInputChannel  ( const RenderChannel * wrappedChannel, unsigned int width, unsigned int height )
    : VideoInputChannel( wrappedChannel )
    , m_width( width )
    , m_height( height )
    , m_activeRenderOutput( 1 )
    , m_blitEffect( nullptr )
    , m_videoRT( nullptr )
    , m_cachedReadbackUpToDate( false )
{
    m_blitEffect = CreateFullscreenEffect( FullscreenEffectType::NFET_SIMPLE_BLIT );
}

// **************************
//
VideoResizedInputChannel::~VideoResizedInputChannel ()
{
    delete m_blitEffect;
    delete m_videoRT;
}

// **************************
//
Texture2DPtr    VideoResizedInputChannel::ReadColorTexture          ( RenderContext * ctx ) const
{
    if( !m_cachedReadbackUpToDate )
    {
        if( m_videoRT == nullptr )
        {
            m_videoRT = allocator( ctx )->CreateCustomRenderTarget( m_width, m_height, RenderTarget::RTSemantic::S_DRAW_READ );
        }

        auto inputRenderTarget = m_wrappedRenderChannel->GetActiveRenderTarget();

        m_activeRenderOutput.SetEntry( 0, inputRenderTarget );
        m_blitEffect->Render( ctx, m_videoRT, m_activeRenderOutput );

        renderer( ctx )->ReadColorTexture( 0, m_videoRT, m_cachedReadbackTexture );

        m_cachedReadbackUpToDate = true;
    }

    return m_cachedReadbackTexture;
}

// **************************
//
void            VideoResizedInputChannel::InvalidateCachedTexture   () const
{
    m_cachedReadbackUpToDate = false;
}

} // nrl
} // bv
