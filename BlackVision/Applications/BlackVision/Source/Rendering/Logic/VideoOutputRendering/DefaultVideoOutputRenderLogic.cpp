#include "DefaultVideoOutputRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Rendering/OffscreenRenderLogic.h"


namespace bv {

// *********************************
//
DefaultVideoOutputRenderLogic::DefaultVideoOutputRenderLogic   ( bool useVideoCard, bool displayVideoOutputOnPreview )
    : m_useVideoCard( useVideoCard )
    , m_displayVideoOutputOnPreview( displayVideoOutputOnPreview )
{
    m_width = 0;
    m_height = 0;
    m_curReadbackFrame = 0;
}

// *********************************
//
DefaultVideoOutputRenderLogic::~DefaultVideoOutputRenderLogic  ()
{
}

// *********************************
//
void    DefaultVideoOutputRenderLogic::FrameRendered            ( Renderer * renderer, OffscreenRenderLogic * offscreenRenderLogic )
{
    if( !m_useVideoCard )
    {
        //Not needed as it does not make sense without readback delay
        //m_offscreenRenderLogic->SwapDisplayRenderTargets();
        return;
    }

    if( m_width != (unsigned int) renderer->GetWidth() || m_height != (unsigned int) renderer->GetHeight() )
    {
        m_width = renderer->GetWidth();
        m_height = renderer->GetHeight();

        printf( "Framebuffer resolution changed to %dx%d\n", m_width, m_height );
    }

    auto frame = offscreenRenderLogic->ReadDisplayTarget( renderer, m_curReadbackFrame );
    m_curReadbackFrame = ( m_curReadbackFrame + 1 ) % offscreenRenderLogic->NumReadBuffersPerRT();

    PushToVideoCard( frame );

    offscreenRenderLogic->SwapDisplayRenderTargets();
}

// *********************************
//
void    DefaultVideoOutputRenderLogic::PushToVideoCard  ( Texture2DConstPtr & frame )
{
    // FIXME: implement
    { frame; }
}

} //bv
