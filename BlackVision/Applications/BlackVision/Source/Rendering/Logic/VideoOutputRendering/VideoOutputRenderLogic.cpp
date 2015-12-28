#include "VideoOutputRenderLogic.h"

#include "Rendering/Logic/FullScreen/Impl/VideoOutputFullscreenEffect.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv {

// *********************************
//
VideoOutputRenderLogic::VideoOutputRenderLogic          ( unsigned int height, bool interlaceOdd )
    : m_height( height )
    , m_effect( nullptr )
    , m_interlaceOdd( interlaceOdd )
    , m_videoFrame( nullptr )
{
}

// *********************************
//
VideoOutputRenderLogic::~VideoOutputRenderLogic         ()
{
}

// *********************************
//
void                            VideoOutputRenderLogic::Render          ( Renderer * renderer, RenderTarget * videoRenderTarget, RenderTarget * curFrameRenderTarget, RenderTarget * prevFrameRenderTarget )
{
    if( !m_effect )
    {
        //FIXME: fake initialize curFrameRenderTarget (as it will be used by the effect a moment after this)
        //FIXME: this suxx but there is an assert in the renderer and maybe it makes sense to leave it there
        renderer->Enable( prevFrameRenderTarget );
        renderer->Disable( prevFrameRenderTarget );
    }

    renderer->Enable    ( videoRenderTarget );

    auto effect = AccessEffect( curFrameRenderTarget, prevFrameRenderTarget );
    effect->Render( renderer );

    effect->SwapTextures();

    renderer->Disable   ( videoRenderTarget );
}

// *********************************
//
void    VideoOutputRenderLogic::VideoFrameRendered      ( Renderer * renderer, RenderTarget * videoRenderTarget )
{
    renderer->ReadColorTexture( 0, videoRenderTarget, m_videoFrame );
    
    // FIXME: add video manager code somewhere near this piece of logic
}

// *********************************
//
void    VideoOutputRenderLogic::SetChannelMapping       ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx )
{
    if( m_effect )
    {
        m_effect->SetRIdx( rIdx );
        m_effect->SetGIdx( gIdx );
        m_effect->SetBIdx( bIdx );
        m_effect->SetAIdx( aIdx );
    }
}

// *********************************
//
void    VideoOutputRenderLogic::SetOverwriteAlpha       ( bool overwriteAlpha )
{
    if( m_effect )
    {
        m_effect->SetOverwriteAlpha( overwriteAlpha );
    }
}

// *********************************
//
void    VideoOutputRenderLogic::SetAlpha                ( float alpha )
{
    if( m_effect )
    {
        m_effect->SetAlpha( alpha );
    }
}

// *********************************
//
void    VideoOutputRenderLogic::SetHeight               ( int height )
{
    if( m_effect )
    {
        m_effect->SetHeight( height );
    }
}

// *********************************
//
VideoOutputFullscreenEffect *   VideoOutputRenderLogic::AccessEffect    ( RenderTarget * curFrameRenderTarget, RenderTarget * prevFrameRenderTarget )
{
    if ( !m_effect )
    {
        auto tex0 = curFrameRenderTarget->ColorTexture( 0 );
        auto tex1 = prevFrameRenderTarget->ColorTexture( 0 );

        m_effect = new VideoOutputFullscreenEffect( tex0, tex1 );
    
        m_effect->SetHeight( m_height );
        m_effect->SetStartEven( !m_interlaceOdd );
    }

    return m_effect;        
}

} //bv
