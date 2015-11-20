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
void    DefaultVideoOutputRenderLogic::FrameRenderedNewImpl            ( Renderer * renderer, OffscreenRenderLogic * offscreenRenderLogic, videocards::VideoCardManager * videoCardManager )
{
    { renderer; }
    { offscreenRenderLogic; }

    // FIXME: default behavior - show rendered preview (render to current display)
    offscreenRenderLogic->DrawDisplayRenderTarget( renderer );

    if ( !m_useVideoCard )
    {
        return;
    }

// FIXME: add effects list along with proper bookkeeping of rendered frames for the interlacer

    //GPURenderPreVideo(); //With all effects

/*
    if not DisplayAsVideoOutput:
        BlitToWindow()
    else:
        GPURenderPreVideo()
        BlitToWindow()

        if PushToVideoCard:
            Readback()
            Push()


    if not PushToVideoCard:
        else:
            BlitToWindow()
*/

    if( m_width != (unsigned int) renderer->GetWidth() || m_height != (unsigned int) renderer->GetHeight() )
    {
        m_width = renderer->GetWidth();
        m_height = renderer->GetHeight();

        printf( "Framebuffer resolution changed to %dx%d\n", m_width, m_height );
    }

    auto frame = offscreenRenderLogic->ReadDisplayTarget( renderer, m_curReadbackFrame );
    m_curReadbackFrame = ( m_curReadbackFrame + 1 ) % offscreenRenderLogic->NumReadBuffersPerRT();

    PushToVideoCard( frame, videoCardManager );

    offscreenRenderLogic->SwapDisplayRenderTargets();


}

// *********************************
//
void    DefaultVideoOutputRenderLogic::PushToVideoCard  ( Texture2DConstPtr & frame, videocards::VideoCardManager * videoCardManager )
{
    //GPUDirect;
	if(videoCardManager->IsEnabled())
	{
		if( videoCardManager->m_CurrentTransferMode == bv::videocards::VideoCard_RAM_GPU::GPU )
		{          
			//m_offscreenRenderLogic->TransferFromGPUToSDI( renderer, m_VideoCardManager );
			//m_offscreenRenderLogic->SwapDisplayRenderTargets();
			//todo: fix gpu direct
		}
		else if( videoCardManager->m_CurrentTransferMode==bv::videocards::VideoCard_RAM_GPU::RAM )
		{
			videoCardManager->GetBufferFromRenderer( frame );
		}
	}
}

} //bv
