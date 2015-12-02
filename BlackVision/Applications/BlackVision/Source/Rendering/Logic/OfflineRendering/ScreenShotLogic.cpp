#include "ScreenShotLogic.h"

#include "Rendering/OffscreenRenderLogic.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "LibImage.h"

namespace bv
{

// ***********************
//
ScreenShotLogic::ScreenShotLogic()
    :   m_remainingFrames( 0 ),
        m_allFrames( 0 )
{}

// ***********************
//
ScreenShotLogic::~ScreenShotLogic()
{}

// ***********************
// Next frame will be written to file.
void ScreenShotLogic::MakeScreenShot( const std::string& filePath, unsigned int numFrames )
{
    m_filePath = filePath;
    m_remainingFrames = numFrames;
    m_allFrames = numFrames;
}

// ***********************
//
void ScreenShotLogic::FrameRendered   (  Renderer* renderer, OffscreenRenderLogic* offscreenRenderLogic )
{
    
    //offscreenRenderLogic->DrawWithAllVideoEffects( renderer );
    offscreenRenderLogic->DrawDisplayRenderTarget( renderer );

    if( ReadbackNeeded() )
    {
        std::string newFilePath = m_filePath + std::to_string( m_allFrames - m_remainingFrames ) + ".bmp";

        auto frame = offscreenRenderLogic->ReadDisplayTarget( renderer, 0 );

        auto chunk = frame->GetData();
        image::SaveBMPImage( newFilePath, chunk, frame->GetWidth(), frame->GetHeight(), 32 );

        --m_remainingFrames;
    }
    offscreenRenderLogic->SwapDisplayRenderTargets();
    //offscreenRenderLogic->SwapDisplayRenderTargets();
}

// ***********************
//
bool ScreenShotLogic::ReadbackNeeded  ()
{    return m_remainingFrames > 0 ? true : false;   }

} //bv
