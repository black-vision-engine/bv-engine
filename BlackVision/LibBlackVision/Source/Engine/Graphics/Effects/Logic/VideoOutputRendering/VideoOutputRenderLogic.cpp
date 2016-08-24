#include "stdafx.h"

#include "VideoOutputRenderLogic.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffectFactory.h"
#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "VideoCardManager.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
VideoOutputRenderLogic::VideoOutputRenderLogic          ( unsigned int height, bool interlaceOdd )
    : m_height( height )
    , m_videoOutputEffect( nullptr )
    , m_interlaceOdd( interlaceOdd )
    , m_videoFrame( nullptr )
    , m_vidTargets( 2 )
{
    m_videoOutputEffect = CreateFullscreenEffectInstance( FullscreenEffectType::FET_VIDEO_OUTPUT );
}

// *********************************
//
VideoOutputRenderLogic::~VideoOutputRenderLogic         ()
{
}

// *********************************
//
void                            VideoOutputRenderLogic::Render          ( RenderTarget * videoRenderTarget, RenderTarget * curFrameRenderTarget, RenderTarget * prevFrameRenderTarget, RenderLogicContext * ctx )
{
    auto render = renderer( ctx );

    if( !m_videoOutputEffect->GetRenderTarget( 0 ) )
    {
        //FIXME: fake initialize curFrameRenderTarget (as it will be used by the effect a moment after this)
        //FIXME: this suxx but there is an assert in the renderer and maybe it makes sense to leave it there
        render->Enable  ( prevFrameRenderTarget );
        render->Disable ( prevFrameRenderTarget );

        m_videoOutputEffect->SetRenderTarget( 0, curFrameRenderTarget );
        m_videoOutputEffect->SetRenderTarget( 1, prevFrameRenderTarget );

        m_videoOutputEffect->SetValue( "height", (float) m_height );
        m_videoOutputEffect->SetValue( "startEven", (int) !m_interlaceOdd );
    }

    m_vidTargets[ 0 ] = curFrameRenderTarget;
    m_vidTargets[ 1 ] = prevFrameRenderTarget;

    //FIXME: regquired - effect instance assumes that it is bound.... somehow deal with it in composite effects
    render->Enable    ( videoRenderTarget );

    m_videoOutputEffect->UpdateInputRenderTargets( m_vidTargets );
    m_videoOutputEffect->Render( videoRenderTarget, ctx );

    render->Disable   ( videoRenderTarget );
}

// *********************************
//
void    VideoOutputRenderLogic::VideoFrameRendered      ( RenderTarget * videoRenderTarget, RenderLogicContext * ctx )
{
    renderer( ctx )->ReadColorTexture( 0, videoRenderTarget, m_videoFrame );
    
    // FIXME: add video manager code somewhere near this piece of logic

    videocards::VideoCardManager::Instance().ProcessFrame( m_videoFrame->GetData() );

//  if( m_renderToSharedMemory )
//  {
//	    m_SharedMemoryVideoBuffer->DistributeFrame(frame);
//  }
}

// *********************************
//
void    VideoOutputRenderLogic::SetChannelMapping       ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx )
{
    if( m_videoOutputEffect )
    {
        auto val = ( ( aIdx & 0x3 ) << 6 ) | ( ( bIdx & 0x3 ) << 4 ) | ( ( gIdx & 0x3 ) << 2 ) | ( ( rIdx & 0x3 ) << 0 );

        m_videoOutputEffect->SetValue( "channelMask", val );
    }
}

// *********************************
//
void    VideoOutputRenderLogic::SetOverwriteAlpha       ( bool overwriteAlpha )
{
    if( m_videoOutputEffect )
    {
        m_videoOutputEffect->SetValue( "overwriteAlpha", overwriteAlpha );
    }
}

// *********************************
//
void    VideoOutputRenderLogic::SetAlpha                ( float alpha )
{
    if( m_videoOutputEffect )
    {
        m_videoOutputEffect->SetValue( "alpha", alpha );
    }
}

// *********************************
//
void    VideoOutputRenderLogic::SetHeight               ( int height )
{
    if( m_videoOutputEffect )
    {
        m_videoOutputEffect->SetValue( "height", height );
    }
}

// *********************************
//
Texture2DPtr    VideoOutputRenderLogic::GetLastVideoFrame  ()
{
    return m_videoFrame;
}

} //bv
