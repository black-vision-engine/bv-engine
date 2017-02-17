#include "stdafx.h"

#include "VideoInputChannel.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoResizedInputChannel.h"


namespace bv { namespace nrl {

// **************************
//
VideoInputChannel::VideoInputChannel                            ( const RenderChannel * wrappedChannel )
    : m_wrappedRenderChannel( wrappedChannel )
    , m_lastFrameHadAudio( true )
{
}

// **************************
//
VideoInputChannel::~VideoInputChannel                           ()
{
}

// **************************
//
bool            VideoInputChannel::IsActive                     () const
{
    return m_wrappedRenderChannel->IsActive();
}

// **************************
//
bool            VideoInputChannel::LastFrameHadAudio            () const
{
    return m_lastFrameHadAudio;
}

// **************************
//
void            VideoInputChannel::ToggleLastFrameHadAudio      () const
{
    m_lastFrameHadAudio = !m_lastFrameHadAudio;
}

// **************************
//
Texture2DPtr    VideoInputChannel::ReadColorTexture             ( NRenderContext * ctx ) const
{
    return m_wrappedRenderChannel->ReadColorTexture( renderer( ctx ) );
}

// **************************
//
void            VideoInputChannel::InvalidateCachedTexture      () const
{
    // Intentionally left empty
}

// **************************
//
VideoInputChannel *  VideoInputChannel::Create                  ( const RenderChannel * wrappedChannel, unsigned int width, unsigned int height )
{
    auto rt = wrappedChannel->GetActiveRenderTarget();

    auto inputWidth = rt->Width();
    auto inputHeight = rt->Height();

    VideoInputChannel * res = nullptr;

    if( width == inputWidth && height == inputHeight )
    {
        res = new VideoInputChannel( wrappedChannel );
    }
    else
    {
        res = new VideoResizedInputChannel( wrappedChannel, width, height );
    }

    return res;
}

} // nrl
} // bv
