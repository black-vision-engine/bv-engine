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
    renderer->Enable    ( videoRenderTarget );

    auto effect = AccessEffect( curFrameRenderTarget, prevFrameRenderTarget );
    effect->Render( renderer );

    renderer->Disable   ( videoRenderTarget );
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
