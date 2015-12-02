#include "ScreenShotLogic.h"

#include "Rendering/OffscreenRenderLogic.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "LibImage.h"

namespace bv
{

// ***********************
//
ScreenShotLogic::ScreenShotLogic()
    :   m_makeScreenShot( false ),
        m_curReadbackFrame( 0 )
{}

// ***********************
//
ScreenShotLogic::~ScreenShotLogic()
{}

// ***********************
// Next frame will be written to file.
void ScreenShotLogic::MakeScreenShot( const std::string& filePath )
{
    m_filePath = filePath;
    m_makeScreenShot = true;
}

// ***********************
//
void ScreenShotLogic::FrameRendered   (  Renderer* renderer, OffscreenRenderLogic* offscreenRenderLogic )
{
    
    //offscreenRenderLogic->DrawWithAllVideoEffects( renderer );
    offscreenRenderLogic->DrawDisplayRenderTarget( renderer );

    if( ReadbackNeeded() )
    {
        auto frame = offscreenRenderLogic->ReadDisplayTarget( renderer, m_curReadbackFrame );
        //m_curReadbackFrame = ( m_curReadbackFrame + 1 ) % offscreenRenderLogic->NumReadBuffersPerRT();

        auto chunk = frame->GetData();
        image::SaveBMPImage( m_filePath, chunk, frame->GetWidth(), frame->GetHeight(), 32 );

        m_makeScreenShot = false;
    }
    offscreenRenderLogic->SwapDisplayRenderTargets();
    //offscreenRenderLogic->SwapDisplayRenderTargets();
}

// ***********************
//
bool ScreenShotLogic::ReadbackNeeded  ()
{    return m_makeScreenShot;   }

} //bv
